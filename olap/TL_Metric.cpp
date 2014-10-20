/*
 * TL_Metric.cpp
 *
 *  Created on: Oct 13, 2014
 *      Author: ever
 */

#include "TL_Metric.h"

namespace tidp {

TL_Metric::TL_Metric() {
}
TL_Metric::TL_Metric(int size) {
	reserve(size);
}
TL_Metric::~TL_Metric() {
	// TODO Auto-generated destructor stub
}

TL_Metric::agg_value_t& TL_Metric::getAggValue(int pos) {
	return _metric_values[pos];
	return _metric_values.at(pos);
}
void TL_Metric::reserve(int size) {
	_metric_values.reserve(size);
}
size_t TL_Metric::size() {
	return _metric_values.size();
}


void TL_Metric_Parse_Int(const char * p, TL_Metric::agg_value_t * aggvalue) {
	aggvalue->i = strtol(p, NULL, 10);
	//cout << "TL_Metric_Parse_Int:" << aggvalue->i << " (" << p << ")"<< endl;
}
void TL_Metric_Parse_Long(const char * p, TL_Metric::agg_value_t *aggvalue) {
	aggvalue->l = strtol(p, NULL, 10);
}
void TL_Metric_Parse_LongLong(const char * p, TL_Metric::agg_value_t *aggvalue) {
	aggvalue->ll = strtoll(p, NULL, 10);
}
void TL_Metric_Parse_Float(const char * p, TL_Metric::agg_value_t *aggvalue) {
	aggvalue->f = strtod(p, NULL);
}
void TL_Metric_Parse_Double(const char * p, TL_Metric::agg_value_t *aggvalue) {
	aggvalue->d = strtol(p, NULL, 10);
}
} /* namespace tidp */
