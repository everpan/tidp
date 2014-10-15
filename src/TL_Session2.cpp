/*
 * TL_Session2.cpp
 *
 *  Created on: Jul 23, 2014
 *      Author: ever
 */

#include "TL_Session2.h"
#include "TL_Server.h"
#define TL_Session2_VERSION 1
namespace tidp {

//todo 处理完将协议解析切换至default
void sock5proxyProtocol(char * buff, int len, TL_Session2& session) {

}

static void httpGetProtocol(char * buff, int len, TL_Session2& session) {
	//标准解析，直到\r\n\r\n
	TL_PacketPtr& packet = session.getCurrentPacket();
	int osize = packet->recv_data.size();
	string::size_type pos;
	packet->recv_data.append(buff, len);
	do {
		//回溯4个字节查找
		pos = packet->recv_data.find("\r\n\r\n", osize >= 4 ? osize - 4 : 0);
		if (pos != string::npos) {
			if (pos + 4 < packet->recv_data.size()) {
				//粘包，分离下一个。
				TL_PacketPtr& packet2 = session.getNewPacket();
				packet2->recv_data = packet->recv_data.substr(pos + 4);
				packet2->protocol_type = TL_Packet::HTTP_GET;
				packet->recv_data.erase(pos + 4, string::npos);
				packet->server->addRequest(packet);
				packet = packet2;
				osize = packet->recv_data.size();
				//pos = packet->recv_data.find("\r\n\r\n", osize >= 4 ? osize - 4 : 0);
				continue;
			} else if (pos + 4 == packet->recv_data.size()) {
				packet->server->addRequest(packet);
				packet = session.getNewPacket();
				break;
			} else {
				break;
			}
		}
	} while (pos != string::npos);
}

static void httpPostProtocol(char * buff, int len, TL_Session2& session) {
	//标准解析，直到\r\n\r\n
	TL_PacketPtr& packet = session.getCurrentPacket();
	string::size_type pos = 0;
	packet->recv_data.append(buff, len);
	//cout << "post:" << packet->recv_data << endl;
	unsigned int _content_length = 0;
	do {
		//回溯4个字节查找
		//_content_length = 0;
		if (packet->extraData == NULL) {
			size_t osize = packet->recv_data.size() - len;
			pos = packet->recv_data.find("\r\n\r\n", osize >= 4 ? osize - 4 : 0);
			if (pos != string::npos) {
				pos += 4;
				packet->extraData = (void*) pos;
			}
		} else {
			pos = (int) packet->extraData;
		}
		cout << "pos:" << pos << endl;
		if (pos != string::npos) {
			//这里将首部全部转换成小写
			for (int i = 0; i < (int) pos; ++i) {
				char & c = packet->recv_data[i];
				if (c < '[' && c > '@') {
					c += 32;
				}
			}
			string::size_type pos2 = packet->recv_data.find("content-length");
			if (pos2 != string::npos) {
				pos2 = packet->recv_data.find(":", pos2);
				if (pos2 != string::npos) {
					_content_length = strtol(&packet->recv_data[pos2 + 1], NULL, 10);
					if (packet->recv_data.size() - pos > _content_length) {
						TL_PacketPtr& packet2 = session.getNewPacket();
						packet2->recv_data = packet->recv_data.substr(pos);
						packet2->protocol_type = TL_Packet::HTTP_POST;
						packet->recv_data.erase(pos, string::npos);
						packet->server->addRequest(packet);
						packet->extraData = NULL;
						packet = packet2;
						continue;
					} else if (packet->recv_data.size() - pos == _content_length) {
						packet->extraData = NULL;
						packet->server->addRequest(packet);
						packet.reset();
						break;
					}
				}
			} else {
				throw TL_Exception("POST : not Found Content-Length," + packet->recv_data, -1);
			}
			break;
		}
	} while (pos != string::npos);
}
/**
 * 协议辨别与解析
 * */
static void defaultParseProtocol(char * buff, int len, TL_Session2& session) {
	//cout << "defaultParse:" << buff << endl;
	TL_PacketPtr& packet = session.getCurrentPacket();
	if (packet->protocol_type == TL_Packet::UNKNOWN) {
		string & recv_data = packet->recv_data;
		if (recv_data.size() == 0) {
			if (len >= 4) {
				if (strncasecmp(buff, "get ", 4) == 0) {
					packet->protocol_type = TL_Packet::HTTP_GET;
					session.setParseFun(httpGetProtocol);
					httpGetProtocol(buff, len, session);
					return;
				} else if (strncasecmp(buff, "post", 4) == 0) {
					packet->protocol_type = TL_Packet::HTTP_POST;
					session.setParseFun(httpPostProtocol);
					httpPostProtocol(buff, len, session);
					return;
				}
			} else if (len > 0) {
				if (buff[0] == (char) 0x02) {
					//binary magic num 0x02300230
					packet->protocol_type = TL_Packet::TL_BINARY;
				} else {
					packet->protocol_type = TL_Packet::LINE_STRING;
				}
			}
		} else {
			if (recv_data[0] == (char) 0x02) {
				packet->protocol_type = TL_Packet::TL_BINARY;
			} else {
				packet->protocol_type = TL_Packet::LINE_STRING;
			}
		}
	}

	if (packet->protocol_type == TL_Packet::LINE_STRING) {
		int p = 0;
		int i = 0;
		while (i < len) {
			if (i == 0 && buff[i] == '\n') {
				if (packet->recv_data.size() > 0) {
					packet->server->addRequest(packet);
					packet = session.getNewPacket();
				}
				++i;
				++p;
			}

			while (i < len && buff[i] != '\n') {
				++i;
			}

			if (buff[i] == '\n') {
				if (i > 0 && buff[i - 1] == '\r') {
					packet->recv_data.append(buff + p, i - p - 1);
				} else {
					packet->recv_data.append(buff + p, i - p);
				}
				packet->server->addRequest(packet);
				packet = session.getNewPacket();
				p = i + 1;
				if (i == len) {
					break;
				}
				++i;
			} else if (i > p) {
				packet->recv_data.append(buff + p, i - p);
			}
		}
	} else {
		cout << "unknow protocol:" << packet->protocol_type << "|" << len << "|" << buff << endl;
	}
}

TL_Session2::TL_Session2(int fd, int port, const string& ip) {
	_my_io = NULL;
	_recv_len = 0;
	_recv_buff = (char*) malloc(RECV_BUFF_SIZE);
	_retry_cnt = 0;
	_sockfd = fd;
	_port = port;
	_server_id = 0;
	_session_id = 0;
	_message_id = time(0);
	_parse_fn = defaultParseProtocol;
	_server = NULL;
	_ip = ip;
}
TL_Session2::~TL_Session2() {
	if (_recv_buff) {
		free(_recv_buff);
		_recv_buff = NULL;
	}

	close();
	cout << "TL_Session2::~TL_Session2" << endl;
}

bool TL_Session2::isConnected() {
	return _sockfd > 0;
}
void TL_Session2::setParseFun(ParseFunt* fun) {
	_parse_fn = fun;
}
int TL_Session2::getFd() {
	return _sockfd;
}

void TL_Session2::setMyEvIo(struct ev_io *my_io) {
	_my_io = my_io;
}

void TL_Session2::setServerId(unsigned int sid) {
	_server_id = sid;
}

unsigned int TL_Session2::getServerId() {
	return _server_id;
}

void TL_Session2::setSessionId(unsigned int sid) {
	_session_id = sid;
}
unsigned int TL_Session2::getSessionId() const {
	return _session_id;
}
void TL_Session2::setServer(TL_Server* server) {
	_server = server;
}
TL_Server * TL_Session2::getServer() {
	return _server;
}
int TL_Session2::read() {
	int ret = -1;
	try {
		do {
			ret = ::read(_sockfd, _recv_buff, RECV_BUFF_SIZE);
			if (ret <= 0) {
				return ret;
			}
			_parse_fn(_recv_buff, ret, *this);
		} while (ret == RECV_BUFF_SIZE);
	} catch (...) {
		cout << "read unknown exception." << endl;
		return -2;
	}
	return ret;
}
int TL_Session2::sendByLocked(const string& buff) {
	Lock(*this);
	return send(buff.c_str(), buff.size());
}
int TL_Session2::send(const string& buff) {
	return send(buff.c_str(), buff.size());
}
int TL_Session2::send(const char * send_buff, int send_len) {
	if (_sockfd < 0)
		return -1;
	int sent_len = 0;
	int ret = 0;
	if (send_len > 0) {
		do {
			ret = ::write(_sockfd, send_buff, send_len - sent_len);
			if (ret > 0) {
				sent_len += ret;
				send_buff += ret;
			} else {
				return ret;
			}
		} while (sent_len < send_len);
	}
	return sent_len;
}
void TL_Session2::close() {
	if (_sockfd > 0) {
		::close(_sockfd);
		_sockfd = -1;
	}
	/*
	 if (_my_io) {
	 delete _my_io;
	 _my_io = NULL;
	 }*/
	if (_packetPtr)
		_packetPtr.reset();
}
void TL_Session2::unregister() {
	_server->removeSession(shared_from_this());
}

TL_Session2 * TL_Session2::createSession(int fd, int port, const string& ip) {
	return new TL_Session2(fd, port, ip);
}

TL_PacketPtr& TL_Session2::getCurrentPacket() {
	cout << __FUNCTION__ << endl;
	if (!_packetPtr) {
		_packetPtr = getNewPacket();
	}
	return _packetPtr;
}
TL_PacketPtr& TL_Session2::getNewPacket() {
	_packetPtr = _server->getPacket();
	_packetPtr->session = shared_from_this();
//cout << "_packetPtr->session" << _packetPtr->session.use_count() << endl;
	return _packetPtr;
}
//----------------------------------------------------------------------------------
TL_Session2Data::TL_Session2Data(TL_Session2Ptr& session_param)
		: session(session_param) {
	messige_id = 0;
	try_cnt = 0;
	recv_time = 0;
	send_time = 0;
}
}
