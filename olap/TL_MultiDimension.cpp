/*
 * TL_MultiDimension.cpp
 *
 *  Created on: Oct 13, 2014
 *      Author: ever
 */

#include "TL_MultiDimension.h"

namespace tidp {

TL_MultiDimension::TL_MultiDimension() {
	_mid = 0;
}

TL_MultiDimension::~TL_MultiDimension() {
	// TODO Auto-generated destructor stub
}

//设定维度个数和度量个数，其中最后一个为度量的个数
void TL_MultiDimension::setDimensionMetricWidth(const vector<int>& ws) {
	if (ws.size() < 1) {
		throw TL_Exception("MultiDimension dimension/metric width is error.", -1);
	}

	if (_dim_metric_width.size() > 0) {
		throw TL_Exception("MultiDimension is not empty,Please use clear() to reset before call this function.", -1);
	}

	for (int i = 0; i < (int) ws.size(); ++i) {
		int w = ws[i];
		if (w < 0) {
			throw TL_Exception("MultiDimension dimension/metric width must be positive number.", -1);
		}
	}
	_dim_metric_width = ws;
	_metric_num = *ws.rbegin();
	createDimension(ws.size() - 1);
}
//设定在更新的过程中，字段解析位置位图
void TL_MultiDimension::setDimensionBitmap(const vector<int> &bm) {

}
void TL_MultiDimension::replace(const char ** p, int len) {

}
void TL_MultiDimension::replace(char *bitmap, int bitmapByteLen, const char **p, int len) {

}

void TL_MultiDimension::clear() {
	_metric_num = 0;
	_dim_metric_width.clear();
	_dimensions.clear();
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
}

} /* namespace tidp */
