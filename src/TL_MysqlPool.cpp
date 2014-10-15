/*
 * File:   TL_MysqlPool.cpp
 * Author: everpan
 *
 * Created on 2011年6月29日, 上午11:44
 */

#include "TL_MysqlPool.h"
namespace tidp {

TL_MysqlPool::TL_MysqlPool() {
	_max = 2;
	_port = 3306;
}

TL_MysqlPool::~TL_MysqlPool() {
	clear();
}

void TL_MysqlPool::init(const std::string& hostname, const std::string& username, const std::string& password, int port, const std::string& dbname) {
	_hostname = hostname;
	_username = username;
	_password = password;
	_port = port;
	_dbname = dbname;
}

TL_Mysql* TL_MysqlPool::get() {
	TL_MysqlPool_get_label: TL_ThreadLock::Lock lk(*this);
	if (_idle.size() == 0) {
		if (_busy.size() >= _max) {
			wait();
		} else {
			TL_Mysql* psql = new TL_Mysql();
			psql->connect(_hostname, _username, _password, _port, _dbname);
			psql->query("set names gbk");
			_busy.insert(psql);
			return psql;
		}
	}
	if (_idle.size() == 0) {
		lk.release();
		goto TL_MysqlPool_get_label;
	} else {
		std::set<TL_Mysql*>::iterator it = _idle.begin();
		TL_Mysql* psql = (*it);
		_idle.erase(it);
		_busy.insert(psql);
		return psql;
	}
}

void TL_MysqlPool::release(TL_Mysql* psql) {
	TL_ThreadLock::Lock lk(*this);
	std::set<TL_Mysql*>::iterator it = _busy.find(psql);
	if (it != _busy.end()) {
		_busy.erase(it);
		_idle.insert(psql);
		notifyAll();
	} else {
		throw TL_Exp("TL_MysqlPool::release the point of TL_Mysql is not in busy's queue.");
	}
}

void TL_MysqlPool::setPoolSize(int size) {
	TL_ThreadLock::Lock lk(*this);
	_max = size;
	if (_max <= 0)
		_max = 2;
}

void TL_MysqlPool::clear() {
	TL_ThreadLock::Lock lk(*this);
	std::set<TL_Mysql*>::iterator it = _busy.begin();
	while (it != _busy.end()) {
		delete *it;
		++it;
	}
	it = _idle.begin();
	while (it != _idle.end()) {
		delete *it;
		++it;
	}
	_busy.clear();
	_idle.clear();
}
}
