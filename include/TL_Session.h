/*
 * File:   TL_Session.h
 * Author: everpan
 *
 * Created on 2011年6月24日, 上午8:53
 */

#ifndef TL_SESSION_H
#define	TL_SESSION_H
#include <string>
#include <map>
#include <list>
#include <set>
#include <string.h>
#include "TL_Socket.h"
#include "TL_Logger.h"
#include "TL_ThreadLock.h"
#include "TL_Exp.h"

namespace tidp {

class TL_SessionMgr;
class TL_Session;

//static int defaultPacketParse(TL_Session& session);
//static int defaultAuthentication(TL_Session& session);

typedef int (*Fn)(TL_Session& session);
typedef int (*AFn)(TL_Session& session);

class TL_Session {
public:
	TL_Session();
	virtual ~TL_Session();
	int getfd() const;
	void read();
	bool authentication();
	unsigned int getSessionId();
	void initIpPort();
	std::string getIp();
	int getPort();
	void close();
	void setProtocolFun(Fn fn);
	void setAuthenFun(Fn fn);
	Fn getAuthenFun();
	Fn getProtocolFun();
	TL_Socket& getSocket();
	std::string& getRecvBufferRef();
	unsigned int getSequence();
	unsigned int incSequence();
	void setUptime(time_t t);
	time_t getUptime();
	time_t getConnTimeAt();
	void setSessionType(int type);
	int getSessionType();
protected:
	TL_Session(const TL_Session& orig);
	TL_Session& operator =(const TL_Session&);

	friend class TL_SessionMgr;
private:
	Fn _fn; //协议解析函数
	Fn _af;
	unsigned int _sessionid; //sessionid
	TL_Socket _socket; //socket
	std::string _ip; //ip
	uint16_t _port; //port
	std::string _recvbuffer; //缓冲
	time_t _last_uptime; //最后更新时间
	time_t _connect_timeat; //连接时间
	// std::string _sendbuffer;
	// std::string _apacket;
	unsigned int _sequenceid; //发送序列
	int _session_type;
	char _buffer[2048];
};

/**
 * session 管理类
 * session 的创建由外部new 其销毁交给管理类
 */
class TL_SessionMgr: public TL_ThreadLock {
public:
	typedef const std::map<unsigned int, TL_Session*> SESSION_QUEUE_T;
	TL_SessionMgr();
	virtual ~TL_SessionMgr();

	void registSession(TL_Session* session);
	void registSessionByNolock(TL_Session* session);
	void addAuthenSession(TL_Session* session);

	//void addTempSession(TL_Session* session);
	/**
	 * removeSession 删除注册的session
	 * @param session
	 * @param isdestory 是否自动销毁,默认不销毁,由用户自己把握
	 */
	void unregistSession(TL_Session* session, bool isdestory = false);
	/**
	 * destorySession 删除注册的session 并自动销毁session
	 * @param session
	 */
	void destorySession(TL_Session* session);
	/**
	 * 支持select模型的io
	 * @param rfds
	 * @param wfds
	 * @param efds
	 * @param max_fd
	 */
	void clearInvalidSession();
	void clear();
	void close(int type);
	void closeTimeout(time_t uptime);
	int selectMode(fd_set* rfds, fd_set*wfds, fd_set* efds);
	//遍历读取
	void selectRead(fd_set* rfds);
	void selectError(fd_set* rfds);
	/**
	 *
	 * @param sessionid
	 * @return
	 */
	//TL_Session* getSessionById(unsigned int sessionid);
	//发送数据到指定的session,
	int send(unsigned int sessionid, const std::string& data);
	int send(unsigned int sessionid, const char * data, int len);
	int sendByLock(unsigned int sessionid, const char * data, int len);
	bool hasSessionByIp(const std::string& ip);
	unsigned int getSessionIdByIp(const std::string& ip, int itype = -1);
	bool getSessionIdByIp(const std::string& ip, std::list<TL_Session*>& slist);
	TL_Session* getSessionById(const unsigned int sessionid);

	const std::map<unsigned int, TL_Session*>& getSessions() const {
		return _sessions;
	}

	void setUptime(time_t now) {
		_last_update_time = now;
	}
	time_t getUptime() {
		return _last_update_time;
	}
private:
	std::map<unsigned int, TL_Session*> _sessions;
	std::set<TL_Session*> _authen_session; //临时session,未认证session
	//ip对session的索引
	std::map<std::string, std::list<TL_Session*> > _ipsessions;

	//unsigned int _auto_increment_sessionid;
	unsigned int _auto_inc_id;
	//unsigned int _min_sessionid;
protected:
	time_t _last_update_time;
	TL_SessionMgr(const TL_SessionMgr& orig);
	TL_SessionMgr& operator=(const TL_SessionMgr&);
};
}
#endif	/* TL_SESSION_H */

