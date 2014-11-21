/*
 * TL_KeyLiner.cpp
 *
 *  Created on: Oct 13, 2014
 *      Author: ever
 */

#include "TL_KeyLiner.h"

namespace tidp {

TL_KeyLiner::TL_KeyLiner() {
	// TODO Auto-generated constructor stub
	_kid = 0;
}

TL_KeyLiner::~TL_KeyLiner() {
	// TODO Auto-generated destructor stub
}

//添加并返回key的编号，如果存在，则返回历史编号
TL_KeyLiner::kid_t TL_KeyLiner::add(const TL_KeyLiner::key_t& key) {
	map<key_t, kid_t>::iterator it = _kimap.find(key);
	if (it != _kimap.end()) {
		return it->second;
	} else {
		kid_t kid = getKID();
		_kimap[key] = kid;
		it = _kimap.find(key);
		_ikindex[kid] = &it->first;
		return kid;
	}
	//return 0;
}
//查找历史编号，如果不存在，返回0
TL_KeyLiner::kid_t TL_KeyLiner::get(const TL_KeyLiner::key_t& key) {
	map<key_t, kid_t>::iterator it = _kimap.find(key);
	if (it != _kimap.end()) {
		return it->second;
	}
	return 0;
}
//取得编号对应的key值
const TL_KeyLiner::key_t & TL_KeyLiner::get(TL_KeyLiner::kid_t kid) {
	map<kid_t, const key_t *>::iterator it = _ikindex.find(kid);
	if (it == _ikindex.end()) {
		static string empty;
		return empty;
	} else {
		return *it->second;
	}
}
//删除key
void TL_KeyLiner::erase(const TL_KeyLiner::key_t& key) {
	map<key_t, kid_t>::iterator it = _kimap.find(key);
	if (it != _kimap.end()) {
		_ikindex.erase(it->second);
		_kimap.erase(it);
	}
}
//删除编号对应的key
void TL_KeyLiner::erase(const TL_KeyLiner::kid_t kid) {
	map<kid_t, const key_t *>::iterator it = _ikindex.find(kid);
	if (it != _ikindex.end()) {
		_ikindex.erase(it);
		_kimap.erase(*it->second);
	}
}
//提供多个编号，取得对应编号的值
void TL_KeyLiner::getMap(const set<kid_t>& kids, map<kid_t, const key_t *>& ikmap) {
	ikmap.clear();
	set<TL_KeyLiner::kid_t>::iterator it = kids.begin();
	while (it != kids.end()) {
		map<kid_t, const key_t *>::iterator it2 = _ikindex.find(*it);
		if (it2 != _ikindex.end()) {
			ikmap.insert(*it2);
		}
		++it;
	}
}

const TL_KeyLiner::index_t & TL_KeyLiner::getIndex() const {
	return _ikindex;
}

const TL_KeyLiner::keyvalue_t& TL_KeyLiner::getKeyValue() const {
	return _kimap;
}

void TL_KeyLiner::add(const vector<key_t>& keys, vector<kid_t>& kids) {
	kids.clear();
	vector<key_t>::const_iterator it = keys.begin();
	while (it != keys.end()) {
		kids.push_back(add(*it));
		++it;
	}
}
void TL_KeyLiner::get(const vector<key_t>& keys, vector<kid_t>& kids) {
	kids.clear();
	vector<key_t>::const_iterator it = keys.begin();
	map<key_t, kid_t>::iterator it2;
	while (it != keys.end()) {
		it2 = _kimap.find(*it);
		if (it2 != _kimap.end()) {
			kids.push_back(it2->second);
		} else {
			kids.push_back(0);
		}
		//kids.push_back(add(*it));
		++it;
	}
}

TL_KeyLiner::kid_t TL_KeyLiner::operator[](const key_t& key) {
	return get(key);
}
const TL_KeyLiner::key_t& TL_KeyLiner::operator[](const kid_t& kid) {
	return get(kid);
}

//private function
TL_KeyLiner::kid_t TL_KeyLiner::getKID() {
	return ++_kid;
}
} /* namespace tidp */
