/*
 * TL_Dimension.h
 *
 *  Created on: Oct 11, 2014
 *      Author: ever
 */

#ifndef TL_DIMENSION_H_
#define TL_DIMENSION_H_
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "TL_Exp.h"
#include "TL_KeyLiner.h"
static const unsigned char CHAR_BIT_CNT_TABLE[256] = { // 0
		0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, //1
				1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, //2
				1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, //3
				2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, //4
				1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, //5
				2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, //6
				2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, //7
				3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, //8
				1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, //9
				2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, //a
				2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, //b
				3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, //c
				2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, //d
				3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, //e
				3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, //f
				4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8 //0
		};
//只对低8位取值，一般用于限定dimension的level不超过8个。
inline int calByteBits(unsigned int a) {
	return CHAR_BIT_CNT_TABLE[a & 0xFF];
}
inline int calBits(unsigned int a) {
	unsigned char * p = (unsigned char *) &a;
	return CHAR_BIT_CNT_TABLE[*p] + CHAR_BIT_CNT_TABLE[*(p + 1)] + CHAR_BIT_CNT_TABLE[*(p + 2)]
			+ CHAR_BIT_CNT_TABLE[*(p + 3)];
}
namespace tidp {
using namespace std;

template<class T>
struct TL_VecCmp {
	bool operator()(const vector<T>& v1, const vector<T>& v2) const {
		for (size_t i = 0; i < v1.size(); ++i) {
			if (v1[i] < v2[i]) {
				return true;
			} else if (v1[i] == v2[i]) {
				continue;
			} else {
				return false;
			}
		}
		return false;
	}
};
template<>
struct TL_VecCmp<string> {
	bool operator()(const vector<string>& v1, const vector<string>& v2) const {
		for (size_t i = 0; i < v1.size(); ++i) {
			int ret = v1[i].compare(v2[i]);
			if (ret < 0) {
				return true;
			} else if (ret == 0) {
				continue;
			} else {
				return false;
			}
		}
		return false;
	}
};
class TL_Dimension {
public:
	typedef TL_KeyLiner::kid_t did_t;
	typedef did_t dim_id_t;
	typedef unsigned int dim_bit_t;

	typedef struct dim_struct {
		dim_bit_t bit;
		vector<string> dims;
		bool operator <(const dim_struct &) const;
	} dim_t;

	struct VecDidPtrCmp {
		bool operator()(const vector<did_t>* v1, const vector<did_t>* v2) const {
			static TL_VecCmp<did_t> vscmp;
			return vscmp(*v1, *v2);
		}
	};
	TL_Dimension();
	virtual ~TL_Dimension();

	int getLevelSize();

	dim_id_t add(dim_bit_t, const char ** dimPtr, int num);
	dim_id_t add(dim_bit_t, const vector<string> & dim);
	dim_id_t add(const dim_struct & dim);
	//根据维度值 获取编号
	dim_id_t get(const dim_struct & dim);
	//根据维度bit位获取 所有编号值。
	void get(dim_bit_t bit, vector<dim_id_t> & dims);
	void get(dim_bit_t bit, set<dim_id_t> & dims);

	void setHierarchyName(const string& hname);
	const string& getHierarchyName();
	void setLevelName(int level, const string& lname);
	const string& getLevelName(int level);

private:
	dim_id_t getDimID();
private:
	did_t _did;
	vector<string> _levelnames;
	string _hierarchy;
	TL_KeyLiner _keys;
	int _level_size; //cache
	map<dim_bit_t, map<vector<did_t>, did_t, TL_VecCmp<did_t> > > _dimmap;
	map<dim_bit_t, map<did_t, const vector<did_t>*> > _idindex;
};

} /* namespace tidp */

#endif /* TL_DIMENSION_H_ */
