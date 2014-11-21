/*
 * TL_Session.h
 *
 *  Created on: Jul 23, 2014
 *      Author: ever
 */

#ifndef TCONNECTSESSION_H_
#define TCONNECTSESSION_H_
#include <sys/types.h>
#include <sys/uio.h>

#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <arpa/inet.h>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <TL_ThreadLock.h>
#include "TL_Packet.h"
using namespace std;
//struct ev_io;
namespace tidp {
class TL_Server;
class TL_Session;
typedef boost::shared_ptr<TL_Session> TL_SessionPtr;
#define RECV_BUFF_SIZE 2048

typedef void (ParseFunt)(const char * buff, int len, TL_Session& session);
class TL_Session: public tidp::TL_ThreadLock, public boost::enable_shared_from_this<TL_Session> {
public:
	//TL_Session();
	explicit TL_Session(int fd, int port, const string& ip);
	virtual ~TL_Session();
	bool isConnected();
	void setParseFun(ParseFunt* fun);
	int getFd();
	//void setMyEvIo(struct ev_io *my_io);
	void setServerId(unsigned int sid);
	unsigned int getServerId();
	void setSessionId(unsigned int sid);
	unsigned int getSessionId() const;
	void setServer(TL_Server* server);
	TL_Server * getServer();
	int read();
	int sendByLocked(const string& buff);
	int send(const string& buff);
	int send(const char * buff,int len);
	void close();
	void unregister();
	static TL_Session * createSession(int fd, int port, const string& ip);
	TL_PacketPtr& getCurrentPacket();
	TL_PacketPtr getNewPacket();
	struct TL_SessionCmp {
		bool operator ()(const TL_SessionPtr& r, const TL_SessionPtr& l) {
			return r->_ip < l->_ip || (r->_ip == l->_ip && r->_port < l->_port);
		}
	};
private:
	//int _errno;
	//struct ev_io * _my_io;
	int _recv_len;
	char *_recv_buff;
	int _retry_cnt; //重试次数，>0的情况下，需要等待对方返回确认包。
	int _sockfd;
	int _port; //
	unsigned int _server_id;
	unsigned int _session_id;
	unsigned int _message_id;
	ParseFunt * _parse_fn;
	TL_Server * _server;
	string _ip;
	//string auth; //认证id
	TL_PacketPtr _packetPtr;
};

/**
 * LTV : length | type | version | command | message id | value
 *        4 | 4 | 4 | 4 | 8 | stream
 *        type: 0-approve 1-request 2-request_ack 3-response 4-response_ack
 */

struct TL_SessionData {
	unsigned int try_cnt; //尝试发送次数
	unsigned int messige_id; //信息ID 由发送方生成
	time_t recv_time; //接受数据时间
	time_t send_time; //发送时间
	string recv_data; //接受数据
	string send_data; //发送数据
	TL_SessionPtr session; //连接会话
	TL_SessionData(TL_SessionPtr& session_param);
};
}
#endif /* TCONNECTSESSION_H_ */
