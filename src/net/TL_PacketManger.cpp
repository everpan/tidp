/*
 * TL_PacketManger.cpp
 *
 *  Created on: Dec 28, 2014
 *      Author: ever
 */

#include <net/TL_PacketManger.h>

namespace tidp {
namespace net {

TL_PacketManger::TL_PacketManger() {
	// TODO Auto-generated constructor stub
	_release_fun = boost::bind(&TL_PacketManger::releaseRawPtr, this, _1);
}

TL_PacketManger::~TL_PacketManger() {
	// TODO Auto-generated destructor stub
}
TL_PacketPtr TL_PacketManger::getPacketPtr() {
	Lock lk(*this);
	if (_idle.size() == 0) {
		TL_PacketPtr sp(new TL_Packet, _release_fun);
		return sp;
	} else {
		std::set<TL_Packet *>::iterator it = _idle.begin();
		TL_Packet * p = *it;
		TL_PacketPtr sp(p, _release_fun);
		_idle.erase(it);
		return sp;
	}
}

void TL_PacketManger::release(TL_PacketPtr& pack) {
	Lock lk(*this);
}

void TL_PacketManger::releaseRawPtr(TL_Packet * pt) {
	Lock lk(*this);
	pt->recv_data.clear();
	pt->send_data.clear();
	pt->cmdid = 0;
	pt->sid = 0;
	pt->session.reset();
	_idle.insert(pt);
}
} /* namespace net */
} /* namespace tidp */
