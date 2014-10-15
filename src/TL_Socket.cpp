/* 
 * File:   TL_Socket.cpp
 * Author: everpan
 * 
 * Created on 2011年3月15日, 上午11:35
 */

#include <iostream>

#include "TL_Socket.h"
#ifdef __CYGWIN__
//http://www.cygwin.com/ml/cygwin/2004-04/msg00532.html
int gethostbyname_r(const char *name,
		struct hostent *ret,
		char *buf,
		size_t buflen,
		struct hostent **result,
		int *h_errnop)
{

	int hsave;
	struct hostent *ph;
	static pthread_mutex_t __mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&__mutex); /* begin critical area */
	hsave = h_errno;
	ph = gethostbyname(name);
	*h_errnop = h_errno; /* copy h_errno to *h_herrnop */
	if (ph == NULL)
	{
		*result = NULL;
	} else
	{
		char **p, **q;
		char *pbuf;
		int nbytes = 0;
		int naddr = 0, naliases = 0;
		/* determine if we have enough space in buf */

		/* count how many addresses */
		for (p = ph->h_addr_list; *p != 0; p++)
		{
			nbytes += ph->h_length; /* addresses */
			nbytes += sizeof (*p); /* pointers */
			naddr++;
		}
		nbytes += sizeof (*p); /* one more for the terminating NULL */

		/* count how many aliases, and total length of strings */

		for (p = ph->h_aliases; *p != 0; p++)
		{
			nbytes += (strlen(*p) + 1); /* aliases */
			nbytes += sizeof (*p); /* pointers */
			naliases++;
		}
		nbytes += sizeof (*p); /* one more for the terminating NULL */

		/* here nbytes is the number of bytes required in buffer */
		/* as a terminator must be there, the minimum value is ph->h_length */
		if (nbytes > (int)buflen)
		{
			*result = NULL;
			pthread_mutex_unlock(&__mutex); /* end critical area */
			return ERANGE; /* not enough space in buf!! */
		}

		/* There is enough space. Now we need to do a deep copy! */
		/* Allocation in buffer:
		 from [0] to [(naddr-1) * sizeof(*p)]:
		 pointers to addresses
		 at [naddr * sizeof(*p)]:
		 NULL
		 from [(naddr+1) * sizeof(*p)] to [(naddr+naliases) * sizeof(*p)] :
		 pointers to aliases
		 at [(naddr+naliases+1) * sizeof(*p)]:
		 NULL
		 then naddr addresses (fixed length), and naliases aliases (asciiz).
		 */

		*ret = *ph; /* copy whole structure (not its address!) */

		/* copy addresses */
		q = (char **) buf; /* pointer to pointers area (type: char **) */
		ret->h_addr_list = q; /* update pointer to address list */
		pbuf = buf + ((naddr + naliases + 2) * sizeof (*p)); /* skip that area */
		for (p = ph->h_addr_list; *p != 0; p++)
		{
			memcpy(pbuf, *p, ph->h_length); /* copy address bytes */
			*q++ = pbuf; /* the pointer is the one inside buf... */
			pbuf += ph->h_length; /* advance pbuf */
		}
		*q++ = NULL; /* address list terminator */

		/* copy aliases */

		ret->h_aliases = q; /* update pointer to aliases list */
		for (p = ph->h_aliases; *p != 0; p++)
		{
			strcpy(pbuf, *p); /* copy alias strings */
			*q++ = pbuf; /* the pointer is the one inside buf... */
			pbuf += strlen(*p); /* advance pbuf */
			*pbuf++ = 0; /* string terminator */
		}
		*q++ = NULL; /* terminator */

		strcpy(pbuf, ph->h_name); /* copy alias strings */
		ret->h_name = pbuf;
		pbuf += strlen(ph->h_name); /* advance pbuf */
		*pbuf++ = 0; /* string terminator */

		*result = ret; /* and let *result point to structure */

	}
	h_errno = hsave; /* restore h_errno */

	pthread_mutex_unlock(&__mutex); /* end critical area */

	return (*result == NULL);

}
#endif

namespace tidp {

TL_Socket::TL_Socket() :
		_socket(INVALID_SOCKET), _bautoclose(true), _sock_type(AF_INET) {
}

TL_Socket::~TL_Socket() {
	if (_bautoclose) {
		close();
	}
}

void TL_Socket::init(int fd, bool bautoclose, int sock_type) {
	if (_bautoclose) {
		close();
	}

	_socket = fd;
	_bautoclose = bautoclose;
	_sock_type = sock_type;
}

void TL_Socket::createSocket(int iSocketType, int sock_type) {
	assert(iSocketType == SOCK_STREAM || iSocketType == SOCK_DGRAM);
	close();

	_sock_type = sock_type;
	_socket = socket(sock_type, iSocketType, 0);

	if (_socket < 0) {
		_socket = INVALID_SOCKET;
		throw TL_Socket_Exception("[TL_Socket::createSocket] create socket error! :" + std::string(strerror(errno)));
	}
}

void TL_Socket::getPeerName(std::string &sPeerAddress, uint16_t &iPeerPort) {
	assert(_sock_type == AF_INET);

	struct sockaddr stPeer;
	bzero(&stPeer, sizeof(struct sockaddr));
	socklen_t iPeerLen = sizeof(sockaddr);

	getPeerName(&stPeer, iPeerLen);

	char sAddr[INET_ADDRSTRLEN] = { 0 };
	struct sockaddr_in *p = (struct sockaddr_in *) &stPeer;

	inet_ntop(_sock_type, &p->sin_addr, sAddr, sizeof(sAddr));

	sPeerAddress = sAddr;
	iPeerPort = ntohs(p->sin_port);
}

void TL_Socket::getPeerName(std::string &sPathName) {
	assert(_sock_type == AF_LOCAL);

	struct sockaddr_un stSock;
	bzero(&stSock, sizeof(struct sockaddr_un));
	socklen_t iSockLen = sizeof(stSock);
	getPeerName((struct sockaddr *) &stSock, iSockLen);

	sPathName = stSock.sun_path;
}

void TL_Socket::getPeerName(struct sockaddr *pstPeerAddr, socklen_t &iPeerLen) {
	if (getpeername(_socket, pstPeerAddr, &iPeerLen) < 0) {
		throw TL_Socket_Exception("[TL_Socket::getPeerName] getpeername error.", errno);
	}
}

void TL_Socket::getSockName(std::string &sSockAddress, uint16_t &iSockPort) {
	assert(_sock_type == AF_INET);

	struct sockaddr stSock;
	bzero(&stSock, sizeof(struct sockaddr));
	socklen_t iSockLen = sizeof(sockaddr);

	getSockName(&stSock, iSockLen);

	char sAddr[INET_ADDRSTRLEN] = "\0";
	struct sockaddr_in *p = (struct sockaddr_in *) &stSock;

	inet_ntop(_sock_type, &p->sin_addr, sAddr, sizeof(sAddr));

	sSockAddress = sAddr;
	iSockPort = ntohs(p->sin_port);
}

void TL_Socket::getSockName(std::string &sPathName) {
	assert(_sock_type == AF_LOCAL);

	struct sockaddr_un stSock;
	bzero(&stSock, sizeof(struct sockaddr_un));
	socklen_t iSockLen = sizeof(stSock);
	getSockName((struct sockaddr *) &stSock, iSockLen);

	sPathName = stSock.sun_path;
}

void TL_Socket::getSockName(struct sockaddr *pstSockAddr, socklen_t &iSockLen) {
	if (getsockname(_socket, pstSockAddr, &iSockLen) < 0) {
		throw TL_Socket_Exception("[TL_Socket::getSockName] getsockname error.", errno);
	}
}

int TL_Socket::accept(TL_Socket &tcSock, struct sockaddr *pstSockAddr, socklen_t &iSockLen) {
	//std::cout << tcSock._socket << "|" << INVALID_SOCKET << std::endl;
	assert(tcSock._socket == INVALID_SOCKET);

	int ifd;

	while ((ifd = ::accept(_socket, pstSockAddr, &iSockLen)) < 0 && errno == EINTR)
		;

	tcSock._socket = ifd;
	tcSock._sock_type = _sock_type;

	return tcSock._socket;
}

void TL_Socket::parseAddr(const std::string &sAddr, struct in_addr &stSinAddr) {
	int iRet = inet_pton(AF_INET, sAddr.c_str(), &stSinAddr);
	if (iRet < 0) {
		throw TL_Socket_Exception("[TL_Socket::parseAddr] inet_pton error.", errno);
	} else if (iRet == 0) {
		/*
		 * struct hostent stHostent;
		 struct hostent *pstHostent;
		 char buf[2048] = "\0";
		 */
		//int iError;

		//gethostbyname_r(sAddr.c_str(), &stHostent, buf, sizeof (buf), &pstHostent, &iError);
		struct hostent *pstHostent = gethostbyname(sAddr.c_str()); //, &stHostent, buf, sizeof (buf), &pstHostent, &iError);
		if (pstHostent == NULL) {
			throw TL_Socket_Exception("[TL_Socket::parseAddr] gethostbyname_r error! :" + std::string(hstrerror(errno)));
		} else {
			stSinAddr = *(struct in_addr *) pstHostent->h_addr;
		}
	}
}

void TL_Socket::bind(const std::string &sServerAddr, int port) {
	assert(_sock_type == AF_INET);

	struct sockaddr stBindAddr;
	bzero(&stBindAddr, sizeof(struct sockaddr));

	struct sockaddr_in *p = (struct sockaddr_in *) &stBindAddr;

	p->sin_family = _sock_type;
	p->sin_port = htons(port);

	if (sServerAddr == "") {
		p->sin_addr.s_addr = htonl(INADDR_ANY);
	} else {
		parseAddr(sServerAddr, p->sin_addr);
	}

	try {
		bind(&stBindAddr, sizeof(stBindAddr));
	} catch (...) {
		throw TL_Socket_Exception("[TL_Socket::bind] bind '" + sServerAddr + ":" + "' error.", errno);
	}
}

void TL_Socket::bind(const char *sPathName) {
	assert(_sock_type == AF_LOCAL);

	unlink(sPathName);

	struct sockaddr_un stBindAddr;
	bzero(&stBindAddr, sizeof(struct sockaddr_un));
	stBindAddr.sun_family = _sock_type;
	strncpy(stBindAddr.sun_path, sPathName, sizeof(stBindAddr.sun_path));

	try {
		bind((struct sockaddr *) &stBindAddr, sizeof(stBindAddr));
	} catch (...) {
		throw TL_Socket_Exception("[TL_Socket::bind] bind '" + std::string(sPathName) + "' error.", errno);
	}
}

void TL_Socket::bind(struct sockaddr *pstBindAddr, socklen_t iAddrLen) {
	//如果服务器终止后,服务器可以第二次快速启动而不用等待一段时间
	int iReuseAddr = 1;

	//设置
	setSockOpt(SO_REUSEADDR, (const void *) &iReuseAddr, sizeof(int), SOL_SOCKET);

	if (::bind(_socket, pstBindAddr, iAddrLen) < 0) {
		throw TL_Socket_Exception("[TL_Socket::bind] bind error.", errno);
	}
}

void TL_Socket::close() {
	if (_socket != INVALID_SOCKET) {
		::close(_socket);
		_socket = INVALID_SOCKET;
	}
}

int TL_Socket::connectNoThrow(const std::string &sServerAddr, uint16_t port) {
	assert(_sock_type == AF_INET);

	if (sServerAddr == "") {
		throw TL_Socket_Exception("[TL_Socket::connect] server address is empty!");
	}

	struct sockaddr stServerAddr;
	bzero(&stServerAddr, sizeof(stServerAddr));

	struct sockaddr_in *p = (struct sockaddr_in *) &stServerAddr;

	p->sin_family = _sock_type;
	parseAddr(sServerAddr, p->sin_addr);
	p->sin_port = htons(port);

	return connect(&stServerAddr, sizeof(stServerAddr));
}

int TL_Socket::connectNoThrow(struct sockaddr* addr) {
	assert(_sock_type == AF_INET);

	return connect(addr, sizeof(struct sockaddr));
}

void TL_Socket::connect(const std::string &sServerAddr, uint16_t port) {
	int ret = connectNoThrow(sServerAddr, port);

	if (ret < 0) {
		throw TL_SocketConnect_Exception("[TL_Socket::connect] connect error", errno);
	}
}

void TL_Socket::connect(const char *sPathName) {
	int ret = connectNoThrow(sPathName);
	if (ret < 0) {
		throw TL_SocketConnect_Exception("[TL_Socket::connect] connect error", errno);
	}
}

int TL_Socket::connectNoThrow(const char *sPathName) {
	assert(_sock_type == AF_LOCAL);

	struct sockaddr_un stServerAddr;
	bzero(&stServerAddr, sizeof(struct sockaddr_un));
	stServerAddr.sun_family = _sock_type;
	strncpy(stServerAddr.sun_path, sPathName, sizeof(stServerAddr.sun_path));

	return connect((struct sockaddr *) &stServerAddr, sizeof(stServerAddr));
}

int TL_Socket::connect(struct sockaddr *pstServerAddr, socklen_t serverLen) {
	return ::connect(_socket, pstServerAddr, serverLen);

}

void TL_Socket::listen(int iConnBackLog) {
	if (::listen(_socket, iConnBackLog) < 0) {
		throw TL_Socket_Exception("[TL_Socket::listen] listen error.", errno);
	}
}

int TL_Socket::recv(void *pvBuf, size_t iLen, int iFlag) {
	return ::recv(_socket, pvBuf, iLen, iFlag);
}

int TL_Socket::send(const void *pvBuf, size_t iLen, int iFlag) {
	return ::send(_socket, pvBuf, iLen, iFlag);
}

int TL_Socket::recvfrom(void *pvBuf, size_t iLen, std::string &sFromAddr, uint16_t &iFromPort, int iFlags) {
	struct sockaddr stFromAddr;
	socklen_t iFromLen = sizeof(struct sockaddr);
	struct sockaddr_in *p = (struct sockaddr_in *) &stFromAddr;

	bzero(&stFromAddr, sizeof(struct sockaddr));

	int iBytes = recvfrom(pvBuf, iLen, &stFromAddr, iFromLen, iFlags);
	if (iBytes >= 0) {
		char sAddr[INET_ADDRSTRLEN] = "\0";

		inet_ntop(_sock_type, &p->sin_addr, sAddr, sizeof(sAddr));

		sFromAddr = sAddr;
		iFromPort = ntohs(p->sin_port);
	}

	return iBytes;
}

int TL_Socket::recvfrom(void *pvBuf, size_t iLen, struct sockaddr *pstFromAddr, socklen_t &iFromLen, int iFlags) {
	return ::recvfrom(_socket, pvBuf, iLen, iFlags, pstFromAddr, &iFromLen);
}

int TL_Socket::sendto(const void *pvBuf, size_t iLen, const std::string &sToAddr, uint16_t port, int iFlags) {
	struct sockaddr stToAddr;
	struct sockaddr_in *p = (struct sockaddr_in *) &stToAddr;

	bzero(&stToAddr, sizeof(struct sockaddr));

	p->sin_family = _sock_type;

	if (sToAddr == "") {
		p->sin_addr.s_addr = htonl(INADDR_BROADCAST);
	} else {
		parseAddr(sToAddr, p->sin_addr);
	}

	p->sin_port = htons(port);

	return sendto(pvBuf, iLen, &stToAddr, sizeof(stToAddr), iFlags);
}

int TL_Socket::sendto(const void *pvBuf, size_t iLen, struct sockaddr *pstToAddr, socklen_t iToLen, int iFlags) {
	return ::sendto(_socket, pvBuf, iLen, iFlags, pstToAddr, iToLen);
}

void TL_Socket::shutdown(int iHow) {
	if (::shutdown(_socket, iHow) < 0) {
		throw TL_Socket_Exception("[TL_Socket::shutdown] shutdown error.", errno);
	}
}

void TL_Socket::setblock(bool bBlock) {
	assert(_socket != INVALID_SOCKET);

	setblock(_socket, bBlock);
}

void TL_Socket::setcloexec(bool bCloExec) {
	assert(_socket != INVALID_SOCKET);

	setCloExec(_socket, bCloExec);
}

int TL_Socket::setSockOpt(int opt, const void *pvOptVal, socklen_t optLen, int level) {
	return setsockopt(_socket, level, opt, pvOptVal, optLen);
}

int TL_Socket::getSockOpt(int opt, void *pvOptVal, socklen_t &optLen, int level) {
	return getsockopt(_socket, level, opt, pvOptVal, &optLen);
}

void TL_Socket::setNoCloseWait() {
	linger stLinger;
	stLinger.l_onoff = 1; //在close socket调用后, 但是还有数据没发送完毕的时候容许逗留
	stLinger.l_linger = 0; //容许逗留的时间为0秒

	if (setSockOpt(SO_LINGER, (const void *) &stLinger, sizeof(linger), SOL_SOCKET) == -1) {
		throw TL_Socket_Exception("[TL_Socket::setNoCloseWait] error", errno);
	}
}

void TL_Socket::setCloseWait(int delay) {
	linger stLinger;
	stLinger.l_onoff = 1; //在close socket调用后, 但是还有数据没发送完毕的时候容许逗留
	stLinger.l_linger = delay; //容许逗留的时间为delay秒

	if (setSockOpt(SO_LINGER, (const void *) &stLinger, sizeof(linger), SOL_SOCKET) == -1) {
		throw TL_Socket_Exception("[TL_Socket::setCloseWait] error", errno);
	}
}

void TL_Socket::setCloseWaitDefault() {
	linger stLinger;
	stLinger.l_onoff = 0;
	stLinger.l_linger = 0;

	if (setSockOpt(SO_LINGER, (const void *) &stLinger, sizeof(linger), SOL_SOCKET) == -1) {
		throw TL_Socket_Exception("[TL_Socket::setCloseWaitDefault] error", errno);
	}
}

void TL_Socket::setTcpNoDelay() {
	int flag = 1;

	if (setSockOpt(TCP_NODELAY, (char*) &flag, int(sizeof(int)), IPPROTO_TCP) == -1) {
		throw TL_Socket_Exception("[TL_Socket::setTcpNoDelay] error", errno);
	}
}

void TL_Socket::setKeepAlive() {
	int flag = 1;
	if (setSockOpt(SO_KEEPALIVE, (char*) &flag, int(sizeof(int)), SOL_SOCKET) == -1) {
		throw TL_Socket_Exception("[TL_Socket::setKeepAlive] error", errno);
	}
}

void TL_Socket::setSendBufferSize(int sz) {
	if (setSockOpt(SO_SNDBUF, (char*) &sz, int(sizeof(int)), SOL_SOCKET) == -1) {
		throw TL_Socket_Exception("[TL_Socket::setSendBufferSize] error", errno);
	}
}

int TL_Socket::getSendBufferSize() {
	int sz;
	socklen_t len = sizeof(sz);
	if (getSockOpt(SO_SNDBUF, (void*) &sz, len, SOL_SOCKET) == -1 || len != sizeof(sz)) {
		throw TL_Socket_Exception("[TL_Socket::getSendBufferSize] error", errno);
	}

	return sz;
}

void TL_Socket::setRecvBufferSize(int sz) {
	if (setSockOpt(SO_RCVBUF, (char*) &sz, int(sizeof(int)), SOL_SOCKET) == -1) {
		throw TL_Socket_Exception("[TL_Socket::setRecvBufferSize] error", errno);
	}
}

int TL_Socket::getRecvBufferSize() {
	int sz;
	socklen_t len = sizeof(sz);
	if (getSockOpt(SO_RCVBUF, (void*) &sz, len, SOL_SOCKET) == -1 || len != sizeof(sz)) {
		throw TL_Socket_Exception("[TL_Socket::getRecvBufferSize] error", errno);
	}

	return sz;
}

void TL_Socket::setblock(int fd, bool bBlock) {
	int val = 0;

	if ((val = fcntl(fd, F_GETFL, 0)) == -1) {
		throw TL_Socket_Exception("[TL_Socket::setblock] fcntl [F_GETFL] error.", errno);
	}

	if (!bBlock) {
		val |= O_NONBLOCK;
	} else {
		val &= ~O_NONBLOCK;
	}

	if (fcntl(fd, F_SETFL, val) == -1) {
		throw TL_Socket_Exception("[TL_Socket::setblock] fcntl [F_SETFL] error.", errno);
	}
}

void TL_Socket::setCloExec(int fd, bool bCloExec) {
	int val = 0;

	if ((val = fcntl(fd, F_GETFL, 0)) == -1) {
		throw TL_Socket_Exception("[TL_Socket::setCloExec] fcntl [F_GETFL] error.", errno);
	}

	if (bCloExec) {
		val |= FD_CLOEXEC;
	} else {
		val &= ~FD_CLOEXEC;
	}

	if (fcntl(fd, F_SETFL, val) == -1) {
		throw TL_Socket_Exception("[TL_Socket::setCloExec] fcntl [F_SETFL] error.", errno);
	}
}

void TL_Socket::createPipe(int fds[2], bool bBlock) {
	if (::pipe(fds) != 0) {
		throw TL_Socket_Exception("[TL_Socket::createPipe] error.", errno);
	}

	try {
		setblock(fds[0], bBlock);
		setblock(fds[1], bBlock);
	} catch (...) {
		::close(fds[0]);
		::close(fds[1]);
		throw;
	}
}

std::vector<std::string> TL_Socket::getLocalHosts() {
	std::vector<std::string> result;
	TL_Socket ts;
	ts.createSocket(SOCK_STREAM, AF_INET);
	int cmd = SIOCGIFCONF;
	struct ifconf ifc;
	int numaddrs = 10;
	int old_ifc_len = 0;
	while (true) {
		int bufsize = numaddrs * static_cast<int>(sizeof(struct ifreq));
		ifc.ifc_len = bufsize;
		ifc.ifc_buf = (char*) malloc(bufsize);
		int rs = ioctl(ts.getfd(), cmd, &ifc);

		if (rs == -1) {
			free(ifc.ifc_buf);
			throw TL_Socket_Exception("[TL_Socket::getLocalHosts] ioctl error.", errno);
		} else if (ifc.ifc_len == old_ifc_len) {
			break;
		} else {
			old_ifc_len = ifc.ifc_len;
		}
		numaddrs += 10;
		free(ifc.ifc_buf);
	}

	numaddrs = ifc.ifc_len / static_cast<int>(sizeof(struct ifreq));
	struct ifreq* ifr = ifc.ifc_req;
	for (int i = 0; i < numaddrs; ++i) {
		if (ifr[i].ifr_addr.sa_family == AF_INET) {
			struct sockaddr_in* addr = reinterpret_cast<struct sockaddr_in*>(&ifr[i].ifr_addr);
			if (addr->sin_addr.s_addr != 0) {
				char sAddr[INET_ADDRSTRLEN] = "\0";
				inet_ntop(AF_INET, &(*addr).sin_addr, sAddr, sizeof(sAddr));
				result.push_back(sAddr);
			}
		}
	}
	free(ifc.ifc_buf);
	return result;
}
}
