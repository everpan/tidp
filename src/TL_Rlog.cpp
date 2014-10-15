/* 
 * File:   TL_Rlog.cpp
 * Author: everpan
 * 
 * Created on 2011年4月21日, 上午9:29
 * 
 * 单线程日志，不支持多线程。
 */

#include "TL_Rlog.h"
#include <pthread.h>
namespace tidp {

TL_Rlog::TL_Rlog() {

	_setlevel = DEBUG_LOG;
	_level = NORMAL_LOG;
	_buf_count = 0;
	_pid = getpid();

	_flags[0] = true;
	_flags[1] = false;
	_flags[2] = false;
	_flags[3] = true;

	_fd = -1;

	_lock = false;
	_cur_time.setTimeStringFormat("[%Y-%m-%d %H:%M:%S]");
	_debugtip = "DEBUG:";
	head();
}

TL_Rlog::~TL_Rlog() {
}

void TL_Rlog::Init(const std::string& sPreFix, const std::string& module, size_t maxsize, size_t maxnum) {
	if (_fd > 0) {
		close(_fd);
		_fd = -1;
	}
	_module = module;
	_filename = sPreFix;
	_max_log_size = maxsize;
	_max_log_num = maxnum;
	_buf_count = 0;
	_pid = getpid();

	_flags[0] = true;
	_flags[1] = false;
	_flags[2] = false;
	_flags[3] = true;

	_lock = false;

#ifdef O_LARGEFILE
	_fd = open((_filename + ".log").c_str(), O_WRONLY | O_CREAT | O_APPEND | O_LARGEFILE, 0644);
#else
	_fd = open((_filename + ".log").c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
#endif
	if (_fd < 0) {
		throw TL_Exp(std::string("can not open file:") + _filename + ".log");
	}
}

void TL_Rlog::set_titleflag(title_flag f) {
	_flags[f] = true;
	_pid = getpid();

	switch (_level) {
		case NO_LOG:
			_debugtip = "";
			break;
		case ERROR_LOG:
			_debugtip = "ERROR:";
			break;
		case NORMAL_LOG:
			_debugtip = "";
			break;
		case DEBUG_LOG:
			_debugtip = "DEBUG:";
			break;
		default:
			break;
	}
}

void TL_Rlog::set_level(const Log_Level & lv) {
	_level = lv;
	switch (_level) {
		case NO_LOG:
			_debugtip = "";
			break;
		case ERROR_LOG:
			_debugtip = "ERROR:";
			break;
		case NORMAL_LOG:
			_debugtip = "";
			break;
		case DEBUG_LOG:
			_debugtip = "DEBUG:";
			break;
		default:
			break;
	}
}

void TL_Rlog::head() {
	_cur_time.sync();
	_head = _cur_time.toString() + ":" + _debugtip;

}

bool TL_Rlog::check_level() {
	if (_fd < 0) {
#ifdef O_LARGEFILE
		_fd = open((_filename + ".log").c_str(), O_WRONLY | O_CREAT | O_APPEND | O_LARGEFILE, 0644);
#else
		_fd = open((_filename + ".log").c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
#endif
	}
	if (_fd < 0) {
		return false;
	}

	_buf_count++;
	if (_buf_count % FLUSH_COUNT == 0) {
		_buf_count = 0;

		// close and roll file
		if (lseek(_fd, 0, SEEK_CUR) > (int) _max_log_size) {
			//todo 多线程(进程)时会写乱,需要重新open之后判断
			if (_fd < 0) {
#ifdef O_LARGEFILE
				_fd = open((_filename + ".log").c_str(), O_WRONLY | O_CREAT | O_APPEND | O_LARGEFILE, 0644);
#else
				_fd = open((_filename + ".log").c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
#endif
			}
			if (_fd < 0) {
				return false;
			}
			if (lseek(_fd, 0, SEEK_CUR) > (int) _max_log_size) {
				char sTmp[16];
				close(_fd);
				_fd = -1;
				std::string from, to;
				// remove the last one
				sprintf(sTmp, "%d", _max_log_num - 1);
				from = _filename + sTmp + ".log";
				if (access(from.c_str(), F_OK) == 0) {
					if (remove(from.c_str()) < 0) {
						throw TL_Exp("TL_Rlog::check_level:: remove " + from + " fail!!!");
					}
				}
				// rename the others
				for (int i = _max_log_num - 2; i >= 0; i--) {
					sprintf(sTmp, "%d", i);
					if (i == 0)
						from = _filename + ".log";
					else
						from = _filename + sTmp + ".log";
					sprintf(sTmp, "%d", i + 1);
					to = _filename + sTmp + ".log";
					if (access(from.c_str(), F_OK) == 0) {
						if (rename(from.c_str(), to.c_str()) < 0) {
							throw TL_Exp("TL_Rlog::check_level:: rename " + from + " fail!!!");
						}
					}
				}
				if (_fd < 0) {
#ifdef O_LARGEFILE
					_fd = open((_filename + ".log").c_str(), O_WRONLY | O_CREAT | O_APPEND | O_LARGEFILE, 0644);
#else
					_fd = open((_filename + ".log").c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
#endif
				}
				if (_fd < 0) {
					return false;
				}
			}
		}
	}
	return true;
}

TL_Rlog & TL_Rlog::operator<<(char n) {
	_log_buffer << n;
	return *this;
}

TL_Rlog & TL_Rlog::operator<<(unsigned char n) {
	_log_buffer << (int) n;
	return *this;
}

TL_Rlog & TL_Rlog::operator<<(short n) {
	_log_buffer << n;
	return *this;
}

TL_Rlog & TL_Rlog::operator<<(unsigned short n) {
	_log_buffer << n;
	return *this;
}

TL_Rlog & TL_Rlog::operator<<(int n) {
	_log_buffer << n;
	return *this;
}

TL_Rlog & TL_Rlog::operator<<(unsigned int n) {
	_log_buffer << n;
	return *this;
}

TL_Rlog & TL_Rlog::operator<<(long n) {
	_log_buffer << n;
	return *this;
}

TL_Rlog & TL_Rlog::operator<<(unsigned long n) {
	_log_buffer << n;
	return *this;
}

TL_Rlog & TL_Rlog::operator<<(long long n) {
	_log_buffer << n;
	return *this;
}

TL_Rlog & TL_Rlog::operator<<(unsigned long long n) {
	_log_buffer << n;
	return *this;
}

TL_Rlog & TL_Rlog::operator<<(double n) {
	_log_buffer << n;
	return *this;
}

TL_Rlog & TL_Rlog::operator<<(float n) {
	_log_buffer << n;
	return *this;
}

TL_Rlog & TL_Rlog::operator<<(const std::string & s) {
	_log_buffer << s;
	return *this;
}
}
