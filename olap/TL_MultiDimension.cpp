/*
 * TL_MultiDimension.cpp
 *
 *  Created on: Oct 13, 2014
 *      Author: ever
 */

#include "TL_MultiDimension.h"

namespace tidp {

TL_MultiDimension::TL_MultiDimension() {
	_max_dimention_num = 0;
	_metric_num = 0;
	_mid = 0;
}

TL_MultiDimension::~TL_MultiDimension() {
	// TODO Auto-generated destructor stub
}

//设定维度个数和度量个数，其中最后一个为度量的个数
void TL_MultiDimension::setDimensionMetricWidth(const vector<int>& ws) {
	if (ws.size() < 1) {
		throw TL_Exception("TL_MultiDimension dimension/metric width is error.", -1);
	}

	if (_dim_metric_width.size() > 0) {
		throw TL_Exception("TL_MultiDimension is not empty,Please use clear() to reset before call this function.", -1);
	}

	for (int i = 0; i < (int) ws.size(); ++i) {
		int w = ws[i];
		if (w < 0) {
			throw TL_Exception("TL_MultiDimension dimension/metric width must be positive number.", -1);
		}
	}
	_dim_metric_width = ws;
	_metric_num = *ws.rbegin();
	createDimension(ws.size() - 1);
}

void TL_MultiDimension::setMetricType(const vector<TL_Metric::metric_type_t>& mt) {
	if (_metric_num == 0 || (int) mt.size() != _metric_num) {
		ostringstream os;
		os << "TL_MultiDimension metric_num = " << _metric_num << ",metric_type_size = " << mt.size()
				<< " all must be grate than zero and equal";
		throw TL_Exception(os.str(), -1);
	}
	_metric_parse_funs.clear();
	for (int i = 0; i < _metric_num; ++i) {
		switch (mt[i]) {
			case TL_Metric::Metric_INT:
				_metric_parse_funs.push_back(TL_Metric_Parse_Int);
				break;
			case TL_Metric::Metric_LONG:
				_metric_parse_funs.push_back(TL_Metric_Parse_Long);
				break;
			case TL_Metric::Metric_LONGLONG:
				_metric_parse_funs.push_back(TL_Metric_Parse_LongLong);
				break;
			case TL_Metric::Metric_FLOAT:
				_metric_parse_funs.push_back(TL_Metric_Parse_Float);
				break;
			case TL_Metric::Metric_DOUBLE:
				_metric_parse_funs.push_back(TL_Metric_Parse_Double);
				break;
			default:
				throw TL_Exception("TL_MultiDimension unknown metric type", -1);
		}
	}
}
void TL_MultiDimension::setDimensionMetricWidthType(const vector<int>& ws, const vector<TL_Metric::metric_type_t>& mt) {
	setDimensionMetricWidth(ws);
	setMetricType(mt);
}

void TL_MultiDimension::setMetricParseFunction(const vector<TL_Metric_ParseFun_t>& mf) {
	if (_metric_num == 0 || (int) mf.size() != _metric_num) {
		ostringstream os;
		os << "TL_MultiDimension metric_num = " << _metric_num << ",metric_fun_size = " << mf.size()
				<< " all must be grate than zero and equal";
		throw TL_Exception(os.str(), -1);
	}
	_metric_parse_funs = mf;
}

TL_MultiDimension::DefinedPtr TL_MultiDimension::getDefined(const vector<int> &dim_bitmap) {
	if ((int) dim_bitmap.size() > _max_dimention_num) {
		throw TL_Exception("TL_MultiDimension bitmap size is excess", -1);
	}
	DefinedPtr ptr(new Defined());
	ptr->_positions_count_levelbitmap.clear();
	int pos = 0;
	int bnum = 0;
	ptr->_positions_count_levelbitmap.push_back(pos); //位置偏移
	ptr->_dimention_combination_bitmap = 0;
	for (int i = 0; i < (int) dim_bitmap.size(); ++i) {
		bnum = calBits(dim_bitmap[i]);
		if (bnum > 0) {
			ptr->_dimention_combination_bitmap |= (1 << i);
		}
		if (bnum > _dim_metric_width[i]) {
			//宽度超限制
			ptr->_positions_count_levelbitmap.clear();
			ostringstream os;
			os << "TL_MultiDimension dimension[" << i << "] width=" << _dim_metric_width[i] << ", setBitmap width="
					<< bnum << " is excess";
			throw TL_Exception(os.str(), -1);
		}
		ptr->_positions_count_levelbitmap.push_back(dim_bitmap[i]); //bitmap
		ptr->_positions_count_levelbitmap.push_back(bnum); //个数
		ptr->_positions_count_levelbitmap.push_back(pos += bnum); //下一个位置偏移,最后一个为度量位置偏移
	}
	//随着接口变化。
	ptr->_dimention_count = dim_bitmap.size();
	//_set_dimension_bitmap = bm;//
	//度量值不可空缺
	ptr->_positions_count_levelbitmap.push_back(_metric_num);
	return ptr;
}
/*
 void TL_MultiDimension::replace(const char ** p, int len) {
 //todo check _max_dimention_num ?
 if (_dimention_num == 0) {
 throw TL_Exception("TL_MultiDimension did you forget to call setDimensionBitmap()", -1);
 }

 if ((int) _metric_parse_funs.size() != _metric_num) {
 throw TL_Exception("TL_MultiDimension did you forget to call setMetricParseFunction() or setMetricType()", -1);
 }
 _dims_tmp.clear();
 _dims_tmp.reserve(_dimention_num);
 register int i = 0;
 register int dnum = _dimention_num * 3;
 int * posp;
 for (; i < dnum; i += 3) {
 if (_dim_positions[i + 1] > 0) {	//bitmap
 TL_DimensionPtr & ptr = _dimensions[i];
 // p + _dim_positions[i] ??????
 posp = &_dim_positions[i];
 //_dims_tmp.push_back(ptr->add(_dim_positions[i + 1], p + _dim_positions[i], _dim_positions[i + 2]));
 _dims_tmp.push_back(ptr->add(*(posp + 1), p + *posp, *(posp + 2)));
 }
 }

 //map<mid_bit_t, map<vector<dim_t>, TL_MetricPtr, TL_VecCmp<dim_t> > > _cube;
 //维度组合
 map<vector<dim_t>, TL_MetricPtr, TL_VecCmp<dim_t> >& tmp = _cube[_dimention_bitmap];
 map<vector<dim_t>, TL_MetricPtr, TL_VecCmp<dim_t> >::iterator it = tmp.find(_dims_tmp);
 if (it == tmp.end()) {
 tmp[_dims_tmp] = TL_MetricPtr(new TL_Metric(_metric_num));
 it = tmp.find(_dims_tmp);
 }
 TL_MetricPtr & pt = it->second;
 int metric_pos = _dim_positions[dnum];
 for (int i = 0; i < _metric_num; ++i) {
 _metric_parse_funs[i](p[metric_pos + i], &pt->getAggValue(i));
 }
 }
 */
void TL_MultiDimension::replace(const DefinedPtr& defptr, const char ** p, int len) {
	const Defined& def = *defptr.get();
	if (def._dimention_count == 0) {
		throw TL_Exception("TL_MultiDimension did you forget to call setDimensionBitmap()", -1);
	}

	if ((int) _metric_parse_funs.size() != _metric_num) {
		throw TL_Exception("TL_MultiDimension did you forget to call setMetricParseFunction() or setMetricType()", -1);
	}
	_dims_tmp.clear();
	_dims_tmp.reserve(def._dimention_count);
	int i = 0;
	int dnum = def._dimention_count * 3;
	const int * posp;
	for (; i < dnum; i += 3) {
		if (def._positions_count_levelbitmap[i + 1] > 0) {	//bitmap
			TL_DimensionPtr & ptr = _dimensions[i];
			posp = &def._positions_count_levelbitmap[i];
			_dims_tmp.push_back(ptr->add(posp[1], p + posp[0], posp[2]));
		}
	}

	//map<mid_bit_t, map<vector<dim_t>, TL_MetricPtr, TL_VecCmp<dim_t> > > _cube;
	//维度组合
	map<vector<dim_t>, TL_MetricPtr, TL_VecCmp<dim_t> >& tmp = _cube[def._dimention_combination_bitmap];
	map<vector<dim_t>, TL_MetricPtr, TL_VecCmp<dim_t> >::iterator it = tmp.find(_dims_tmp);
	if (it == tmp.end()) {
		tmp[_dims_tmp] = TL_MetricPtr(new TL_Metric(_metric_num));
		it = tmp.find(_dims_tmp);
	}
	TL_MetricPtr & pt = it->second;
	int metric_pos = def._positions_count_levelbitmap[dnum];
	for (int i = 0; i < _metric_num; ++i) {
		_metric_parse_funs[i](p[metric_pos + i], &pt->getAggValue(i));
	}
}
void TL_MultiDimension::replace(char *bitmap, int bitmapByteLen, const char **p, int len) {

}
/*
 const TL_MetricPtr& TL_MultiDimension::get(const char **p, int len) {
 if (_dimention_num == 0) {
 throw TL_Exception("TL_MultiDimension did you forget to call setDimensionBitmap()", -1);
 }

 if ((int) _metric_parse_funs.size() != _metric_num) {
 throw TL_Exception("TL_MultiDimension did you forget to call setMetricParseFunction() or setMetricType()", -1);
 }
 _dims_tmp.clear();
 _dims_tmp.reserve(_dimention_num);

 for (int i = 0; i < _dimention_num * 3; i += 3) {
 if (_dim_positions[i + 1] > 0) {	//bitmap
 TL_DimensionPtr & ptr = _dimensions[i];
 // p + _dim_positions[i] ??????
 _dims_tmp.push_back(ptr->add(_dim_positions[i + 1], p + _dim_positions[i], _dim_positions[i + 2]));
 }
 }
 map<vector<dim_t>, TL_MetricPtr, TL_VecCmp<dim_t> >& tmp = _cube[_dimention_bitmap];
 map<vector<dim_t>, TL_MetricPtr, TL_VecCmp<dim_t> >::iterator it = tmp.find(_dims_tmp);
 if (it != tmp.end()) {
 return it->second;
 }
 static TL_MetricPtr empty;
 return empty;
 }
 */
const TL_MetricPtr& TL_MultiDimension::get(const DefinedPtr& defptr, const char ** p, int len) {
	const Defined& def = *defptr.get();
	if (def._dimention_count == 0) {
		throw TL_Exception("TL_MultiDimension did you forget to call setDimensionBitmap()", -1);
	}

	if ((int) _metric_parse_funs.size() != _metric_num) {
		throw TL_Exception("TL_MultiDimension did you forget to call setMetricParseFunction() or setMetricType()", -1);
	}
	_dims_tmp.clear();
	_dims_tmp.reserve(def._dimention_count);
	register int i = 0;
	register int dnum = def._dimention_count * 3;
	const int * posp;
	for (; i < dnum; i += 3) {
		if (def._positions_count_levelbitmap[i + 1] > 0) {	//bitmap
			TL_DimensionPtr & ptr = _dimensions[i];
			// p + _dim_positions[i] ??????
			posp = &def._positions_count_levelbitmap[i];
			//_dims_tmp.push_back(ptr->add(_dim_positions[i + 1], p + _dim_positions[i], _dim_positions[i + 2]));
			_dims_tmp.push_back(ptr->add(posp[1], p + posp[0], posp[2]));
		}
	}

	//map<mid_bit_t, map<vector<dim_t>, TL_MetricPtr, TL_VecCmp<dim_t> > > _cube;
	//维度组合
	map<vector<dim_t>, TL_MetricPtr, TL_VecCmp<dim_t> >& tmp = _cube[def._dimention_combination_bitmap];
	map<vector<dim_t>, TL_MetricPtr, TL_VecCmp<dim_t> >::iterator it = tmp.find(_dims_tmp);
	if (it != tmp.end()) {
		return it->second;
	}
	static TL_MetricPtr empty;
	return empty;
}
void TL_MultiDimension::clear() {
	//_dimention_num = 0;
	_metric_num = 0;
	_dim_metric_width.clear();
	_dimensions.clear();
	_metric_parse_funs.clear();
}

//private function
TL_MultiDimension::mid_t TL_MultiDimension::getMID() {
	return ++_mid;
}

void TL_MultiDimension::createDimension(int dim_num) {
	_dimensions.clear();
	for (int i = 0; i < dim_num; ++i) {
		_dimensions.push_back(TL_DimensionPtr(new TL_Dimension()));
	}
	_max_dimention_num = dim_num;
}

} /* namespace tidp */
