/*
 * File:   TL_Session.cpp
 * Author: everpan
 *
 * Created on 2011年6月24日, 上午8:53
 */

#include <map>

#include "TL_Session.h"
namespace tidp {

/**
 * defaultPacketParse 数据包处理方法，默认echo
 * @param session
 * @return
 */
static int defaultPacketParse(TL_Session& session) {
	std::string& recv = session.getRecvBufferRef();
	//echo
	session.getSocket().send(recv.c_str(), recv.size());
	return 0;
}

static int defaultAuthentication(TL_Session& session) {
	char recv[512];
	TL_Socket& sock = session.getSocket();
	int ret = sock.recv(recv, 512);
	if (ret < 0) {
		LOG() << debug << "recv from " << session.getIp() << ":" << session.getPort() << " return " << ret << endl;
		sock.close();
	} else {
		LOG() << debug << "defaultAuthentication recv from " << ret << " size data." << endl;
	}
	return 0;
}

TL_Session::TL_Session() {
	_port = 0;
	_sequenceid = 0;
	_session_type = -1;
	_fn = defaultPacketParse;
	_af = defaultAuthentication;
	_last_uptime = 0;
	_sessionid = 0;
	_connect_timeat = 0;
}

TL_Session::~TL_Session() {

}

int TL_Session::getfd() const {
	return _socket.getfd();
}

void TL_Session::read() {
	int len = 0;
	while (1) {
		len = _socket.recv(_buffer, sizeof(_buffer));
		if (len < 0) {
			int eno = errno;
			if (eno == EAGAIN || eno == EWOULDBLOCK) {
				break;
			} else {
				_socket.close();
				LOG() << debug << "recv:" << _ip << ":" << _port << ":" << eno << ":" << strerror(eno) << endl;
				break;
			}
		} else if (len == 0) {
			//closed by client
			//int eno = errno;
			LOG() << debug << "recv:" << _ip << ":" << _port << ":peer has  performed an orderly shutdown" << endl;
			_socket.close();
			return;
		} else if (len == sizeof(_buffer)) {
			_recvbuffer.append(_buffer, len);
			len = 0;
		} else {
			// 0 < len < sizeof (_buffer)
			//_recvbuffer.append(_buffer, len);
			break;
		}
	}
	if (len > 0) {
		_recvbuffer.append(_buffer, len);
	}
	_fn(*this);
}

bool TL_Session::authentication() {
	return _af(*this) == 0;
	//return true;
}

unsigned int TL_Session::getSessionId() {
	return _sessionid;
}

void TL_Session::initIpPort() {
	//_socket.get
	if (_socket.getDomain() == AF_INET) {
		_socket.getPeerName(_ip, _port);
	} else {
		_socket.getPeerName(_ip);
		_port = 0;
	}
	_connect_timeat = time(0);
}

std::string TL_Session::getIp() {
	return _ip;
}

int TL_Session::getPort() {
	return _port;
}

void TL_Session::close() {
	//_sessionid = 0;
	_socket.close();
}

void TL_Session::setProtocolFun(Fn fn) {
	_fn = fn;
}

void TL_Session::setAuthenFun(Fn fn) {
	_af = fn;
}

Fn TL_Session::getProtocolFun() {
	return _fn;
}

Fn TL_Session::getAuthenFun() {
	return _af;
}

TL_Socket& TL_Session::getSocket() {
	return _socket;
}

std::string& TL_Session::getRecvBufferRef() {
	return _recvbuffer;
}

unsigned int TL_Session::getSequence() {
	return _sequenceid;
}

unsigned int TL_Session::incSequence() {
	return ++_sequenceid;
}

void TL_Session::setUptime(time_t t) {
	_last_uptime = t;
}

time_t TL_Session::getUptime() {
	return _last_uptime;
}

time_t TL_Session::getConnTimeAt() {
	return _connect_timeat;
}

void TL_Session::setSessionType(int type) {
	_session_type = type;
}

int TL_Session::getSessionType() {
	return _session_type;
}
//----------------------------------------------------

TL_SessionMgr::TL_SessionMgr() {
	_sessions.clear();
	_ipsessions.clear();
	_authen_session.clear();
	//_auto_increment_sessionid = 0;
	//_min_sessionid = 1;
	_auto_inc_id = 1;
	_last_update_time = 0;
}

TL_SessionMgr::~TL_SessionMgr() {
	clear();
}

/**
 * registSession 注册session 确立sessionid. 对于同一个socket的注册会返回之前的sessionid
 * @param session
 */

void TL_SessionMgr::registSession(TL_Session* session) {
	// TL_ThreadLock::Lock lk(*this);
	registSessionByNolock(session);
}

void TL_SessionMgr::registSessionByNolock(TL_Session* session) {
	std::map<unsigned int, TL_Session*>::iterator it = _sessions.begin();
	bool hassocket = false;
	//遍历查找 防止一个soketid 多次注册.
	while (it != _sessions.end()) {
		if (it->second->getfd() == session->getfd()) {
			session->_sessionid = it->second->_sessionid;
			//hassocket = true;
			return;
		}
		++it;
	}

	if (!hassocket) {
		/*
		 do {
		 ++_auto_increment_sessionid;
		 // 0  保留 空session处理
		 if (_auto_increment_sessionid == 0) _auto_increment_sessionid = 1;
		 it = _sessions.find(_auto_increment_sessionid);
		 } while (it != _sessions.end()
		 // protecte against loop while
		 && _sessions.size() < (unsigned int) (-1));
		 session->_sessionid = _auto_increment_sessionid;
		 _sessions[_auto_increment_sessionid] = session;
		 _ipsessions[session->_ip].push_back(session);
		 */
		do {
			++_auto_inc_id;
			it = _sessions.find(_auto_inc_id);
		} while (it != _sessions.end());
		/*
		 while (it != _sessions.end() && _sessions.size() < (unsigned int) (-1)) {
		 ++_min_sessionid;
		 if (_min_sessionid == 0)
		 _min_sessionid = 1;
		 it = _sessions.find(_min_sessionid);
		 }

		 session->_sessionid = _min_sessionid;
		 _sessions[_min_sessionid] = session;
		 _ipsessions[session->_ip].push_back(session);
		 */
		session->_sessionid = _auto_inc_id;
		_sessions[_auto_inc_id] = session;
		_ipsessions[session->_ip].push_back(session);
	}
}

/**
 * 对临时会话或者未认证对话进行管理,超时退出
 * @param session
 */
void TL_SessionMgr::addAuthenSession(TL_Session* session) {
	//TL_ThreadLock::Lock lk(*this);
	std::set<TL_Session*>::iterator it = _authen_session.find(session);
	if (_authen_session.end() == it) {
		_authen_session.insert(session);
	} else {
		throw TL_Exp("TL_SessionMgr::addAuthenSession the session is in authen queue.");
	}
}

/**
 * removeSession 从mgr中移除session,session空间由用户承担释放.
 * @param session
 */
void TL_SessionMgr::unregistSession(TL_Session* session, bool isdestory) {
	//TL_ThreadLock::Lock lk(*this);
	std::map<unsigned int, TL_Session*>::iterator it;
	//TL_Session* sessiontmp;
	if (session->_sessionid > 0) {
		if ((it = _sessions.find(session->_sessionid)) != _sessions.end()) {
			//session = it->second;
			_sessions.erase(it);
			_ipsessions[session->_ip].remove(session);
		}
	} /*
	 else {

	 it = _sessions.begin();
	 while (it != _sessions.end()) {
	 if (it->second->getfd() == session->getfd()) {
	 //find the socket
	 TL_Session* sessiontmp = it->second;
	 std::string ip = it->second->_ip;
	 _sessions.erase(it++);
	 _ipsessions[ip].remove(sessiontmp);
	 if (sessiontmp != session) {
	 delete sessiontmp;
	 }
	 break;
	 }
	 ++it;
	 }
	 }*/
	/** 从tmp里delete*/

	if (isdestory) {
		/*
		 if (_min_sessionid > session->_sessionid) {
		 _min_sessionid = session->_sessionid;
		 }
		 */
		delete session;
		session = NULL;
	}
}

/**
 * destorySession
 * 删除注册session 并自动销毁
 * @param session
 */
void TL_SessionMgr::destorySession(TL_Session* session) {
	unregistSession(session, true);
}

/**
 * clearInvalidSession
 * 清理非法的session
 */
void TL_SessionMgr::clearInvalidSession() {
	// TL_ThreadLock::Lock lk(*this);
	std::map<unsigned int, TL_Session*>::iterator it = _sessions.begin();
	std::string ip;
	TL_Session * session;
	while (it != _sessions.end()) {
		if (!(it->second->_socket.isValid())) {
			//find the invalid socket

			ip = it->second->_ip;
			session = it->second;
			/*
			 if (_min_sessionid > session->_sessionid) {
			 _min_sessionid = session->_sessionid;
			 }*/
			LOG() << debug << "TL_SessionMgr::clearInvalidSession:session[" << session->getSessionId() << "] "
					<< session->getSessionType() << "|" << ip << ":" << session->getPort() << " is closeed." << endl;
			_ipsessions[ip].remove(session);
			_sessions.erase(it++);
			delete session;
			//continue;
		} else {
			++it;
		}
	}

	std::set<TL_Session*>::iterator it2 = _authen_session.begin();
	while (it2 != _authen_session.end()) {
		if (!(*it2)->_socket.isValid()) {
			delete (*it2);
			_authen_session.erase(it2++);
			continue;
		}
		++it2;
	}
}

void TL_SessionMgr::clear() {
	// TL_ThreadLock::Lock lk(*this);
	std::map<unsigned int, TL_Session*>::iterator it = _sessions.begin();
	while (it != _sessions.end()) {
		/*TL_Session* session = it->second;
		 if(session->_socket.isValid()){
		 session->_socket.close();
		 }*/
		delete it->second;
		++it;
	}
	_sessions.clear();
	_ipsessions.clear();
}
void TL_SessionMgr::close(int type) {
	std::map<unsigned int, TL_Session*>::iterator it = _sessions.begin();
	while (it != _sessions.end()) {
		if (it->second->getSessionType() == type) {
			it->second->close();
		}
		++it;
	}
	clearInvalidSession();
}

void TL_SessionMgr::closeTimeout(time_t uptime) {
	std::map<unsigned int, TL_Session*>::iterator it = _sessions.begin();
	while (it != _sessions.end()) {
		if (it->second->getUptime() > uptime) {
			LOG() << debug << "session[" << it->second->getSessionId() << "] timeout." << it->second->getIp() << ":"
					<< it->second->getPort() << endl;
			it->second->close();
		}
		++it;
	}
	clearInvalidSession();
}

/**
 * selectMode 初始select模型的socket集
 * @param rfds
 * @param wfds
 * @param efds
 * @return 最大max_fd
 */
int TL_SessionMgr::selectMode(fd_set* rfds, fd_set*wfds, fd_set* efds) {
	// TL_ThreadLock::Lock lk(*this);
	std::map<unsigned int, TL_Session*>::iterator it = _sessions.begin();
	int max_fd = 0;
	int fdtmp = 0;

	while (it != _sessions.end()) {
		if (it->second->_socket.isValid()) {
			fdtmp = it->second->getfd();
			if (rfds != NULL) {
				FD_SET(fdtmp, rfds);
			}
			if (wfds != NULL) {
				FD_SET(fdtmp, wfds);
			}
			if (efds != NULL) {
				FD_SET(fdtmp, efds);
			}
			max_fd = max_fd > fdtmp ? max_fd : fdtmp;
		}
		++it;
	}

	std::set<TL_Session*>::iterator it_tmp = _authen_session.begin();
	while (it_tmp != _authen_session.end()) {
		fdtmp = (*it_tmp)->getfd();
		if (rfds != NULL) {
			FD_SET(fdtmp, rfds);
		}
		if (wfds != NULL) {
			FD_SET(fdtmp, wfds);
		}
		if (efds != NULL) {
			FD_SET(fdtmp, efds);
		}
		max_fd = max_fd > fdtmp ? max_fd : fdtmp;
		++it_tmp;
	}

	return max_fd;
}

/**
 * selectRead select 模型下的数据处理过程,包含数据的协议解析 可以异步处理,同步处理要注意时延
 * @param rfds
 */
void TL_SessionMgr::selectRead(fd_set* rfds) {
	// TL_ThreadLock::Lock lk(*this);
	std::map<unsigned int, TL_Session*>::iterator it = _sessions.begin();
	int fdtmp = 0;
	while (it != _sessions.end()) {
		fdtmp = it->second->getfd();
		//read event
		if (fdtmp > 0 && FD_ISSET(fdtmp, rfds)) {
			it->second->read();
			it->second->setUptime(_last_update_time);
		}
		++it;
	}

	std::set<TL_Session*>::iterator it_tmp = _authen_session.begin();
	while (it_tmp != _authen_session.end()) {
		fdtmp = (*it_tmp)->getfd();
		if (fdtmp > 0 && FD_ISSET(fdtmp, rfds)) {
			TL_Session* session = *it_tmp;
			if (!(session->authentication())) {
				//认证不成功
				session->_socket.close();
				delete session;
				_authen_session.erase(it_tmp++);
				continue;
			} else {
				//认证成功
				_authen_session.erase(it_tmp++);
				//registSession(session);
				registSessionByNolock(session);
				continue;
			}
		}
		++it_tmp;
	}
}

void TL_SessionMgr::selectError(fd_set* efds) {
	// TL_ThreadLock::Lock lk(*this);
	std::map<unsigned int, TL_Session*>::iterator it = _sessions.begin();
	int fdtmp = 0;
	while (it != _sessions.end()) {
		fdtmp = it->second->getfd();
		//read event
		if (fdtmp > 0 && FD_ISSET(fdtmp, efds)) {
			it->second->_socket.close();
		}
		++it;
	}
}

int TL_SessionMgr::send(unsigned int sessionid, const std::string& data) {
	return send(sessionid, data.c_str(), data.size());
}

int TL_SessionMgr::send(unsigned int sessionid, const char * data, int len) {
	//TL_ThreadLock::Lock lk(*this);
	std::map<unsigned int, TL_Session*>::iterator it = _sessions.find(sessionid);
	if (it == _sessions.end()) {
		//session invalid
		return -1;
	}
	TL_Socket &sock = it->second->_socket;
	int slen = 0;
	int ret = 0;
	do {
		ret = sock.send(data + slen, len - slen);
		if (ret < 0) {
			int eno = errno;
			if (eno == EAGAIN or eno == EWOULDBLOCK) {
				if (slen < len)
					continue;
			}
			sock.close();
			return ret;
		}
		slen += ret;
	} while (slen < len);
	return slen;
}

int TL_SessionMgr::sendByLock(unsigned int sessionid, const char * data, int len) {
	// TL_ThreadLock::Lock lk(*this);
	return send(sessionid, data, len);
}

bool TL_SessionMgr::hasSessionByIp(const std::string& ip) {
	// TL_ThreadLock::Lock lk(*this);
	//return _ipsessions.find(ip) != _ipsessions.end();
	std::map<std::string, std::list<TL_Session*> >::iterator it = _ipsessions.find(ip);
	if (it == _ipsessions.end()) {
		return false;
	} else {
		return it->second.size() > 0;
	}
}

unsigned int TL_SessionMgr::getSessionIdByIp(const std::string& ip, int itype) {
	std::map<std::string, std::list<TL_Session*> >::iterator it = _ipsessions.find(ip);
	if (it != _ipsessions.end()) {
		std::list<TL_Session*>& slist = it->second;
		std::list<TL_Session*>::iterator it2 = slist.begin();
		TL_Session* session;
		while (it2 != slist.end()) {
			session = *it2;
			if (itype == -1) { //不关心类型
				return session->_sessionid;
			} else {
				if (session->_session_type == itype) {
					return session->_sessionid;
				}
			}
			++it2;
		}
	}
	return 0;
}

/**
 * getSessionIdByIp
 * @param ip
 * @param slist 返回ip对应的session，slist中的session随时可能失效，需要多TL_SessionMgr锁定再使用。
 * @return
 */
bool TL_SessionMgr::getSessionIdByIp(const std::string& ip, std::list<TL_Session*>& slist) {
	std::map<std::string, std::list<TL_Session*> >::iterator it = _ipsessions.find(ip);
	if (it != _ipsessions.end()) {
		slist = it->second;
		return true;
	}
	return false;
}

TL_Session* TL_SessionMgr::getSessionById(const unsigned int sessionid) {
	std::map<unsigned int, TL_Session*>::iterator it = _sessions.find(sessionid);
	if (it != _sessions.end()) {
		return it->second;
	}
	return NULL;
}
}
