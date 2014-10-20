/*
 * TL_MultiLiner.h
 *
 *  Created on: Oct 11, 2014
 *      Author: ever
 */

#ifndef TL_MULTILINER_H_
#define TL_MULTILINER_H_

#include <vector>
#include <map>
#include <set>
#include <string>
#include <stdint.h>
using namespace std;
namespace tidp {
//typedef unsigned int uint32_t;

static const unsigned char CHAR_BIT_CNT_TABLE[256] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3,
		3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4,
		5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2,
		3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4,
		4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5,
		6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6,
		5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8 };

template<class T>
class TL_MulitLiner {
public:
	typedef T value_type;
	//template<class T2>
	struct VecTCmp {
		bool operator()(const vector<T>& vs1, const vector<T>& vs2) const {
			if (vs1.size() < vs2.size()) {
				return true;
			} else if (vs1.size() == vs2.size()) {
				for (uint32_t i = 0; i < vs1.size(); ++i) {
					if (vs1[i] < vs2[i]) {
						return true;
					} else if (vs1[i] == vs2[i]) {
						continue;
					} else {
						return false;
					}
				}
			}
			return false;
		}
	};

	struct Level_t {
		uint32_t uid;
		vector<T> members;
	};
	typedef map<vector<T>, uint32_t, typename TL_MulitLiner<T>::VecTCmp> VecTid_t;
	typedef map<uint32_t, VecTid_t> CombinaVecTid_t;
	typedef typename VecTid_t::iterator VecTid_it;
	typedef typename CombinaVecTid_t::iterator CombinaVecTid_it;
	TL_MulitLiner() {
		_uid = 1;
	}

	//根据id 找到string
	const vector<string> getMultiString(uint32_t uid);

	uint32_t setUid(uint32_t bitpos, const vector<T> & vs);
	uint32_t getUid(uint32_t bitpos, const vector<T> & vs);

	uint32_t getCombinaMembers(uint32_t bitpos, set<vector<T> >);

	void get(const set<uint32_t>& uids, map<uint32_t, string> uid_names);

	uint32_t getUid() {
		//return __sync_fetch_and_add(&_uid, 1);
		return ++_uid;
	}
private:
	CombinaVecTid_t _combinaVecTid;

	//map<uint32_t,>
	uint32_t _uid;
};
template<class T> uint32_t TL_MulitLiner<T>::setUid(uint32_t bitpos, const vector<T> & vs) {
	CombinaVecTid_it it = _combinaVecTid.find(bitpos);
	if (it != _combinaVecTid.end()) {
		VecTid_it it2 = it->second.find(vs);
		if (it2 != it->second.end()) {
			return it2->second;
		} else {
			uint32_t uid = getUid();
			_combinaVecTid[bitpos][vs] = uid;
			return uid;
		}
	} else {
		uint32_t uid = getUid();
		_combinaVecTid[bitpos][vs] = uid;
		return uid;
	}
	return 0;
}

template<class T> uint32_t TL_MulitLiner<T>::getUid(uint32_t bitpos, const vector<T> & vs) {
	CombinaVecTid_it it = _combinaVecTid.find(bitpos);
	if (it != _combinaVecTid.end()) {
		VecTid_it it2 = it->second.find(vs);
		if (it2 != it->second.end()) {
			return it2->second;
		}
	}
	return 0;
}

}

#endif /* TL_MULTILINER_H_ */
