/*
 * TL_Metric.h
 *
 *  Created on: Oct 13, 2014
 *      Author: ever
 */

#ifndef TL_METRIC_H_
#define TL_METRIC_H_

#include <map>
#include <vector>
#include <stdlib.h>
#include <iostream>

namespace tidp {
using namespace std;
class TL_Metric {
public:
	typedef enum aggregation_type {
		SUM, COUNT, MIN, MAX, DISTINCT_COUNT
	} agg_type_t;

	typedef union {
		int i;
		long l;
		long long ll;
		float f;
		double d;
	} agg_value_t;

	typedef enum metric_type {
		Metric_INT, Metric_LONG, Metric_LONGLONG, Metric_FLOAT, Metric_DOUBLE
	} metric_type_t;

	typedef unsigned int dim_t;

	TL_Metric();
	TL_Metric(int size);
	virtual ~TL_Metric();
	agg_value_t& getAggValue(int pos);
	void reserve(int size);
	size_t size();
private:
	vector<agg_value_t> _metric_values;
};
//提供几个默认解析参数
typedef void (*TL_Metric_ParseFun_t)(const char * p, TL_Metric::agg_value_t *);
void TL_Metric_Parse_Int(const char * p, TL_Metric::agg_value_t * aggvalue);
void TL_Metric_Parse_Long(const char * p, TL_Metric::agg_value_t *aggvalue);
void TL_Metric_Parse_LongLong(const char * p, TL_Metric::agg_value_t *aggvalue);
void TL_Metric_Parse_Float(const char * p, TL_Metric::agg_value_t *aggvalue);
void TL_Metric_Parse_Double(const char * p, TL_Metric::agg_value_t *aggvalue);
} /* namespace tidp */

#endif /* TL_METRIC_H_ */
