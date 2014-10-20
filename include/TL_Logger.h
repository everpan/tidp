/*
 * File:   TL_Logger.h
 * Author: everpan
 *
 * Created on 2011年6月7日, 下午4:33
 */

#ifndef TL_LOGGER_H
#define	TL_LOGGER_H
#include <iostream>
#include <ostream>
#include <map>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "TL_Singleton.h"
#include "TL_Thread.h"
#include "TL_MemString.h"
#include "TL_Datetime.h"
#include "TL_Exp.h"
namespace tidp {

class TL_LoggerCache: public TL_ThreadLock {
public:
	TL_LoggerCache();
	~TL_LoggerCache();

	TL_MemString* getCache(const std::string& filename);
	void prepCache(const std::string& filename, TL_MemString* memcache);
	//void releasCache(TL_MemString* memcache);
	//TL_MemString* operator(const std::string& filename);
	void flush();
	int rename(const std::string& filename);
	void init(const std::string& fileprefix, int filenum, size_t filemaxsize);
	std::string sysinfo();

	typedef struct logfilecache_struct {
		std::list<TL_MemString*> idle; //闲置cache
		std::set<TL_MemString*> busy; //正在使用的cache
		std::list<TL_MemString*> prep; //闲置且需要处理的cache，通常等待数据清理
		//由单独线程进行处理，写线程。
		int fd;
		size_t fsize;

		logfilecache_struct() {
			idle.clear();
			busy.clear();
			prep.clear();
			fd = 0;
			fsize = 0;
		}
	} LOGFILECACHE_T;
private:
	std::map<std::string, LOGFILECACHE_T> _logcache;
	//std::map<std::string,TL_MemString*> _mergercache;
	std::string _file_prefix;
	TL_MemString _merger;
	size_t _file_max_size;
	int _file_num;
};

class TL_LoggerStrem;
typedef TL_LoggerStrem&(*TL_LoggerStremFUNT)(TL_LoggerStrem& orig);

class TL_Logger: public TL_Singleton<TL_Logger>, public TL_Thread {
public:
	TL_Logger();
	TL_LoggerStrem getLogStream(const std::string& fname = "sys");
	TL_MemString* getMemString(const std::string& fname);
	std::string getTimeTips();
	void prepMemString(const std::string& fname, TL_MemString* mstr);
	void run();
	void init(const std::string& logprefix, int lognum = 10, size_t maxlogsize = 50 * 1024 * 1024);
	std::string sysinfo();
protected:
	TL_Logger(const TL_Logger& orig);
private:
	bool _bterminated;
	TL_LoggerCache _logcache;
	TL_Datetime _dt;
};

class TL_LoggerStrem {
public:

	enum {
		NO_LOG = 0, ERROR_LOG = 1, DEBUG_LOG, INFO_LOG, WARNING_LOG
	};
	TL_LoggerStrem(const std::string& fname, TL_MemString * mstr);
	//TL_LoggerStrem(const TL_LoggerStrem& orig);
	virtual ~TL_LoggerStrem();

	template<class T>
	TL_LoggerStrem& operator <<(const T& t) {
		if (_mstring == NULL) {
			_mstring = TL_Logger::getInstance()->getMemString(_fname);
			(*_mstring) << TL_Logger::getInstance()->getTimeTips();
		}
		(*_mstring) << t;
		return *this;
	}

	TL_LoggerStrem& operator <<(TL_LoggerStremFUNT fn) {
		if (_mstring == NULL) {
			TL_Logger* logerPtr = TL_Logger::getInstance();
			_mstring = logerPtr->getMemString(_fname);
			(*_mstring) << logerPtr->getTimeTips();
		}
		fn(*this);
		return *this;
	}
	TL_MemString& getMemString();
	//void setMemString(TL_MemString *mstr);
	//const std::string& getFilename();

	void flush();

	void setLevel(int level) {
		_level = level;
	}

	int getLevel() {
		return _level;
	}
protected:
	TL_LoggerStrem& operator =(const TL_LoggerStrem& orig);
private:
	TL_MemString * _mstring;
	std::string _fname;
	//TL_Datetime _dt;
	int _level;
	int _cur_level;
};
TL_LoggerStrem& flush(TL_LoggerStrem& orig);
TL_LoggerStrem& debug(TL_LoggerStrem& orig);
TL_LoggerStrem& info(TL_LoggerStrem& orig);
TL_LoggerStrem& error(TL_LoggerStrem& orig);
TL_LoggerStrem& warning(TL_LoggerStrem& orig);
TL_LoggerStrem& endl(TL_LoggerStrem& orig);

#define THREAD_LOG_SUPPORT TL_Logger::getInstance()->start();
#define LOG(x) (TL_Logger::getInstance()->getLogStream(x))
#define LOGL(x) (TL_Logger::getInstance()->getLogStream(x) << debug << __FILE__ << ":" << __LINE__ << ' ')

}
#endif	/* TL_LOGGER_H */
