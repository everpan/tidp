/*
 * TL_ExecuteThreadFactory.cpp
 *
 *  Created on: Dec 29, 2014
 *      Author: ever
 */

#include <net/TL_ExecuteThreadFactory.h>

namespace tidp {
namespace net {

TL_ExecuteThreadFactory::TL_ExecuteThreadFactory() : _execute_factory(new TL_ExecuteFactory) {
	// TODO Auto-generated constructor stub

}

TL_ExecuteThreadFactory::~TL_ExecuteThreadFactory() {
	// TODO Auto-generated destructor stub
}
void TL_ExecuteThreadFactory::setExecuteFactory(const TL_ExecuteFactoryPtr & factory) {
	_execute_factory = factory;
}
TL_ExecuteThreadPtr TL_ExecuteThreadFactory::create() {
	TL_ExecuteThreadPtr sp(new TL_ExecuteThread);
	if(_execute_factory){
		sp->setExecuteFactory(_execute_factory);
	}
	return sp;
}
} /* namespace net */
} /* namespace tidp */
