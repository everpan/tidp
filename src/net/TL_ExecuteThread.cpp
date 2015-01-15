/*
 * TL_ExecuteThread.cpp
 *
 *  Created on: Dec 28, 2014
 *      Author: ever
 */

#include <net/TL_ExecuteThread.h>
#include <net/TL_Server.h>
namespace tidp {
namespace net {

TL_ExecuteThread::TL_ExecuteThread() :
		_server(NULL)/*, _execute_factory(new TL_ExecuteFactory())*/{
}

TL_ExecuteThread::~TL_ExecuteThread() {
}
void TL_ExecuteThread::setServer(TL_Server * server) {
	_server = server;
}
TL_ExecutePtr& TL_ExecuteThread::getExecute(unsigned int cmdid) {
	std::map<unsigned int, TL_ExecutePtr>::iterator it = _executes.find(cmdid);
	if (it == _executes.end()) {
		if(!_execute_factory){
			throw TL_Exception("unset TL_ExecuteFactory!!!",-1);
		}
		TL_ExecutePtr esp = _execute_factory->create(cmdid);
		_executes.insert(std::make_pair(cmdid, esp));
		it = _executes.find(cmdid);
		LOG() << debug << "new execute for " << id() << " cmd=" << cmdid << " " << esp.get() << endl;
	}
	return it->second;
}
void TL_ExecuteThread::setExecuteFactory(TL_ExecuteFactoryPtr& factory) {
	_execute_factory = factory;
}
void TL_ExecuteThread::run() {
	_running = 1;
	TL_PacketPtr packet;
	while (_running) {
		if (_server->getRequest(packet)) {
			TL_ExecutePtr& pt = getExecute(packet->cmdid);
			//LOG() << debug << packet->sid << "|request|" << packet->recv_data;
			pt->execute(packet);
			packet.reset();
		}
	}
}
} /* namespace net */
} /* namespace tidp */
