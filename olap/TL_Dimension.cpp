/*
 * TL_Dimension.cpp
 *
 *  Created on: Oct 11, 2014
 *      Author: ever
 */

#include "TL_Dimension.h"

namespace tidp {

TL_Dimension::TL_Dimension() {
	// TODO Auto-generated constructor stub
	_did = 0;
	_level_size = -1;
}

TL_Dimension::~TL_Dimension() {
	// TODO Auto-generated destructor stub
}
bool TL_Dimension::dim_t::operator <(const dim_struct &other) const {
	static TL_VecCmp<string> vscmp;
	return vscmp(dims, other.dims);
}

int TL_Dimension::getLevelSize() {
	if (_level_size == -1) {
		int r2 = 0;
		map<dim_bit_t, map<vector<did_t>, did_t, TL_VecCmp<did_t> > >::const_iterator it = _dimmap.begin();
		while (it != _dimmap.end()) {
			r2 = calBits(it->first);
			if (r2 > _level_size)
				_level_size = r2;
			++it;
		}
	}
	return _level_size;
}

TL_Dimension::dim_id_t TL_Dimension::add(dim_bit_t bit, const char ** dimPtr, int num) {
	if (calByteBits(bit) != num) {
		ostringstream os;
		os << "TL_Dimension bitmap is not match key count:" << bit << ":" << calBits(bit) << "!=" << num;
		throw TL_Exception(os.str(), -1);
	}

	map<dim_bit_t, map<vector<did_t>, did_t, TL_VecCmp<did_t> > >::iterator it = _dimmap.find(bit);
	if (it == _dimmap.end()) {
		_level_size = -1;
		_dimmap[bit];
		it = _dimmap.find(bit);
	}
	map<vector<did_t>, did_t, TL_VecCmp<did_t> > & dimmap = it->second;
	vector<dim_id_t> dids;
	while (--num > -1) {
		dids.push_back(_keys.add(dimPtr[num]));
	}

	map<vector<did_t>, did_t, TL_VecCmp<did_t> >::iterator it2 = dimmap.find(dids);
	if (it2 == dimmap.end()) {
		did_t did = getDimID();
		dimmap[dids] = did;
		it2 = dimmap.find(dids);

		_idindex[bit][did] = &(it2->first);
	}
	return it2->second;
}
TL_Dimension::dim_id_t TL_Dimension::add(dim_bit_t bit, const vector<string> & dim) {
	int num = dim.size();
	if (calByteBits(bit) != num) {
		ostringstream os;
		os << "TL_Dimension bitmap is not match key count:" << bit << ":" << calBits(bit) << "!=" << num;
		throw TL_Exception(os.str(), -1);
	}

	map<dim_bit_t, map<vector<did_t>, did_t, TL_VecCmp<did_t> > >::iterator it = _dimmap.find(bit);
	if (it == _dimmap.end()) {
		_level_size = -1;
		_dimmap[bit];
		it = _dimmap.find(bit);
	}
	map<vector<did_t>, did_t, TL_VecCmp<did_t> > & dimmap = it->second;
	vector<dim_id_t> dids;
	_keys.add(dim, dids);
	map<vector<did_t>, did_t, TL_VecCmp<did_t> >::iterator it2 = dimmap.find(dids);
	if (it2 == dimmap.end()) {
		did_t did = getDimID();
		dimmap[dids] = did;
		it2 = dimmap.find(dids);

		_idindex[bit][did] = &(it2->first);
	}
	return it2->second;
}
TL_Dimension::dim_id_t TL_Dimension::add(const dim_struct & dim) {
	int num = dim.dims.size();
	if (calByteBits(dim.bit) != num) {
		ostringstream os;
		os << "TL_Dimension bitmap is not match key count:" << dim.bit << ":" << calBits(dim.bit) << "!=" << num;
		throw TL_Exception(os.str(), -1);
	}
	map<dim_bit_t, map<vector<did_t>, did_t, TL_VecCmp<did_t> > >::iterator it = _dimmap.find(dim.bit);
	if (it == _dimmap.end()) {
		_level_size = -1;
		_dimmap[dim.bit];
		it = _dimmap.find(dim.bit);
	}
	map<vector<did_t>, did_t, TL_VecCmp<did_t> > & dimmap = it->second;

	vector<dim_id_t> dids;
	_keys.add(dim.dims, dids);
	map<vector<did_t>, did_t, TL_VecCmp<did_t> >::iterator it2 = dimmap.find(dids);
	if (it2 == dimmap.end()) {
		did_t did = getDimID();
		dimmap[dids] = did;
		it2 = dimmap.find(dids);

		_idindex[dim.bit][did] = &(it2->first);
	}
	return it2->second;
}
//
TL_Dimension::dim_id_t TL_Dimension::get(const dim_struct & dim) {
	map<dim_bit_t, map<vector<did_t>, did_t, TL_VecCmp<did_t> > >::const_iterator it = _dimmap.find(dim.bit);
	if (it == _dimmap.end()) {
		return 0;
	}
	vector<dim_id_t> dids;
	_keys.get(dim.dims, dids);
	const map<vector<did_t>, did_t, TL_VecCmp<did_t> > & dimmap = it->second;
	map<vector<did_t>, did_t, TL_VecCmp<did_t> >::const_iterator it2 = dimmap.find(dids);
	if (it2 == dimmap.end()) {
		return 0;
	}
	return it2->second;
}

void TL_Dimension::get(dim_bit_t bit, vector<dim_id_t>& dims) {
	dims.clear();
	map<dim_bit_t, map<did_t, const vector<did_t>*> >::const_iterator it = _idindex.find(bit);
	if (it != _idindex.end()) {
		const map<did_t, const vector<did_t>*> & dit_v = it->second;
		dims.reserve(dit_v.size());
		map<did_t, const vector<did_t>*>::const_iterator it2 = dit_v.begin();
		while (it2 != dit_v.end()) {
			dims.push_back(it2->first);
		}
	}
}
void TL_Dimension::get(dim_bit_t bit, set<dim_id_t>& dims) {
	dims.clear();
	map<dim_bit_t, map<did_t, const vector<did_t>*> >::const_iterator it = _idindex.find(bit);
	if (it != _idindex.end()) {
		const map<did_t, const vector<did_t>*> & dit_v = it->second;
		//dims.reserve(dit_v.size());
		map<did_t, const vector<did_t>*>::const_iterator it2 = dit_v.begin();
		while (it2 != dit_v.end()) {
			dims.insert(it2->first);
		}
	}
}
void TL_Dimension::setHierarchyName(const string& hname) {
	_hierarchy = hname;
}
const string& TL_Dimension::getHierarchyName() {
	return _hierarchy;
}

void TL_Dimension::setLevelName(int level, const string& lname) {
	_levelnames.reserve(level);
	_levelnames[level] = lname;
}

const string& TL_Dimension::getLevelName(int level) {
	if (level < 0 || (int) _levelnames.size() < level) {
		static string empty;
		return empty;
	}
	return _levelnames[level];
}
TL_Dimension::dim_id_t TL_Dimension::getCounter() {
	return _did;
}



void TL_Dimension::display(ostream& os) {
	//just for test;
	//map<dim_bit_t, map<vector<did_t>, did_t, TL_VecCmp<did_t> > > _dimmap;
	map<dim_bit_t, map<vector<did_t>, did_t, TL_VecCmp<did_t> > >::const_iterator it = _dimmap.begin();
	while (it != _dimmap.end()) {
		const map<vector<did_t>, did_t, TL_VecCmp<did_t> >& dim_did_map = it->second;
		map<vector<did_t>, did_t, TL_VecCmp<did_t> >::const_iterator it2 = dim_did_map.begin();
		while (it2 != dim_did_map.end()) {
			os << "<" << it->first << "|";
			const vector<did_t>& v = it2->first;
			size_t s = v.size();
			for (size_t i = 0; i < s; ++i) {
				os << _keys[v[i]] << "(" << v[i] << ")" << "|";
			}
			os << ">";
			++it2;
		}
		++it;
	}
}
//private function
TL_Dimension::dim_id_t TL_Dimension::getDimID() {
	return ++_did;
}

}
/* namespace tidp */
