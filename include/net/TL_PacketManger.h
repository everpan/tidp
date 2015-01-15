/*
 * TL_PacketManger.h
 *
 *  Created on: Dec 28, 2014
 *      Author: ever
 */

#ifndef INCLUDE_NET_TL_PACKETMANGER_H_
#define INCLUDE_NET_TL_PACKETMANGER_H_
#include <set>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <net/TL_Packet.h>
#include <TL_ThreadLock.h>

namespace tidp {
namespace net {
typedef boost::function<void(TL_Packet*)> packetReleaseFun;
class TL_PacketManger : public TL_ThreadLock{
public:
	TL_PacketManger();
	virtual ~TL_PacketManger();

	TL_PacketPtr getPacketPtr();
	void release(TL_PacketPtr& pack);
private:
	void releaseRawPtr(TL_Packet * pt);
	packetReleaseFun _release_fun;
	std::set<TL_Packet * > _idle;

};

} /* namespace net */
} /* namespace tidp */

#endif /* INCLUDE_NET_TL_PACKETMANGER_H_ */
