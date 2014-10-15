/*
 * TL_Cube.h
 *
 *  Created on: Oct 13, 2014
 *      Author: ever
 */

#ifndef TL_CUBE_H_
#define TL_CUBE_H_

#include <string>
#include <map>
#include "TL_Dimension.h"
#include "TL_Metric.h"

namespace tidp {
using namespace std;
class TL_Cube {
public:
	TL_Cube();
	virtual ~TL_Cube();
private:
	string _cube_name;
	map<TL_Dimension::dim_id_t, TL_Metric> _dim_metric;
};

} /* namespace tidp */

#endif /* TL_CUBE_H_ */
