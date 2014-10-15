/*
 * TL_MultiDimension.h
 *
 *  Created on: Oct 13, 2014
 *      Author: ever
 */

#ifndef TL_MULTIDIMENSION_H_
#define TL_MULTIDIMENSION_H_

#include "TL_Dimension.h"
#include "TL_Metric.h"
#include "TL_Exp.h"
#include <boost/shared_ptr.hpp>
namespace tidp {

class TL_MultiDimension {
public:
	typedef unsigned int mid_t;
	typedef unsigned int mid_bit_t;
	typedef TL_Dimension::dim_t dim_t;

	typedef boost::shared_ptr<TL_Dimension> TL_DimensionPtr;
	typedef boost::shared_ptr<TL_Metric> TL_MetricPtr;
	TL_MultiDimension();
	virtual ~TL_MultiDimension();



	void setDimensionMetricWidth(const vector<int>& ws);
	void setDimensionBitmap(const vector<int> &bm);
	void replace(const char ** p,int len);
	void replace(char *bitmap, int bitmapByteLen, const char **p,int len);

	void clear();

private:
	mid_t getMID();
	void createDimension(int dim_num);
	//void create
private:
	char * _bitmap;
	char * _update_bitmap;
	mid_t _mid;

	int _metric_num;
	vector<int> _dim_metric_width; //all,d1...dn,m
	vector<TL_DimensionPtr> _dimensions;

	map<mid_bit_t, map<vector<dim_t>, TL_MetricPtr, TL_VecCmp<dim_t> > > _cube;
};

} /* namespace tidp */

#endif /* TL_MULTIDIMENSION_H_ */
