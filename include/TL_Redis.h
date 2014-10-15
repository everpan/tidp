/*
 * TL_Redis.h
 *
 *  Created on: Aug 18, 2014
 *      Author: ever
 */

#ifndef TL_REDIS_H_
#define TL_REDIS_H_

#include <string>
#include <map>
#include <set>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef __linux__
#include <sys/epoll.h>
#endif

#include "TL_Common.h"
//#include <sstream>
namespace tidp {
using namespace std;

#define REDIS_SIMPLE_STRING '+' //不可以带换行符号 +OK\r\n
#define REDIS_ERRORS '-'
#define REDIS_INTEGERS ':'
#define REDIS_BULK_STRING '$' //max 512M $6\r\nfoobar\r\n || nil is $-1\r\n
#define REDIS_ARRAYS '*' //*0\r\n || *2\r\n$3\r\nfoo\r\n$3\r\nbar\r\n
#define REDIS_FINAL "\r\n"

#ifndef REDIS_CACHE_SIZE
#define REDIS_CACHE_SIZE 5
#endif

#define NextReply(reply) do {ret = getReply(reply); if(ret == ReplyMore){read();}} while (ret != ReplyOK)
class TL_Redis {
public:
	struct Reply {
		char flag; //标识
		int length; //长度
		int status;
		void * data;
		Reply();
		void clear();
		void show() {
			cout << "Reply:" << flag << "|" << length << "|" << status << endl;
		}
	};
	enum Reply_flag {
		ReplyError = -1, //error
		ReplyFlag = 0,
		ReplyLen = 1, //计算长度
		ReplyData = 2,
		ReplyMore = 3,
		ReplyOK = 4
	};
	TL_Redis();
	virtual ~TL_Redis();

	void connect(const string& host, short port);
	void reConnect();
	void close();
	void send(const char* buf, int len);
	void flush();
	void sendSize(char first_type, int len);
	void sendCMD(const char * cmd, int clen, int plen);
	void sendBulkString(const char* s, int len);
	void sendBulkString(const string& s);
	void sendSimpleString(const char* s, int len);
	//read
	int read();
	int getReply(Reply& Reply);
	int readIntegers();
	void readBulkString(string& bulk_str);
	void readArrays(set<string>& set_str);
	void readArrays(list<string>& list_str);
	void readArrays(map<string, string>& map_str);

	int select(int db = 0);
	int select(const string& db = "0");
	//---
	int keys(const string& pattern, set<string>& keys);
	//--- hash
	int hset(const string& key, const string& field, const string& val);
	int hget(const string& key, const string& field, string & val);
	int hdel(const string& key, const string& field);
	int hdel(const string& key, const set<string>& fields);
	int hmget(const string& key, const list<string>& fieldslist, map<string, string>& fieldvals);
	int hkeys(const string& key, set<string>& fields);

	int hexists(const string& key, const string& field);
	int hgetall(const string& key, map<string, string>& fieldvals);
	//--- list
	//Get the length of a list
	int llen(const string& key);
	int lset(const string& key, int idnex, const string& val);
	int lget(const string& key, int index, string & val);
	//Remove and get the first element in a list
	int lpop(const string& key, string& val);
	//Remove and get the first element in a list
	int lrange(const string& key, int start, int stop, set<string>& vals);

	static void coutCmd(const string &s);

private:
	int _sockfd;
	char _len_cache[32];
	char * _read_cache;
	char * _send_cache;
	int _read_len;
	int _send_len;
	char * _data_start;

	string _host;
	string _errmsg;
	int _port;
	Reply _reply;
#ifdef __linux__
	int _epfd;
	struct epoll_event _ep_event;
#else
	fd_set _fdset;
#endif
};
//in line function
inline void TL_Redis::send(const char* buf, int len) {
	const char *p = buf;
	if (len + _send_len >= REDIS_CACHE_SIZE) {
		flush();
		int ret = 0;
		while (len >= REDIS_CACHE_SIZE) {
			ret = ::write(_sockfd, p, REDIS_CACHE_SIZE);
			//coutCmd(string(p,ret));
			if (ret > 0) {
				len -= ret;
				p += ret;
			} else {
				close();
				throw TL_Exception("send error", errno);
			}
		}
	}
	if (len > 0) {
		memcpy(_send_cache + _send_len, p, len);
		_send_len += len;
	}
}

inline void TL_Redis::sendSimpleString(const char* s, int len) {
	send("+", 1);
	send(s, len);
	send("\r\n", 2);
}

inline void TL_Redis::sendSize(char first_type, int len) {
	_len_cache[0] = first_type;
	int rlen = snprintf(_len_cache + 1, 31, "%d", len);
	send(_len_cache, rlen + 1);
	send("\r\n", 2);
}
inline void TL_Redis::sendCMD(const char * cmd, int clen, int plen) {
	sendSize(REDIS_ARRAYS, plen);
	sendBulkString(cmd, clen);
}
} /* namespace tidp */

#endif /* TL_REDIS_H_ */
