/*
 * TL_ExecuteFactory.h
 *
 *  Created on: Aug 12, 2014
 *      Author: ever
 */
/*
 *
 * */
#ifndef TSEXECUTEFACTORY_H_
#define TSEXECUTEFACTORY_H_
#include "TL_Execute.h"

namespace tidp {
#define DEFAULT_EXECUTE_COMMAND_ID ((unsigned) -1)
class TL_Execute;
class TL_ExecuteFactory {
public:
	TL_ExecuteFactory();
	virtual ~TL_ExecuteFactory();
	virtual TL_ExecutePtr create(unsigned command);
};
typedef boost::shared_ptr<TL_ExecuteFactory> TL_ExecuteFactoryPtr;
}
#endif /* TSEXECUTEFACTORY_H_ */
