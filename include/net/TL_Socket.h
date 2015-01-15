/*
 * TL_Socket.h
 *
 *  Created on: Dec 27, 2014
 *      Author: ever
 */

#ifndef INCLUDE_NET_TL_SOCKET_H_
#define INCLUDE_NET_TL_SOCKET_H_
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <string>
#include <sys/un.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>


#include <TL_Exp.h>
namespace tidp {
namespace net {

#include <errno.h>

//extern int errno;
#ifdef __CYGWIN__
int gethostbyname_r(const char *name,
		struct hostent *ret,
		char *buf,
		size_t buflen,
		struct hostent **result,
		int *h_errnop);
#endif





class TL_Socket {
public:
	TL_Socket();
	virtual ~TL_Socket();

	void init(int fd, bool bautoclose, int iDomain = AF_INET);

	void setAutoClose(bool bautoclose) {
		_bautoclose = bautoclose;
	}

	void setOwner(bool bautoclose) {
		_bautoclose = bautoclose;
	}

	/**
	 * 套接字类型:AF_INET, AF_LOCAL
	 * @param iDomain
	 */
	void setDomain(int iDomain) {
		_sock_type = iDomain;
	}

	int getDomain() {
		return _sock_type;
	}
	/**
	 * 生成socket, 如果已经存在以前的socket, 则释放掉, 生成新的
	 * @param iDomain : socket方式SOCK_STREAM|SOCK_DGRAM
	 * @param iSocketType : socket类型
	 * @throws TL_Socket_Exception
	 * @return void
	 */
	void createSocket(int iSocketType = SOCK_STREAM, int iDomain = AF_INET);

	/**
	 * 获取socket文件描述符
	 * @return int : socket文件描述符
	 */
	int getfd() const {
		return _socket;
	}
	const std::string& getHost(){
		return _ip;
	}
	int getPort(){
		return _port;
	}
	/**
	 * socket是否有效
	 * @return bool : socket是否有效
	 */
	bool isValid() const {
		return _socket != INVALID_SOCKET;
	}

	/**
	 * 关闭socket
	 * @return void
	 */
	void close();

	/**
	 * 获取对点的ip和端口,对AF_INET的socket有效
	 * @param sPeerAddress : 对点的ip地址
	 * @param iPeerPort : 对点的端口地址
	 * @throws TL_Socket_Exception
	 * @return void
	 */
	void getPeerName(std::string &sPeerAddress, uint16_t &iPeerPort);

	/**
	 * 获取对点的ip和端口,对AF_LOCAL的socket有效
	 * @param sPathName : 文件路径
	 * @throws TL_Socket_Exception
	 * @return void
	 */
	void getPeerName(std::string &sPathName);

	/**
	 * 获取自己的ip和端口,对AF_INET的socket有效
	 * @param sSockAddress : ip地址
	 * @param iSockPort : 端口地址
	 * @throws TL_Socket_Exception
	 * @return void
	 */
	void getSockName(std::string &sSockAddress, uint16_t &iSockPort);

	/**
	 * 获取socket文件路径,对AF_LOCAL的socket有效
	 * @param sPathName
	 * @param TL_Socket_Exception
	 */
	void getSockName(std::string &sPathName);

	/**
	 * 修改socket选项
	 * @param opt : 选项名称
	 * @param pvOptVal : 选项值指针
	 * @param optLen : pvOptVal对应的长度
	 * @param level : socket操作层次, 默认是socket层
	 * @return int
	 */
	int setSockOpt(int opt, const void *pvOptVal, socklen_t optLen, int level = SOL_SOCKET);

	/**
	 * 获取socket选项值
	 * @param opt : 选项名称
	 * @param pvOptVal : 输出, 选项值指针
	 * @param optLen : 输入pvOptVal指向的缓存的长度
	 * @param level : socket操作层次, 默认是socket层
	 * @return int
	 */
	int getSockOpt(int opt, void *pvOptVal, socklen_t &optLen, int level = SOL_SOCKET);

	/**
	 * accept
	 * @param tcSock : 客户端socket结构
	 * @param pstSockAddr : 客户端地址
	 * @param iSockLen : pstSockAddr长度
	 * @return int : > 0 ,客户端socket; <0, 出错
	 */
	int accept(TL_Socket &tcSock, struct sockaddr *pstSockAddr, socklen_t &iSockLen);

	/**
	 * 绑定,对AF_INET的socket有效
	 * @param port : 端口
	 * @param sServerAddr : 服务器地址
	 * @throws TL_Socket_Exception
	 * @return void
	 */
	void bind(const std::string &sServerAddr, int port);

	/**
	 * 绑定域套接字,对AF_LOCAL的socket有效
	 * @param sPathName
	 */
	void bind(const char *sPathName);

	/**
	 * 连接其他服务,对AF_INET的socket有效(同步连接)
	 * @param sServerAddr : ip地址
	 * @param port : 端口
	 * @throws TL_Socket_Exception
	 * @return void
	 */
	void connect(const std::string &sServerAddr, uint16_t port);

	/**
	 * 连接本地套接字,对AF_LOCAL的socket有效(同步连接)
	 * @param sPathName
	 * @throws TL_Socket_Exception
	 */
	void connect(const char *sPathName);

	/**
	 * 发起连接, 连接失败的状态不通过异常返回,通过connect的返回值
	 * 在异步连接的时候需要
	 * @param sServerAddr
	 * @param port
	 * @throws TL_Socket_Exception:其他错误还是通过异常返回(例如),例如地址错误
	 * @return int
	 */
	int connectNoThrow(const std::string &sServerAddr, uint16_t port);

	/**
	 * 发起连接, 连接失败的状态不通过异常返回,通过connect的返回值
	 * 在异步连接的时候需要
	 * @param addr socket直接可用的地址
	 * @throws TL_Socket_Exception:其他错误还是通过异常返回(例如),例如地址错误
	 * @return int
	 */
	int connectNoThrow(struct sockaddr* addr);

	/**
	 * 连接本地套接字,对AF_LOCAL的socket有效
	 * 发起连接, 连接失败的状态不通过异常返回,通过connect的返回值
	 * 在异步连接的时候需要
	 * @param sPathName
	 * @throws TL_Socket_Exception:其他错误还是通过异常返回(例如),例如地址错误
	 * @return int
	 */
	int connectNoThrow(const char *sPathName);

	/**
	 * 在socket上监听
	 * @param connBackLog : 连接队列个数
	 * @throws TL_Socket_Exception
	 */
	void listen(int connBackLog);

	/**
	 * 接收数据(一般用于tcp)
	 * @param pvBuf : 接收buffer
	 * @param iLen : buffer长度
	 * @param iFlag : 标示
	 * @return int : 接收的数据长度
	 */
	int recv(void *pvBuf, size_t iLen, int iFlag = 0);

	/**
	 * 发送数据(一般用于tcp)
	 * @param pvBuf : 发送buffer
	 * @param iLen : buffer长度
	 * @param iFlag : 标示
	 * @return int : 发送了的数据长度
	 */
	int send(const void *pvBuf, size_t iLen, int iFlag = 0);

	/**
	 * 接收数据(一般用于udp)
	 * @param pvBuf : 发送buffer
	 * @param iLen : buffer长度
	 * @param sFromAddr : 输出, 服务端ip地址
	 * @param iFromPort : 输出, 服务端端口
	 * @param iFlag : 标示
	 * @return int : 接收了的数据长度
	 */
	int recvfrom(void *pvBuf, size_t iLen, std::string &sFromAddr, uint16_t &iFromPort, int iFlags = 0);

	/**
	 * 接收数据(一般用于udp)
	 * @param pvBuf : 发送buffer
	 * @param iLen : buffer长度
	 * @param pstFromAddr : 地址
	 * @param iFromLen : 输出, pstFromAddr长度
	 * @param iFlag : 标示
	 * @return int : 接收了的数据长度
	 */
	int recvfrom(void *pvBuf, size_t iLen, struct sockaddr *pstFromAddr, socklen_t &iFromLen, int iFlags = 0);

	/**
	 * 发送数据(一般用于udp)
	 * @param pvBuf : 发送buffer
	 * @param iLen : buffer长度
	 * @param sToAddr : 服务端ip地址, 如果sToAddr为空, 则udp广播
	 * @param iToPort : 服务端端口
	 * @param iFlag : 标示
	 * @return int : >0 发送的数据长度 ;<=0, 出错
	 */
	int sendto(const void *pvBuf, size_t iLen, const std::string &sToAddr, uint16_t iToPort, int iFlags = 0);

	/**
	 * 发送数据(一般用于udp)
	 * @param pvBuf : 发送buffer
	 * @param iLen : buffer长度
	 * @param pstToAddr : 服务端地址
	 * @param iToLen : pstToAddr长度
	 * @param iFlag : 标示
	 * @return int : >0 发送的数据长度 ;<=0, 出错
	 */
	int sendto(const void *pvBuf, size_t iLen, struct sockaddr *pstToAddr, socklen_t iToLen, int iFlags = 0);

	/**
	 * 关闭
	 * @param iHow : 关闭方式:SHUT_RD|SHUT_WR|SHUT_RDWR
	 * @throws TL_Socket_Exception
	 * @return void
	 */
	void shutdown(int iHow);

	/**
	 * 设置socket方式
	 * @param bBlock : true, 阻塞; false, 非阻塞
	 * @throws TL_Socket_Exception
	 * @return void
	 */
	void setblock(bool bBlock = false);
	void setcloexec(bool bCloExec = true);
	/**
	 * 设置非closewait状态, 可以重用socket
	 * @throws TL_Socket_Exception
	 * @return void
	 */
	void setNoCloseWait();

	/**
	 * 设置为closewait状态, 最常等待多久
	 * @param delay :等待时间秒
	 * @throws TL_Socket_Exception
	 */
	void setCloseWait(int delay = 30);

	/**
	 * 设置closewait缺省状态(close以后马上返回并尽量把数据发送出去)
	 * @throws TL_Socket_Exception
	 */
	void setCloseWaitDefault();

	/**
	 * 设置nodelay(只有在打开keepalive才有效)
	 * @throws TL_Socket_Exception
	 */
	void setTcpNoDelay();

	/**
	 * 设置keepalive
	 * @throws TL_Socket_Exception
	 * @return void
	 */
	void setKeepAlive();

	/**
	 * 获取recv buffer 大小
	 * @throws TL_Socket_Exception
	 * @return int
	 */
	int getRecvBufferSize();

	/**
	 * 设置recv buffer 大小
	 * @throws TL_Socket_Exception
	 * @param int
	 */
	void setRecvBufferSize(int sz);

	/**
	 * 获取发送buffer大小
	 * @throws TL_Socket_Exception
	 * @param int
	 */
	int getSendBufferSize();

	/**
	 * 设置发送buffer大小
	 * @throws TL_Socket_Exception
	 * @param sz
	 */
	void setSendBufferSize(int sz);

	/**
	 * 获取本地所有ip
	 *
	 * @throws TL_Socket_Exception
	 * @return vector<std::string>
	 */
	static std::vector<std::string> getLocalHosts();

	/**
	 * 设置socket方式
	 * @param fd : 句柄
	 * @param bBlock : true, 阻塞; false, 非阻塞
	 * @throws TL_Socket_Exception
	 * @return void
	 */
	static void setblock(int fd, bool bBlock);

	static void setCloExec(int fd, bool bCloseEx);
	/**
	 * 生成管道,抛出异常时会关闭fd
	 * @param fds
	 * @param bBlock
	 * @throws TL_Socket_Exception
	 */
	static void createPipe(int fds[2], bool bBlock);

	/**
	 * 解析地址, 从字符串(ip或域名), 解析到in_addr结构
	 * @param sAddr : 字符串
	 * @param stAddr : 地址
	 * @throws TL_Socket_Exception
	 * @return void
	 */
	static void parseAddr(const std::string &sAddr, struct in_addr &stAddr);

	/**
	 * 绑定
	 * @param pstBindAddr: 需要绑定的地址
	 * @param iAddrLen : pstBindAddr指向的结构的长度
	 * @throws TL_Socket_Exception
	 * @return void
	 */
	void bind(struct sockaddr *pstBindAddr, socklen_t iAddrLen);

protected:

	/**
	 * 连接其他服务
	 * @param pstServerAddr : 服务地址
	 * @param serverLen : pstServerAddr指向的结构的长度
	 * @return int
	 */
	int connect(struct sockaddr *pstServerAddr, socklen_t serverLen);

	/**
	 * 获取对点的地址
	 * @param pstPeerAddr : 地址指针
	 * @param iPeerLen : pstPeerAddr指向的结构长度
	 * @throws TL_Socket_Exception
	 * @return void
	 */
	void getPeerName(struct sockaddr *pstPeerAddr, socklen_t &iPeerLen);

	/**
	 * 获取自己的的ip和端口
	 * @param pstSockAddr : 地址指针
	 * @param iSockLen : pstSockAddr
	 * @throws TL_Socket_Exception
	 * @return void
	 */
	void getSockName(struct sockaddr *pstSockAddr, socklen_t &iSockLen);

private:
	/**
	 * no implementation
	 */
	TL_Socket(const TL_Socket &tcSock);

	/**
	 * no implementation
	 */
	TL_Socket & operator=(const TL_Socket &tcSock);

protected:
	static const int INVALID_SOCKET = -1;

	/**
	 * socket句柄
	 */
	int _socket;

	/**
	 * true 析构的时候自动关闭socket
	 */
	bool _bautoclose;

	/**
	 * socket类型
	 */
	int _sock_type;

	std::string _ip;
	int _port;
};

} /* namespace net */
} /* namespace tidp */

#endif /* INCLUDE_NET_TL_SOCKET_H_ */
