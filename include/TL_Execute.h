/*
 * TL_Execute.h
 *
 *  Created on: Jul 17, 2014
 *      Author: ever
 */

#ifndef TSEXECUTE_H_
#define TSEXECUTE_H_
#include <boost/shared_ptr.hpp>
#include "TL_Packet.h"

//#include "TL_Server.h"

//#include "TL_Cgi.h"
namespace tidp {
//default TL_Execute is echo.
class TL_Execute {
public:
	TL_Execute();
	virtual ~TL_Execute();
	virtual void execute(TL_PacketPtr& packet);
	static TL_Execute* create();
};
typedef boost::shared_ptr<TL_Execute> TL_ExecutePtr;

}
#endif /* TSEXECUTE_H_ */
