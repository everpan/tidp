/*
 * TL_ExecuteThread.h
 *
 *  Created on: Dec 28, 2014
 *      Author: ever
 */

#ifndef INCLUDE_NET_TL_EXECUTETHREAD_H_
#define INCLUDE_NET_TL_EXECUTETHREAD_H_

#include <net/TL_Packet.h>
#include <TL_Logger.h>
#include <TL_Thread.h>
#include <boost/shared_ptr.hpp>
#include <net/TL_ExecuteFactory.h>
#include <map>
namespace tidp {
namespace net {
class TL_Server;
class TL_ExecuteThread : public TL_Thread{
public:
	TL_ExecuteThread();
	virtual ~TL_ExecuteThread();
	void setServer(TL_Server * server);
	void setExecuteFactory(TL_ExecuteFactoryPtr& factory);
	TL_ExecutePtr & getExecute(unsigned int cmdid);
	virtual void run();
private:
	std::map<unsigned int,TL_ExecutePtr> _executes;
	//vector<_executes>
	TL_Server * _server;
	TL_ExecuteFactoryPtr _execute_factory;
};
typedef boost::shared_ptr<TL_ExecuteThread> TL_ExecuteThreadPtr;
} /* namespace net */
} /* namespace tidp */

#endif /* INCLUDE_NET_TL_EXECUTETHREAD_H_ */
