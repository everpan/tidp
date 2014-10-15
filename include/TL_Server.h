/*
 * TL_Server.h
 *
 *  Created on: Jul 24, 2014
 *      Author: ever
 */

#ifndef TSCHEDULESERVER_H_
#define TSCHEDULESERVER_H_

#include <iostream>
#include <map>
#include <set>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>
#include <arpa/inet.h>
/*
 #ifdef EV_COMMON
 #undef EV_COMMON
 #endif

 #ifndef EV_COMMON
 #define EV_COMMON void * data[2];
 #endif
 */
#include <unistd.h>
#include <sys/un.h>
#include <string>
//#include <ev.h>
//#include <boost/serialization/singleton.hpp>
#include <boost/bind.hpp>
#include <sys/epoll.h>

#include "TL_ExecuteFactory.h"
#include "TL_ThreadCache.h"
#include "TL_Thread.h"
#include "TL_ThreadLock.h"
#include "TL_Session2.h"
#include "TL_Packet.h"
#include "TL_Common.h"

//#include "TL_ExecuteThread.h"

using namespace std;
namespace tidp {
class TL_ExecuteThread;
class TL_Server: public TL_Thread /*: public boost::serialization::singleton<TL_Server>*/{
public:
	typedef set<TL_Session2Ptr, TL_Session2::TL_Session2Cmp> SessionType;
	TL_Server();
	TL_Server(const string& ip, short port);
	TL_Server(const string& unixlocal);
	virtual ~TL_Server();
	//void initEpoll();
	static unsigned int getSessionId();
	void setTaskThreadNum(int num);
	virtual void run();

	//static void acceptCB(EV_P_ ev_io *w, int revents);
	//static void readCB(EV_P_ ev_io *w, int revents);

	void setExecuteFactory(const TL_ExecuteFactoryPtr& factory_ptr);
	TL_ExecutePtr getExecute(unsigned int command_id);
public:
	const TL_Session2Ptr & addSession(const TL_Session2Ptr& session);
	void removeSession(const TL_Session2Ptr& session);
	TL_Session2Ptr getSessionPtr(unsigned int session_id);
	TL_PacketPtr getPacket();
	void release(TL_Packet* data);

	void addRequest(const TL_PacketPtr& request);
	void getRequest(TL_PacketPtr& request);
	void sendPacket(const TL_PacketPtr& response);
	void addResponse(const TL_PacketPtr& response);
	//static void setNonblock(int fd);

	void setErrorMessage(const string& errmsg);
	const string& getErrorMessage();
	void getErrorMessage(string& errmsg);

private:
	static volatile unsigned int _max_session_id;
	int _sockfd;
	int _epfd;
	int _task_thread_num;
	//struct ev_loop * _loop;
	//struct ev_io _svr_io;
	struct epoll_event * _events;
	string _errmsg;
	//SessionType _sessions;
	map<string, SessionType> _ip_sessions;
	map<unsigned int, TL_Session2Ptr> _id_sessions;
	tidp::TL_ThreadDataPool<TL_Packet> _data_pool;
	TL_ExecuteFactoryPtr _execute_factory_ptr;
	set<TL_PacketPtr> _recv_pool; //正常情况下主线程读取，放入其中 Execute 获取执行。
	tidp::TL_ThreadLock _recv_lock;
	tidp::TL_ThreadLock _send_lock;
	tidp::TL_ThreadLock _pack_lock;
	set<TL_PacketPtr> _send_pool; //多个处理线程公用，需要加锁。

	//业务线程池
	set<TL_ExecuteThread*> _task_threads;
};
}
#endif /* TSCHEDULESERVER_H_ */
