/*
 * TL_Execute.h
 *
 *  Created on: Dec 28, 2014
 *      Author: ever
 */

#ifndef SRC_NET_TL_EXECUTE_H_
#define SRC_NET_TL_EXECUTE_H_
#include <net/TL_Packet.h>
#include <net/TL_Session.h>
#include <TL_Logger.h>
#include <memory>
namespace tidp {
namespace net {
class TL_Execute {
public:
	TL_Execute();
	virtual ~TL_Execute();
	virtual void execute(TL_PacketPtr& packet);
};
typedef std::shared_ptr<TL_Execute> TL_ExecutePtr;
} /* namespace net */
} /* namespace tidp */

#endif /* SRC_NET_TL_EXECUTE_H_ */
