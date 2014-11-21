/*
 * TL_KeyLiner.h
 *
 *  Created on: Oct 13, 2014
 *      Author: ever
 */

#ifndef TL_KEYLINER_H_
#define TL_KEYLINER_H_

#include <string>
#include <map>
#include <set>
#include <vector>

namespace tidp {
using namespace std;

class TL_KeyLiner {
public:
	typedef unsigned int kid_t;
	typedef string key_t;
	typedef map<key_t, kid_t> keyvalue_t;
	typedef map<kid_t, const key_t *> index_t;

	TL_KeyLiner();
	virtual ~TL_KeyLiner();

	//添加并返回key的编号，如果存在，则返回历史编号
	kid_t add(const key_t& key);
	//查找历史编号，如果不存在，返回0
	kid_t get(const key_t& key);
	//取得编号对应的key值
	const key_t & get(kid_t kid);
	//删除key
	void erase(const key_t& key);
	//删除编号对应的key
	void erase(const kid_t kid);
	//提供多个编号，取得对应编号的值
	void getMap(const set<kid_t>& kids, map<kid_t, const key_t *>& ikmap);
	const index_t & getIndex() const;
	const keyvalue_t& getKeyValue() const;
	void add(const vector<key_t>& keys, vector<kid_t>& kids);
	void get(const vector<key_t>& keys, vector<kid_t>& kids);

	kid_t operator[](const key_t& key);
	const key_t& operator[](const kid_t& kid);

private:
	kid_t getKID();
private:
	kid_t _kid;
	keyvalue_t _kimap;
	index_t _ikindex;
};

} /* namespace tidp */

#endif /* TL_KEYLINER_H_ */
