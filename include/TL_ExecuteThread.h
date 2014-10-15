/*
 * TL_ExecuteThread.h
 *
 *  Created on: Aug 10, 2014
 *      Author: ever
 */

#ifndef TSEXECUTETHREAD_H_
#define TSEXECUTETHREAD_H_
#include <map>
#include "TL_Thread.h"

#include "TL_Packet.h"
#include "TL_ExecuteFactory.h"
#include "TL_Server.h"
using namespace std;
namespace tidp {
class TL_ExecuteThread: public tidp::TL_Thread {
public:
	TL_ExecuteThread();
	virtual ~TL_ExecuteThread();
	virtual void run();
	void setServer(TL_Server* server);
private:
	TL_Server * _server;
	TL_PacketPtr _packet;
	map<unsigned int, TL_ExecutePtr> _cmd_cache;
};
}
#endif /* TSEXECUTETHREAD_H_ */
