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

typedef boost::shared_ptr<TL_Dimension> TL_DimensionPtr;
typedef boost::shared_ptr<TL_Metric> TL_MetricPtr;

class TL_MultiDimension {
public:
	typedef unsigned int mid_t;
	typedef unsigned int mid_bit_t;
	typedef TL_Dimension::did_t dim_t;

	class Defined {
		friend class TL_MultiDimension;
	protected:
		int _dimention_combination_bitmap; //维度组合bitmap
		int _dimention_count; //包含维度个数，不能大于cube中的定义个数
		vector<int> _positions_count_levelbitmap; //位置|个数|维度级别bitmap,交替出现
	};
	typedef boost::shared_ptr<Defined> DefinedPtr;
	TL_MultiDimension();
	virtual ~TL_MultiDimension();

	void setDimensionMetricWidth(const vector<int>& ws);
	void setMetricType(const vector<TL_Metric::metric_type_t>& mt);
	void setDimensionMetricWidthType(const vector<int>& ws, const vector<TL_Metric::metric_type_t>& mt);

	void setMetricParseFunction(const vector<TL_Metric_ParseFun_t>& mf);
	//void setDimensionBitmap(const vector<int> &bm);

	DefinedPtr getDefined(const vector<int> &dim_bitmap);

	//void replace(const char ** p, int len);
	void replace(const DefinedPtr& def, const char ** p, int len);
	void replace(char *bitmap, int bitmapByteLen, const char **p, int len);

	//const TL_MetricPtr& get(const char **p, int len);
	const TL_MetricPtr& get(const DefinedPtr& def,const char **p, int len);
	void clear();

private:
	mid_t getMID();
	void createDimension(int dim_num);
	//void create
private:
	char * _bitmap;
	char * _update_bitmap;
	mid_t _mid;
	int _max_dimention_num;
	//int _dimention_num; //调用接口时候的维度个数
	//int _dimention_bitmap; //调用接口时候的维度组合bitmap
	int _metric_num; //度量个数

	vector<TL_Metric_ParseFun_t> _metric_parse_funs;
	vector<int> _dim_metric_width; //all,d1...dn,m
	//vector<int> _set_dimension_bitmap;
	vector<TL_DimensionPtr> _dimensions;
	//vector<int> _dim_positions; //位置和个数交替
	map<mid_bit_t, map<vector<dim_t>, TL_MetricPtr, TL_VecCmp<dim_t> > > _cube;
	vector<dim_t> _dims_tmp; //更新临时变量
};

} /* namespace tidp */

#endif /* TL_MULTIDIMENSION_H_ */
