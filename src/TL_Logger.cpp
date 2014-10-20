/* 
 * File:   TL_Logger.cpp
 * Author: everpan
 * 
 * Created on 2011年6月7日, 下午4:32
 */

#include "TL_Logger.h"
namespace tidp {

//--------------------------------------------------------------------------

TL_LoggerCache::TL_LoggerCache() {
	_file_max_size = (500 << 20); //500M
	_file_num = 10;
}

TL_LoggerCache::~TL_LoggerCache() {
	std::map<std::string, LOGFILECACHE_T>::iterator it = _logcache.begin();
	while (it != _logcache.end()) {
		//delete busy queue
		std::set<TL_MemString*>& cachelistbusy = it->second.busy;
		std::set<TL_MemString*>::iterator it2 = cachelistbusy.begin();
		while (it2 != cachelistbusy.end()) {
			//todo flush
			//cachelistbusy
			delete (*it2);
			++it2;
		}
		//delete idle queue
		std::list<TL_MemString*>& cachelistidle = it->second.idle;
		std::list<TL_MemString*>::iterator it3 = cachelistidle.begin();
		while (it3 != cachelistidle.end()) {
			//todo flush
			delete (*it3);
			++it3;
		}
		++it;
	}
}

/**
 * getCache
 * 获取对应文件的一个缓存
 * @param filename
 * @return
 */
TL_MemString* TL_LoggerCache::getCache(const std::string& filename) {
	TL_ThreadLock::Lock lk(*this);
	LOGFILECACHE_T& logfilecache = _logcache[filename];
	/*
	 std::map<std::string,TL_MemString*>::iterator it = _mergercache.find(filename);

	 if(it == _mergercache.end()){
	 //new file
	 //_logcache
	 TL_MemString* mem = new TL_MemString();
	 logfilecache.busy.insert(mem); //永远处于busy状态，自己内部使用
	 _mergercache[filename] = mem;
	 }
	 */

	if (logfilecache.prep.size() > 200) {
		//写对象太多 等待下
		timedwait(800);
	}
	if (logfilecache.idle.size() > 0) {
		//todo 这里可以控制idle的数量，防止过分膨胀
		TL_MemString* mem = logfilecache.idle.front();
		logfilecache.idle.pop_front();
		logfilecache.busy.insert(mem);
		return mem;
	} else {
		TL_MemString* mem = new TL_MemString();
		logfilecache.busy.insert(mem);
		return mem;
	}
}
//释放cache

void TL_LoggerCache::prepCache(const std::string& filename, TL_MemString* memcache) {
	TL_ThreadLock::Lock lk(*this);
	//busy to prep
	LOGFILECACHE_T& logfilecache = _logcache[filename];
	std::set<TL_MemString*>::iterator it = logfilecache.busy.find(memcache);
	if (it == logfilecache.busy.end()) {
		throw TL_Exp("TL_LoggerCache::prepCache the memcache ptr is not busy!");
	}
	logfilecache.busy.erase(it);
	//todo check uniq
	logfilecache.prep.push_back(memcache);
	notifyAll();
}

/**
 *  将预处理队列中的数据处理完，并将cache放置到idle队列
 * 单独线程运行。
 */
void TL_LoggerCache::flush() {
	TL_ThreadLock::Lock lk(*this);
	std::map<std::string, LOGFILECACHE_T>::iterator it = _logcache.begin();
	while (it != _logcache.end()) {
		LOGFILECACHE_T& lfcache = it->second;
		std::list<TL_MemString*>& preplist = lfcache.prep;
		std::list<TL_MemString*>& idlelist = lfcache.idle;
		int fd = lfcache.fd;

		if (fd <= 0) {
			std::string filename = _file_prefix + it->first + ".log";
			fd = open(filename.c_str(),
			O_WRONLY | O_CREAT | O_APPEND, 0666);
			if (fd < 0) {
				throw TL_Exp("TL_LoggerCache::flush can't open file [" + filename + "].");
			}
			lfcache.fd = fd;
			struct stat st;
			fstat(fd, &st);
			lfcache.fsize = st.st_size;
		}
		//TL_MemString* mergerstr = _mergercache[it->first];

		while (preplist.size() > 0) {
			TL_MemString* mem = preplist.front();
			preplist.pop_front();
			//write log
			//lfcache.fsize += mem->write(fd);

			_merger.append(*mem);
			mem->clear();
			idlelist.push_back(mem);
		}
		lfcache.fsize += _merger.write(fd);
		_merger.clear();
		//rename file while touch max
		if (lfcache.fsize >= _file_max_size) {
			//std::string filename = ;
			::close(lfcache.fd);
			//lfcache.fd = -1;
			lfcache.fd = rename(_file_prefix + it->first);
			lfcache.fsize = 0;
		}
		++it;
	}
	wait();
}

int TL_LoggerCache::rename(const std::string& filename) {
	char fname[100];
	int i = _file_num - 2;
	if (i < 0)
		i = 2;

	std::string from, to;
	//if()
	for (; i >= 0; --i) {
		sprintf(fname, "%s%d.log", filename.c_str(), i);
		from = fname;
		if (access(from.c_str(), F_OK) == 0) {
			sprintf(fname, "%s%d.log", filename.c_str(), i + 1);
			::rename(from.c_str(), fname);
		}
	}
	from = filename + ".log";
	to = filename + "0.log";
	if (access(from.c_str(), F_OK) == 0) {
		::rename(from.c_str(), to.c_str());
	}
	int fd = open(from.c_str(),
	O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (fd < 0) {
		throw TL_Exp("TL_LoggerCache::flush can't open file [" + from + "].");
	}
	return fd;
}

void TL_LoggerCache::init(const std::string& fileprefix, int filenum, size_t filemaxsize) {
	_file_prefix = fileprefix;
	if (fileprefix.size() > 0 && fileprefix[fileprefix.size() - 1] != '_')
		_file_prefix += '_';
	_file_num = filenum;
	_file_max_size = filemaxsize;
}

std::string TL_LoggerCache::sysinfo() {
	TL_ThreadLock::Lock lk(*this);
	TL_MemString mstring;
	std::map<std::string, LOGFILECACHE_T>::iterator it = _logcache.begin();
	mstring << "LOG SYS INFO:\n";
	//int sysfd = 0;
	while (it != _logcache.end()) {
		LOGFILECACHE_T& lfc = it->second;
		mstring << "[" << it->first << "]:\n";
		mstring << "\tidle: " << lfc.idle.size() << "\tbusy: " << lfc.busy.size() << "\tprep: " << lfc.prep.size()
				<< "\tfsize: " << lfc.fsize << "\n";
		/*
		 if (it->first == "sys") {
		 sysfd = lfc.fd;
		 }*/
		++it;
	}
	//if (sysfd <= 0) sysfd = 1;
	//mstring.write(sysfd);
	return std::string(mstring.c_str(), mstring.size());
}
//---------------------------------------------------------------------------------

TL_LoggerStrem::TL_LoggerStrem(const std::string& fname, TL_MemString * mstr) {
	_mstring = mstr;
	_fname = fname;
	if (_fname.empty())
		_fname = "sys";
	(*_mstring) << TL_Logger::getInstance()->getTimeTips();
	_level = 0xffffffff;
	_cur_level = _level;
}

/*
 TL_LoggerStrem::TL_LoggerStrem(const TL_LoggerStrem& orig) {
 _mstring = orig._mstring;
 _fname = orig._fname;
 _level = orig.
 }
 */
TL_LoggerStrem::~TL_LoggerStrem() {

}

TL_MemString& TL_LoggerStrem::getMemString() {
	return *_mstring;
}

void TL_LoggerStrem::flush() {
	TL_Logger::getInstance()->prepMemString(_fname, _mstring);
	_mstring = NULL;
	//_mstring = TL_Logger::getInstance()->getMemString(_fname);
	//*(_mstring) << TL_Logger::getInstance()->getTimeTips();
}

TL_LoggerStrem& flush(TL_LoggerStrem& orig) {
	orig.flush();
	return orig;
}

TL_LoggerStrem& endl(TL_LoggerStrem& orig) {
	orig << '\n';
	orig.flush();
	return orig;
	//todo here is a question
}

TL_LoggerStrem& debug(TL_LoggerStrem& orig) {
	orig << " DEBUG:";
	return orig;
}

TL_LoggerStrem& info(TL_LoggerStrem& orig) {
	orig << " INFO:";
	return orig;
}

TL_LoggerStrem& error(TL_LoggerStrem& orig) {
	orig << " ERROR:";
	return orig;
}

TL_LoggerStrem& warnning(TL_LoggerStrem& orig) {
	orig << " WARNING:";
	return orig;
}
//----------------------------------------------------------------------------------

TL_Logger::TL_Logger() {
	_dt.setTimeStringFormat("[%Y-%m-%d %H:%M:%S]");
	_bterminated = false;
}

TL_LoggerStrem TL_Logger::getLogStream(const std::string& fname) {
	//if(fname.empty())
	return TL_LoggerStrem(fname, _logcache.getCache(fname));
}

TL_MemString* TL_Logger::getMemString(const std::string& fname) {
	return _logcache.getCache(fname);
}

std::string TL_Logger::getTimeTips() {
	_dt.sync();
	return _dt.toString();
}

void TL_Logger::prepMemString(const std::string& fname, TL_MemString* mstr) {
	_logcache.prepCache(fname, mstr);
}

void TL_Logger::init(const std::string& logprefix, int lognum, size_t maxlogsize) {
	_logcache.init(logprefix, lognum, maxlogsize);
}

std::string TL_Logger::sysinfo() {
	return _logcache.sysinfo();
}

void TL_Logger::run() {
	while (!_bterminated) {
		try {
			_logcache.flush();
		} catch (TL_Exp& e) {
			std::cout << __FUNCTION__ << ":" << __LINE__ << e.what() << std::endl;
		} catch (...) {
			std::cout << "exception:" << __FUNCTION__ << ":" << __LINE__ << std::endl;
		}
	}
}

}