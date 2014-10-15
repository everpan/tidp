/*
 * TL_Server.cpp
 *
 *  Created on: Jul 24, 2014
 *      Author: ever
 */

#include "TL_Server.h"
#include "TL_ExecuteThread.h"
#define MAX_LISTEN_SIZE 1024
#define MAX_EPOLL_CREATE_SIZE 2048
#define MAX_EPOLL_EVENTS_SIZE 4096
namespace tidp {
volatile unsigned int TL_Server::_max_session_id = 0;
TL_Server::TL_Server() {
	_sockfd = -1;
	_epfd = -1;
	_task_thread_num = 1;
	//_loop = ev_loop_new(EVFLAG_NOENV);
	setExecuteFactory(TL_ExecuteFactoryPtr(new TL_ExecuteFactory));
}
TL_Server::TL_Server(const string& ip, short port) {
	_sockfd = -1;
	_epfd = -1;
	_task_thread_num = 1;
	//TL_Common::setSocketReuseAddr(_sockfd);
	_sockfd = TL_Common::bind(ip, port);
	TL_Common::setNonBlock(_sockfd);
	int ret = listen(_sockfd, MAX_LISTEN_SIZE);
	if (ret != 0) {
		throw TL_Exception("listen error:", errno);
	}
	_epfd = epoll_create(MAX_EPOLL_CREATE_SIZE);
	if (_epfd < 1) {
		throw TL_Exception("epoll_create error:", errno);
	}
	struct epoll_event ev;
	ev.data.fd = _sockfd;
	ev.events = EPOLLIN;
	ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, _sockfd, &ev);
	if (ret != 0) {
		throw TL_Exception("epoll_ctl error:", errno);
	}
	_events = new struct epoll_event[MAX_EPOLL_EVENTS_SIZE];

	setExecuteFactory(TL_ExecuteFactoryPtr(new TL_ExecuteFactory));
}

TL_Server::TL_Server(const string& unixlocal) {
	_sockfd = -1;
	_epfd = -1;
	_task_thread_num = 1;
	unlink(unixlocal.c_str());
	//_sockfd = TL_Common::connectUnix(unixlocal);
	int ret = listen(_sockfd, MAX_LISTEN_SIZE);
	if (ret != 0) {
		throw TL_Exception("listen error:", errno);
	}
	_epfd = epoll_create(MAX_EPOLL_CREATE_SIZE);
	if (_epfd < 1) {
		throw TL_Exception("epoll_create error:", errno);
	}
	struct epoll_event ev;
	ev.data.fd = _sockfd;
	ev.events = EPOLLIN;
	ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, _sockfd, &ev);
	if (ret != 0) {
		throw TL_Exception("epoll_ctl error:", errno);
	}

	_events = new struct epoll_event[MAX_EPOLL_EVENTS_SIZE];

	setExecuteFactory(TL_ExecuteFactoryPtr(new TL_ExecuteFactory));
}
TL_Server::~TL_Server() {
	/*
	 if (_loop) {
	 ev_loop_destroy(_loop);
	 _loop = NULL;
	 }*/
	set<TL_ExecuteThread*>::const_iterator it = _task_threads.begin();
	while (it != _task_threads.end()) {
		delete *it;
		++it;
	}
	_task_threads.clear();

	if (_events) {
		delete[] _events;
		_events = NULL;
	}

}

unsigned int TL_Server::getSessionId() {
	return __sync_add_and_fetch(&_max_session_id, 1);
}

void TL_Server::setTaskThreadNum(int num) {
	if (num > 0)
		_task_thread_num = num;
}

void TL_Server::run() {
	//if(_task_threads)
	if (_task_thread_num < 0) {
		_task_thread_num = 1;
	}

	for (int i = 0; i < _task_thread_num; ++i) {
		TL_ExecuteThread * task = new TL_ExecuteThread();
		task->setServer(this);
		tidp::TL_ThreadControl tctl = task->start();
		tctl.detach();
		_task_threads.insert(task);
	}
	_running = 1;
	while (_running) {
		int ret = epoll_wait(_epfd, _events, MAX_EPOLL_EVENTS_SIZE, -1);
		if (ret < 0) {
			if (errno == EINTR) {
				continue;
			} else {
				throw TL_Exception("epoll_wait error:", errno);
			}
		}
		for (int i = 0; i < ret; ++i) {
			if ((_events[i].events & EPOLLIN)) {
				if (_events[i].data.fd == _sockfd) {
					int client_sock = -1;
					struct sockaddr client_addr;
					unsigned int socklen = sizeof(struct sockaddr);
					client_sock = accept(_sockfd, &client_addr, &socklen);
					if (client_sock < 0) {
						throw TL_Exception("accept error:", errno);
					}
					TL_Common::setNonBlock(client_sock);
					struct sockaddr_in *p = (struct sockaddr_in*) &client_addr;
					char client_addr_in[INET_ADDRSTRLEN] = { '\0' };
					inet_ntop(PF_INET, &p->sin_addr, client_addr_in, sizeof(client_addr_in));
					int client_port = ntohs(p->sin_port);
					TL_Session2Ptr pt = TL_Session2Ptr(
							TL_Session2::createSession(client_sock, client_port, client_addr_in));
					pt->setSessionId(getSessionId());
					addSession(pt);
				} else {
					const TL_Session2Ptr& sessionPtr = *(TL_Session2Ptr*) _events[i].data.ptr;
					int ret = sessionPtr->read();
					if (ret < 0 || (ret == 0 && errno != EAGAIN)) {
						//cout << "read:" << ret << endl;
						epoll_ctl(_epfd, EPOLL_CTL_DEL, sessionPtr->getFd(), NULL);
						removeSession(sessionPtr);
						//sessionPtr->unregister();
					}
				}
			}
			if (_events[i].events & EPOLLERR) {
				cout << "EPOLLERR:" << endl;
				const TL_Session2Ptr& sessionPtr = *(TL_Session2Ptr*) _events[i].data.ptr;
				removeSession(sessionPtr);
				//sessionPtr->unregister();
			}
		}
	}
	//ev_run(_loop, 0);
}
/*
 void TL_Server::acceptCB(EV_P_ ev_io *w, int revents) {
 int client_sock = -1;
 struct sockaddr client_addr;
 unsigned int socklen = sizeof(struct sockaddr);
 client_sock = accept(w->fd, &client_addr, &socklen);
 TL_Common::setNonBlock(client_sock);
 struct sockaddr_in *p = (struct sockaddr_in*) &client_addr;
 char client_addr_in[INET_ADDRSTRLEN] = { '\0' };
 inet_ntop(PF_INET, &p->sin_addr, client_addr_in, sizeof(client_addr_in));
 int client_port = ntohs(p->sin_port);

 TL_Server * server = (TL_Server *) w->data;
 if (client_sock > 0) {
 server->addSession(TL_Session2Ptr(TL_Session2::createSession(client_sock, client_port, client_addr_in)));
 }
 }
 void TL_Server::readCB(EV_P_ ev_io *w, int revents) {
 TL_Session2Ptr & session = *(TL_Session2Ptr*) (w->data);
 int ret = -1;
 try {
 ret = session->read();
 } catch (...) {

 }
 if (ret == -1 && errno == EAGAIN) {
 return;
 }

 if (ret <= 0) {
 cout << "start ev_io_stop:" << w << endl;
 ev_io_stop(loop, w);
 session->unregister();
 session->close();
 }
 }
 */
void TL_Server::setExecuteFactory(const TL_ExecuteFactoryPtr& factory_ptr) {
	_execute_factory_ptr = factory_ptr;
}
TL_ExecutePtr TL_Server::getExecute(unsigned int command_id) {
	return _execute_factory_ptr->create(command_id);
}
const TL_Session2Ptr& TL_Server::addSession(const TL_Session2Ptr& session) {
	//判断sessionid的唯一性，防止冲突
	unsigned int session_id = session->getSessionId();
	map<unsigned int, TL_Session2Ptr>::const_iterator it = _id_sessions.find(session_id);
	while (it != _id_sessions.end()) {
		session_id = getSessionId();
		it = _id_sessions.find(session_id);
	}
	session->setSessionId(session_id);
	_id_sessions[session_id] = session;
	const TL_Session2Ptr& sessionRef = _id_sessions[session_id];

	struct epoll_event ev;
	ev.data.ptr = (void*) &sessionRef;
	ev.events = EPOLLIN | EPOLLERR;
	int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, sessionRef->getFd(), &ev);
	if (ret < 0) {
		throw TL_Exception("epoll_ctl error:", errno);
	}
	/*
	 struct ev_io * client_io = new ev_io();
	 ev_io_init(client_io, &readCB, session->getFd(), EV_READ);
	 client_io->data = (void*) &sessionRef;
	 ev_io_start(_loop, client_io);
	 */

	//sessionRef->setMyEvIo(client_io);
	sessionRef->setServer(this);
	return sessionRef;
}
void TL_Server::removeSession(const TL_Session2Ptr& session) {
	//cout << "rm Session:" << session->getSessionId() << "|" << session << "|" << _id_sessions.size() << "|"
	//		<< session.use_count() << endl;
	//Lock lk(*this);
	cout << "rm Session:" << _id_sessions.size() << "|uc:" << session.use_count() << "|fd:" << session->getFd()
			<< "|sid:" << session->getSessionId() << "|ascn:" << _id_sessions.size() - 1 << endl;
	_id_sessions.erase(session->getSessionId());
	session->close();
	//cout << "rm Session:" << _id_sessions.size() << "|" << session.use_count() << endl;
	return;
	//TL_Session2Ptr tmp = session;
}

TL_Session2Ptr TL_Server::getSessionPtr(unsigned int session_id) {
	map<unsigned int, TL_Session2Ptr>::const_iterator it = _id_sessions.find(session_id);
	if (it != _id_sessions.end()) {
		return it->second;
	}
	return TL_Session2Ptr();
}
TL_PacketPtr TL_Server::getPacket() {
	//tidp::TL_ThreadDataPool<TL_Packet>::Lock lk(_data_pool);
	TL_PacketPtr sp(_data_pool.get(), boost::bind(&TL_Server::release, this, _1));
	sp->server = this;
	cout << "getPacket:" << sp << endl;
	return sp;
	/*
	tidp::TL_ThreadDataPool<TL_Packet>::Lock lk(_data_pool);
	TL_PacketPtr sp(_data_pool.getNoLock(), boost::bind(&TL_Server::release, this, _1));
	sp->server = this;
	cout << "getPacket:" << sp << endl;
	return sp;
	*/
}

void TL_Server::release(TL_Packet * data) {
	//tidp::TL_ThreadDataPool<TL_Packet>::Lock lk(_data_pool);
	data->clear();
	_data_pool.release(data);
	cout << this << " release data:" << data << " pool size:" << _data_pool.getSize() << "|" << data->recv_data << endl;
	return;
	tidp::TL_ThreadDataPool<TL_Packet>::Lock lk(_data_pool);
	data->clear();
	_data_pool.releaseNoLock(data);
	cout << this << " release data:" << data << " pool size:" << _data_pool.getSize() << "|" << data->recv_data << endl;
}
/*
 * 完整请求包放到池中
 * */
void TL_Server::addRequest(const TL_PacketPtr& request) {
	tidp::TL_ThreadLock::Lock lk(_recv_lock);
	_recv_pool.insert(request);
	//通知丢失的情况需要考虑。
	_recv_lock.notifyImpl(1);
}
void TL_Server::getRequest(TL_PacketPtr& request) {
	tidp::TL_ThreadLock::Lock lk(_recv_lock);
	volatile size_t size = _recv_pool.size();
	while (size == 0) {
		lk.wait();
		size = _recv_pool.size();
	}
	set<TL_PacketPtr>::iterator it = _recv_pool.begin();
	request = *it;
	_recv_pool.erase(it);
}
void TL_Server::sendPacket(const TL_PacketPtr& response) {
	addResponse(response);
}
void TL_Server::addResponse(const TL_PacketPtr& response) {
	tidp::TL_ThreadLock::Lock lk(_send_lock);
	_send_pool.insert(response);
}

void TL_Server::setErrorMessage(const string& errmsg) {
	_errmsg = errmsg;
}
const string& TL_Server::getErrorMessage() {
	return _errmsg;
}
void TL_Server::getErrorMessage(string& errmsg) {
	errmsg = _errmsg;
}
}
