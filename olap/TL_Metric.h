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

namespace tidp {
using namespace std;
class TL_Metric {
public:
	typedef enum aggregation_type {
		SUM, COUNT, MIN, MAX, DISTINCT_COUNT
	} agg_type;
	typedef union {
		int i;
		float f;
		double d;
	} agg_value_t;

	typedef unsigned int dim_t;
	TL_Metric();
	virtual ~TL_Metric();
private:
	//map<dim_t, vector<int> > _metrics;
	vector<agg_value_t> _metrics_values;
};

} /* namespace tidp */

#endif /* TL_METRIC_H_ */
