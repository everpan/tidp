/*
 * TL_Redis.cpp
 *
 *  Created on: Aug 18, 2014
 *      Author: ever
 */

#include <TL_Redis.h>

namespace tidp {

TL_Redis::TL_Redis() {
	_sockfd = -1;
	_read_cache = (char*) malloc(REDIS_CACHE_SIZE);
	_send_cache = (char*) malloc(REDIS_CACHE_SIZE);
	_send_len = 0;
	_read_len = 0;
	_data_start = _read_cache;

	_port = 6379;
	if ((_epfd = epoll_create(10)) == -1) {
		throw TL_Exception("epoll_create1 fail.", errno);
	}
}

TL_Redis::~TL_Redis() {
	if (_read_cache) {
		free((void*) _read_cache);
		_read_cache = NULL;
	}
	if (_send_cache) {
		free((void*) _send_cache);
		_send_cache = NULL;
	}
	if (_sockfd > 0) {
		::close(_sockfd);
		_sockfd = -1;
	}
	if (_epfd > 0) {
		::close(_epfd);
		_epfd = -1;
	}
}

TL_Redis::Reply::Reply() {
	flag = 0;
	length = 0;
	status = 0;
	data = NULL;
}

void TL_Redis::Reply::clear() {
	flag = 0;
	length = 0;
	status = 0;
	data = NULL;
}

void TL_Redis::connect(const string& host, short port) {
	_host = host;
	_port = port;
	_sockfd = TL_Common::connect(host, port);
	TL_Common::setNonBlock(_sockfd);
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = _sockfd;
	if (epoll_ctl(_epfd, EPOLL_CTL_ADD, _sockfd, &ev) == -1) {
		throw TL_Exception("epoll_ctrl fail.", errno);
	}
}

void TL_Redis::reConnect() {
	close();
	connect(_host, _port);
}

void TL_Redis::close() {
	if (_sockfd > 0) {
		::close(_sockfd);
		_sockfd = -1;
	}
}

void TL_Redis::flush() {
	if (_sockfd > 0) {
		int ret = 0;
		while (ret != _send_len) {
			int wlen = write(_sockfd, _send_cache + ret, _send_len - ret);
			if (wlen > 0) {
				ret += wlen;
			} else {
				close();
				throw TL_Exception("send error", errno);
			}
		}
	}
//	cout << "send:";
//	coutCmd(string(_send_cache, _send_len));
//	cout << endl;
	_send_len = 0;
}

void TL_Redis::sendBulkString(const char* s, int len) {
	sendSize(REDIS_BULK_STRING, len);
	send(s, len);
	send("\r\n", 2);
}

void TL_Redis::sendBulkString(const string& s) {
	sendBulkString(s.c_str(), s.size());
}

//read
int TL_Redis::read() {
	int ret = epoll_wait(_epfd, &_ep_event, 1, 100);
	if (ret == -1) {
		throw TL_Exception("epoll_wait fail.", errno);
	}
	int rlen = -1;
	//cout << "read:" << _read_len << "|" << rlen << "|";
	if (ret > 0) {
		if (_read_len > 0 && _data_start - _read_cache >= REDIS_CACHE_SIZE / 2) {
			//cout << "here1:~~~~~~" << endl;
			memcpy(_read_cache, _data_start, _read_len);
			_data_start = _read_cache;
			rlen = ::read(_sockfd, _read_cache + _read_len, REDIS_CACHE_SIZE - _read_len);
		} else if (_read_len == 0) {
			//cout << "here2:~~~~~~" << endl;
			bzero(_read_cache, REDIS_CACHE_SIZE);
			//coutCmd(string(_read_cache, REDIS_CACHE_SIZE));
			rlen = ::read(_sockfd, _read_cache, REDIS_CACHE_SIZE);

			//coutCmd(string(_read_cache, rlen));
			//cout << "*" << (void*) _read_cache << "|" << rlen;
			_data_start = _read_cache;
		} else {
			//cout << "here3:~~~~~~" << endl;
			char * p = _data_start + _read_len;
			rlen = ::read(_sockfd, p, _read_cache + REDIS_CACHE_SIZE - p);
		}
		if (rlen < 0) {
			close();
			throw TL_Exception("read fail.", errno);
		}
		_read_len += rlen;
	}
	//cout << "|rlen:" << rlen << "|" << _read_len << "|";
	//coutCmd(string(_data_start, _read_len));
	//cout << endl;
	return rlen;
}

int TL_Redis::getReply(Reply& reply) {
	if (_read_len == 0) {
		return ReplyMore;
	}
	switch (reply.status) {
		case ReplyFlag:
			if (_read_len > 0) {
				reply.flag = _data_start[0];
				++_data_start;
				--_read_len;
				if (reply.flag == '-' || reply.flag == '+') {
					reply.status = ReplyData;
				} else {
					reply.status = ReplyLen;
				}
			}
			//cout << "flag:" << reply.flag << "|" << (int) reply.flag << endl;
			//coutCmd(string(_data_start, _read_len));
			//cout << endl;
			return ReplyOK;
		case ReplyData: { //-
			int i = 0;
			if (reply.flag == '-' || reply.flag == '+') {
				//simple string
				char *p = _data_start;
				while (_read_len > 0) {
					if (*p == '\r' && _read_len > 1 && *(p + 1) == '\n') {
						string & data = *(string*) reply.data;
						data.append(_data_start, p - _data_start);
						_data_start = p + 2;
						_read_len -= 2;
						return ReplyOK;
					} else {
						--_read_len;
						++i;
						++p;
						if (i > REDIS_CACHE_SIZE / 2) {
							string & data = *(string*) reply.data;
							data.append(_data_start, p - _data_start);
							_data_start = p;
						}
					}
				}
			} else {
				//bulk string
				//cout << "bulk string:";
				//coutCmd(string(_data_start, _read_len));
				//cout << "|     " << _read_len << "|" << reply.length << endl;
				if (_read_len > reply.length) {
					if (reply.length > 0) {
						string & data = *(string*) reply.data;
						data.append(_data_start, reply.length);
						_read_len -= reply.length;
						_data_start += reply.length;
						reply.length = 0;
					}

					if (reply.length == 0) {
//						cout << "bulk string:2|";
//						coutCmd(string(_data_start, _read_len));
//						cout << endl;
						if (_read_len > 1 && _data_start[0] == '\r' && _data_start[1] == '\n') {
							_data_start += 2;
							_read_len -= 2;
							return ReplyOK;
						} else if (_read_len > 1) {
							throw TL_Exception("bulk string end flag is error.", -1);
						} else {
							cout << " need more" << endl;
							return ReplyMore;
						}
					} else {
						return ReplyMore;
					}
				} else { //_read_len <= reply.length
					string & data = *(string*) reply.data;
					data.append(_data_start, _read_len);
					//_data_start -= _read_len;
					reply.length -= _read_len;
//					cout << "bulk string:3|" << _read_len << endl;
					_read_len = 0;
					//cout << "bulk string:3|" << _read_len << endl;
				}
			}
			return ReplyMore;
		}
		case ReplyLen: { //:
			while (_read_len > 0) {
				if (*_data_start == '\r') {
					if (_read_len > 1) {
						_data_start += 2;
						_read_len -= 2;
						//cout << "length(ok):" << reply.length << endl;
						return ReplyOK;
					} else {
						//cout << "length(more1):" << reply.length << "|" << _read_len << endl;
						return ReplyMore;
					}
				} else {
					reply.length = reply.length * 10 + *_data_start - '0';
					--_read_len;
					++_data_start;
				}
			}
			//cout << "length(more2):" << reply.length << endl;
			return ReplyMore;
		}
		case REDIS_BULK_STRING: //$
		case REDIS_ARRAYS: //*
		default:
			return ReplyError;
	}
}

int TL_Redis::readIntegers() {
	_reply.clear();
	int ret = -1;
	NextReply(_reply);
	if (_reply.flag != ':') {
		throw TL_Exception("flag error.", -1);
	}
	NextReply(_reply);
	return _reply.length;
}

void TL_Redis::readBulkString(string& bulk_str) {
	bulk_str.clear();
	TL_Redis::Reply reply;
	int ret = -1;
	NextReply(reply);
	if (reply.flag == '-') {
		reply.status = ReplyData;
		reply.data = (void*) &_errmsg;
		NextReply(reply);
	} else {
		reply.status = ReplyLen;
		reply.length = 0;
		NextReply(reply);

		if (reply.length > 0) {
			reply.status = ReplyData;
			reply.data = (void*) &bulk_str;
			NextReply(reply);
		}
	}
}

void TL_Redis::readArrays(set<string>& arr_str) {
	_reply.clear();
	arr_str.clear();
	int ret = -1;
	NextReply(_reply);
	if (_reply.flag != REDIS_ARRAYS) {
		throw TL_Exception("ret is not array.", -1);
	}
	_reply.status = ReplyLen;
	_reply.length = 0;
	NextReply(_reply);
	if (_reply.length > 0) {
		_reply.status = ReplyData;
		string tmp;
		for (int i = 0; i < _reply.length; ++i) {
			readBulkString(tmp);
			arr_str.insert(tmp);
		}
	}
}

void TL_Redis::readArrays(list<string>& list_str) {
	_reply.clear();
	list_str.clear();
	int ret = -1;
	NextReply(_reply);
	if (_reply.flag != REDIS_ARRAYS) {
		throw TL_Exception("ret is not array.", -1);
	}
	_reply.status = ReplyLen;
	_reply.length = 0;
	NextReply(_reply);
	if (_reply.length > 0) {
		_reply.status = ReplyData;
		string tmp;
		for (int i = 0; i < _reply.length; ++i) {
			readBulkString(tmp);
			list_str.push_back(tmp);
		}
	}
}

void TL_Redis::readArrays(map<string, string>& map_str) {
	_reply.clear();
	map_str.clear();
	int ret = -1;
	NextReply(_reply);
	if (_reply.flag != REDIS_ARRAYS) {
		throw TL_Exception("ret is not array.", -1);
	}
	_reply.status = ReplyLen;
	_reply.length = 0;
	NextReply(_reply);
	if (_reply.length > 0) {
		if (_reply.length % 2 != 0) {
			throw TL_Exception("key val not pair.", -1);
		}
		_reply.status = ReplyData;
		string key, val;
		for (int i = 0; i < _reply.length; i += 2) {
			readBulkString(key);
			readBulkString(val);
			map_str[key] = val;
		}
	}
}

int TL_Redis::select(int db) {
	sendCMD("SELECT", 6, 2);
	int rlen1 = snprintf(_len_cache + 16, 31 - 16, "%d", db);
	_len_cache[0] = REDIS_BULK_STRING;
	int rlen2 = snprintf(_len_cache + 1, 16, "%d", rlen1);
	_len_cache[++rlen2] = '\r';
	_len_cache[++rlen2] = '\n';
	++rlen2;
	memcpy(_len_cache + rlen2, _len_cache + 16, rlen1);
	rlen2 += rlen1;
	_len_cache[rlen2] = '\r';
	_len_cache[++rlen2] = '\n';
	//_len_cache[]
	send(_len_cache, rlen2 + 1);
	flush();
	int ret = -1;
	_reply.clear();
	NextReply(_reply);
	_reply.data = (void*) &_errmsg;
	_reply.status = ReplyData;
	NextReply(_reply);
	//cout << "select db:" << db << ":" << _errmsg << endl;
	if (_reply.flag == '+')
		return db;
	else
		return -1;
	//return _reply.flag == '+' ? 1 : -1;
}

int TL_Redis::select(const string& db) {
	sendCMD("SELECT", 6, 2);
	sendBulkString(db);
	flush();
	int ret = -1;
	_reply.clear();
	NextReply(_reply);
	_reply.data = (void*) &_errmsg;
	_reply.status = ReplyData;
	NextReply(_reply);
	if (_reply.flag == '+') {
		_errmsg.clear();
		return 1;
	}
	return -1;
	//return _reply.flag == '+' ? 1 : -1;
}

int TL_Redis::keys(const string& pattern, set<string>& keys) {
	sendCMD("KEYS", 4, 2);
	sendBulkString(pattern);
	flush();
	readArrays(keys);
	return keys.size();
}

int TL_Redis::hset(const string& key, const string& field, const string& val) {
	sendCMD("HSET", 4, 4);
	sendBulkString(key);
	sendBulkString(field);
	sendBulkString(val);
	flush();
	return readIntegers();
}

int TL_Redis::hget(const string& key, const string& field, string & val) {
	sendCMD("HGET", 4, 3);
	sendBulkString(key);
	sendBulkString(field);
	flush();
	readBulkString(val);
	return val.size();
}

int TL_Redis::hdel(const string& key, const string& field) {
	sendCMD("HDEL", 4, 3);
	sendBulkString(key);
	sendBulkString(field);
	flush();
	return readIntegers();
}

int TL_Redis::hdel(const string& key, const set<string>& fields) {
	sendCMD("HDEL", 4, 2 + fields.size());
	sendBulkString(key);
	set<string>::const_iterator it = fields.begin();
	while (it != fields.end()) {
		sendBulkString(*it);
		++it;
	}
	flush();
	return readIntegers();
}

int TL_Redis::hmget(const string& key, const list<string>& fieldslist, map<string, string>& fieldvals) {
	sendCMD("HMGET", 5, 2 + fieldslist.size());
	sendBulkString(key);
	list<string>::const_iterator it = fieldslist.begin();
	while (it != fieldslist.end()) {
		sendBulkString(*it);
		++it;
	}
	flush();

	list<string> retlist;
	readArrays(retlist);
	if (fieldslist.size() != retlist.size()) {
		throw TL_Exception("key val not pair.", -1);
	}
	fieldvals.clear();
	list<string>::const_iterator it1 = fieldslist.begin();
	list<string>::const_iterator it2 = retlist.begin();
	while (it1 != fieldslist.end()) {
		fieldvals[*it1] = *it2;
		++it1;
		++it2;
	}
	return fieldvals.size();
}

int TL_Redis::hkeys(const string& key, set<string>& fields) {
	sendCMD("HKEYS", 5, 2);
	sendBulkString(key);
	flush();
	readArrays(fields);
	return fields.size();
}

int TL_Redis::hexists(const string& key, const string& field) {
	sendCMD("HEXISTS", 7, 3);
	sendBulkString(key);
	sendBulkString(field);
	flush();
	return readIntegers();
}

int TL_Redis::hgetall(const string& key, map<string, string>& fieldvals) {
	sendCMD("HGETALL", 7, 2);
	sendBulkString(key);
	flush();
	readArrays(fieldvals);
	return fieldvals.size();
}

void TL_Redis::coutCmd(const string &s) {
	int i = 0;
	cout << "    ";
	while (i < (int) s.size()) {
		if (s[i] == '\r') {
			cout << "\\r";
		} else if (s[i] == '\n') {
			cout << "\\n";
		} else if (s[i] < '!') {
			cout << "\\" << (int) s[i];
		} else {
			cout << s[i];
		}
		++i;
	}
}

} /* namespace tidp */
