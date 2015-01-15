/*
 * TL_Server.cpp
 *
 *  Created on: Dec 27, 2014
 *      Author: ever
 *
 *      改版 进一步解耦合
 */

#include <net/TL_Server.h>
#define LISTEN_BACKLOG 128
#define EPOLL_EVENT_WAIT_SIZE 512
#define SERVER_RECV_BUFFER_DEFAULT_SIZE 4096
namespace tidp {
namespace net {
/*
 * 数据包解析函数 约定，如果产生新包，则返回  > 0 并且每次只能返回一个。
 * 如果未产生新的数据包，则返回0，也表示需要更多的数据。
 * */
static int parse(string& recv_data, TL_PacketPtr& pack) {
	string::size_type pos = recv_data.find('\n');
	if (pos != string::npos) {
		if (pack) {
			pack->recv_data.assign(recv_data.c_str(), pos + 1);
			//pack->session->send(recv_data.c_str(), pos + 1);
			pack->cmdid = 1;
		}
		recv_data.erase(0, pos + 1);
		return 1;
	}
	return 0;
}

TL_Server::TL_Server() : _execute_thread_factory(new TL_ExecuteThreadFactory) {
	_thread_num = 1;
	_epfd = epoll_create(10);
	_session_auto_increment_id = 0;
	_recv_buff = (char *) malloc(SERVER_RECV_BUFFER_DEFAULT_SIZE);
	_events = new struct epoll_event[EPOLL_EVENT_WAIT_SIZE];
	_parse_fun = parse;
}

TL_Server::~TL_Server() {
}

void TL_Server::bind(const char *sServerAddr, int port) {
	if (!_sock.isValid()) {
		_sock.createSocket();
	}

	_sock.bind(sServerAddr, port);
	_sock.listen(LISTEN_BACKLOG);
	_sock.setblock(false);

	struct epoll_event ev;
	ev.data.ptr = &_sock;
	ev.events = EPOLLIN;
	int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, _sock.getfd(), &ev);
	LOG() << "bind to " << sServerAddr << ":" << port << " ret:" << ret << endl;
	if (ret != 0) {
		throw TL_Exception("epoll_ctl error:", -1);
	}

}
void TL_Server::bind(const char *sPathName) {
	if (!_sock.isValid()) {
		_sock.createSocket();
	}

	_sock.bind(sPathName);
	_sock.listen(LISTEN_BACKLOG);
	_sock.setblock(false);

	struct epoll_event ev;
	ev.data.ptr = &_sock;
	ev.events = EPOLLIN;
	int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, _sock.getfd(), &ev);
	LOG() << debug << "bind to " << sPathName << ": ok!" << endl;
	if (ret != 0) {
		throw TL_Exception("epoll_ctl error:", -1);
	}
}
unsigned int TL_Server::getSessionAutoIncrementId() {
	return __sync_add_and_fetch(&_session_auto_increment_id, 1);
}
void TL_Server::run() {
	startExecuetThread();
	TL_PacketPtr packet;
	_running = 1;
	while (_running) {
		int event_size = epoll_wait(_epfd, _events, EPOLL_EVENT_WAIT_SIZE, 100);
		if (event_size < 0) {
			if (errno == EINTR) {
				_session_manger.autoReleaseInvalidSession();
				continue;
			} else {
				throw TL_Exception("epoll_wait error:", errno);
			}
		}
		if(event_size == 0){
			_session_manger.autoReleaseInvalidSession();
			continue;
		}
		//LOG() << debug << "event size:" << event_size << endl;
		for (int i = 0; i < event_size; ++i) {
			if (_events[i].events & EPOLLERR) {
				if (_events[i].data.ptr == &_sock) {
					LOG() << debug << "server sock is error." << endl;
				} else {
					TL_SessionPtr& session = *(TL_SessionPtr*) _events[i].data.ptr;
					TL_Socket& sock = session->getSocket();
					LOG() << debug << "epollerr from " << sock.getHost() << ":" << sock.getPort()
							<< " need to close and rm session:" << session.get() << " sid:" << session->getId() << endl;
					epoll_ctl(_epfd, EPOLL_CTL_DEL, session->getSocket().getfd(), NULL);
					sock.close();
					_session_manger.release(session);
				}
			} else if (_events[i].events & EPOLLIN) {
				//server
				if (_events[i].data.ptr == &_sock) {
					TL_SessionPtr& session = _session_manger.getIdleSession();
					struct sockaddr pstSockAddr;
					bzero(&pstSockAddr, sizeof(struct sockaddr));
					socklen_t len = sizeof(struct sockaddr);
					TL_Socket& sock = session->getSocket();
					//LOG() << "accept before " << _sock.getfd() << endl;
					int accept_fd = _sock.accept(sock, &pstSockAddr, len);
					if (accept_fd < 0) {
						throw TL_Exception("accept error", errno);
					}
					session->setId(getSessionAutoIncrementId());
					//cout << "accept new session " << accept_fd << std::endl;
					sock.setblock(false);
					struct epoll_event ev;
					ev.data.ptr = &session;
					ev.events = EPOLLIN;
					int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, sock.getfd(), &ev);

					LOG() << debug << "accept ok : " << accept_fd << ",from " << sock.getHost() << ":"
							<< sock.getPort() << " new session:" << session.get() << " sid:" << session->getId()
							<< endl;
					if (ret != 0) {
						throw TL_Exception("epoll_ctl error:", errno);
					}
				} else {
					TL_SessionPtr& session = *(TL_SessionPtr*) _events[i].data.ptr;
					bool isRecved = false;
					do {
						//session read data
						int x = session->read(_recv_buff, SERVER_RECV_BUFFER_DEFAULT_SIZE);
						if (x == -1 && errno == EAGAIN) {
							break;
						} else if (x > 0) {
							session->addRecvBuffer(_recv_buff, x);
							isRecved = true;
						} else {
							TL_Socket& sock = session->getSocket();
							LOG() << debug << "recv " << x << " from " << sock.getfd() << "," << sock.getHost()
									<< ":" << sock.getPort() << " need to close and rm session:" << session.get()
									<< " sid:" << session->getId() << endl;
							epoll_ctl(_epfd, EPOLL_CTL_DEL, sock.getfd(), NULL);
							_session_manger.release(session);
							break;
						}
					} while (1);
					if (isRecved) {
						//session parse data
						int ret = 0;
						do {
							if(!packet){
								//不必每次都去获取，减少锁等开销
								packet = _packet_manger.getPacketPtr();
							}
							if(!packet->session){
								//不能长久持有session 可能session过期，需要的时候再获取
								packet->session = session;
								packet->sid = session->getId();
							}
							ret = _parse_fun(session->getRecvBuffer(), packet);
							if (ret != 0) {
								_request_queue.push_back(packet);
								packet.reset();
								continue;
							}
						} while (0);
						if(packet && packet->session){
							packet->session.reset();
							packet->sid = 0;
						}
					}
				}
			} //end epollin
		} //end for events check
	} //end while
}
void TL_Server::setExecuteNumber(int num) {
	if (num < 0) {
		_thread_num = 1;
	} else {
		_thread_num = num;
	}
}
void TL_Server::startExecuetThread() {
	for (int i = 0; i < _thread_num; ++i) {
		TL_ExecuteThreadPtr ptr = _execute_thread_factory->create();

		_execute_threads.push_back(ptr);
		ptr->setServer(this);
		ptr->start();
	}
}
void TL_Server::setExecuteThreadFactory(TL_ExecuteThreadFactoryPtr& factory){
	_execute_thread_factory = factory;
}
TL_ExecuteThreadFactoryPtr TL_Server::getExecuteThreadFactory(){
	return _execute_thread_factory;
}
void TL_Server::setParseFun(const ParseFun& fun) {
	_parse_fun = fun;
}

bool TL_Server::getRequest(TL_PacketPtr& pt) {
	return _request_queue.pop_front(pt);
}

} /* namespace net */
} /* namespace tidp */
