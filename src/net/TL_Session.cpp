/*
 * TL_Session.cpp
 *
 *  Created on: Dec 27, 2014
 *      Author: ever
 */

#include <net/TL_Session.h>
#include <net/TL_Socket.h>
namespace tidp {
namespace net {
TL_Session::TL_Session() {
	_errno = 0;
	_session_id = 0;
}


TL_Session::~TL_Session() {
}
int TL_Session::read(char * buff, int len) {
	return ::read(_sock.getfd(),buff,len);
}
int TL_Session::send(const char * buff, int len) {
	if(!_sock.isValid()){
		ostringstream os;
		os << "sid:" << _session_id << " sock is invalid";
		throw TL_Exception(os.str(),-1);
	}
	int x = 0;
	int y = 0;
	while (x < len) {
		y = ::write(_sock.getfd(), buff + x, len - x);
		if(y == -1 && errno == EAGAIN){
			continue;
		}else if (y < 0) {
			ostringstream os;
			os << "send to sid:"<< _session_id << " sock:" << _sock.getfd() << " failed";
			throw TL_Exception(os.str(),errno);
			//return y;
		}
		x += y;
	}
	return x;
}
int TL_Session::send(const std::string& str){
	return send(str.c_str(),str.size());
}
int TL_Session::getErrno() {
	return _errno;
}

void TL_Session::close() {
	_sock.close();
	_errno = 0;
	_session_id = 0;
	_recv_buff.clear();
}

bool TL_Session::isValid() {
	return _sock.isValid();
}
void TL_Session::clearRecvBuffer(){
	_recv_buff.clear();
}
unsigned int TL_Session::getId() {
	return _session_id;
}
void TL_Session::setId(unsigned int sid) {
	_session_id = sid;
}

const std::string& TL_Session::getHost() {
	return _sock.getHost();
}
int TL_Session::getPort() {
	return _sock.getPort();
}
TL_Socket& TL_Session::getSocket(){
	return _sock;
}

std::string& TL_Session::getRecvBuffer(){
	return _recv_buff;
}
void TL_Session::addRecvBuffer(const char* buff,int len){
	_recv_buff.append(buff,len);
	//todo parse
}

} /* namespace net */
} /* namespace tidp */
