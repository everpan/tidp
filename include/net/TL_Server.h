/*
 * TL_Server.h
 *
 *  Created on: Dec 27, 2014
 *      Author: ever
 */

#ifndef INCLUDE_NET_TL_SERVER_H_
#define INCLUDE_NET_TL_SERVER_H_

#include <iostream>
#include <map>
#include <set>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/un.h>
#include <string>
#include <sys/epoll.h>

#include <TL_Thread.h>
#include <net/TL_Socket.h>
#include <net/TL_SessionManger.h>
#include <net/TL_PacketManger.h>
#include <net/TL_ExecuteThreadFactory.h>
#include <TL_Logger.h>
namespace tidp {
namespace net {
#include <errno.h>


typedef std::function<int(string&,TL_PacketPtr&)> ParseFun;
class TL_Server: public TL_Thread {
public:
	TL_Server();
	virtual ~TL_Server();
	void bind(const char *sServerAddr, int port);
	void bind(const char *sPathName);
	unsigned int getSessionAutoIncrementId();
	virtual void run();
	void setExecuteNumber(int num);
	virtual void startExecuetThread();
	void setExecuteThreadFactory(TL_ExecuteThreadFactoryPtr& factory);
	TL_ExecuteThreadFactoryPtr getExecuteThreadFactory();
	void setParseFun(const ParseFun& fun);
	bool getRequest(TL_PacketPtr& pt);
private:
	//处理请求 线程数
	int _thread_num;
	int _epfd;
	unsigned int _session_auto_increment_id;
	struct epoll_event * _events;
	char * _recv_buff;
	ParseFun _parse_fun;
	//会话管理
	TL_SessionManger _session_manger;
	TL_PacketManger _packet_manger;
	//TL_ExecuteManger _execute_manger;
	TL_ExecuteThreadFactoryPtr _execute_thread_factory;
	TL_ThreadQueue<TL_PacketPtr> _request_queue;
	vector<TL_ExecuteThreadPtr> _execute_threads;
	TL_Socket _sock;
};

} /* namespace net */
} /* namespace tidp */

#endif /* INCLUDE_NET_TL_SERVER_H_ */
