/*
 * TL_ExecuteThread.cpp
 *
 *  Created on: Aug 10, 2014
 *      Author: ever
 */

#include "TL_ExecuteThread.h"
namespace tidp {
TL_ExecuteThread::TL_ExecuteThread() {
	// TODO Auto-generated constructor stub
	_server = NULL;
}

TL_ExecuteThread::~TL_ExecuteThread() {
	// TODO Auto-generated destructor stub
}

void TL_ExecuteThread::run() {
	map<unsigned int, TL_ExecutePtr>::iterator it;
	while (_running) {
		try {
			//从server中获取数据包
			_server->getRequest(_packet);
			it = _cmd_cache.find(_packet->head.packCmdId);
			if (it == _cmd_cache.end()) {
				TL_ExecutePtr executePtr = _server->getExecute(_packet->head.packCmdId);//TL_ExecutePtr(TL_ExecuteFactory::create(_packet->command_id));

				_cmd_cache[_packet->head.packCmdId] = executePtr;
				it = _cmd_cache.find(_packet->head.packCmdId);
			}
			TL_ExecutePtr & executePtr = it->second;
			executePtr->execute(_packet);
			_packet.reset();
		} catch (const exception & e) {
			cout << e.what() << endl;
		}
	}
}

void TL_ExecuteThread::setServer(TL_Server* server) {
	_server = server;
}
}
