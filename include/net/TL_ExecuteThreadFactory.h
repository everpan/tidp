/*
 * TL_ExecuteThreadFactory.h
 *
 *  Created on: Dec 29, 2014
 *      Author: ever
 */

#ifndef INCLUDE_NET_TL_EXECUTETHREADFACTORY_H_
#define INCLUDE_NET_TL_EXECUTETHREADFACTORY_H_
#include <boost/shared_ptr.hpp>
#include <net/TL_ExecuteThread.h>
#include <net/TL_ExecuteFactory.h>
namespace tidp {
namespace net {

class TL_ExecuteThreadFactory {
public:
	TL_ExecuteThreadFactory();
	virtual ~TL_ExecuteThreadFactory();
	void setExecuteFactory(const TL_ExecuteFactoryPtr & factory);
	virtual TL_ExecuteThreadPtr create();
private:
	TL_ExecuteFactoryPtr _execute_factory;
};
typedef boost::shared_ptr<TL_ExecuteThreadFactory> TL_ExecuteThreadFactoryPtr;
} /* namespace net */
} /* namespace tidp */

#endif /* INCLUDE_NET_TL_EXECUTETHREADFACTORY_H_ */
