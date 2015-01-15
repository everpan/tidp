/*
 * TL_Session.h
 *
 *  Created on: Dec 27, 2014
 *      Author: ever
 */

#ifndef INCLUDE_NET_TL_SESSION_H_
#define INCLUDE_NET_TL_SESSION_H_
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <errno.h>

#include <string>
#include <net/TL_Socket.h>
#include <TL_ThreadLock.h>
namespace tidp {
namespace net {

#include <errno.h>

class TL_Session :public TL_ThreadLock {
public:
	TL_Session();
	virtual ~TL_Session();
	int read(char * buff, int len);
	int send(const char * buff, int len);
	int send(const std::string& str);
	int getErrno();
	void close();
	bool isValid();
	void clearRecvBuffer();
	unsigned int getId();
	void setId(unsigned int sid);
	//port == -1 local
	//void setHostIp(const std::string& ip, int port);
	const std::string& getHost();
	int getPort();
	TL_Socket& getSocket();

	std::string& getRecvBuffer();
	void addRecvBuffer(const char* buff,int len);
protected:
	TL_Session(const TL_Session&);
	TL_Session& operator=(const TL_Session&);
private:
	//int _socket;
	int _errno;
	unsigned int _session_id;
	//int _port;

	//std::string _ip;
	TL_Socket _sock;
	std::string _recv_buff;
};
} /* namespace net */
} /* namespace tidp */

#endif /* INCLUDE_NET_TL_SESSION_H_ */
