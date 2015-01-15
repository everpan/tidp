/*
 * TL_ExecuteFactory.cpp
 *
 *  Created on: Dec 28, 2014
 *      Author: ever
 */

#include <net/TL_ExecuteFactory.h>
namespace tidp {
namespace net {

TL_ExecuteFactory::TL_ExecuteFactory() {
	// TODO Auto-generated constructor stub

}

TL_ExecuteFactory::~TL_ExecuteFactory() {
	// TODO Auto-generated destructor stub
}
TL_ExecutePtr TL_ExecuteFactory::create(unsigned int cmdid){
	return TL_ExecutePtr(new TL_Execute);
}

} /* namespace net */
} /* namespace tidp */
