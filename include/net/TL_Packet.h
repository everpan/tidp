/*
 * TL_Packet.h
 *
 *  Created on: Dec 28, 2014
 *      Author: ever
 */

#ifndef INCLUDE_NET_TL_PACKET_H_
#define INCLUDE_NET_TL_PACKET_H_
#include <boost/shared_ptr.hpp>

namespace tidp {
namespace net {
class TL_Session;
typedef boost::shared_ptr<TL_Session> TL_SessionPtr;

class TL_Packet {
public:
	TL_Packet();
	virtual ~TL_Packet();
public:
	unsigned int cmdid;
	//当前的sessionid，因为session是重复利用的，有可能此session已经被其他人利用（session id 会不一样）
	//所以要check下是否是同一个链接。
	unsigned int sid;
	time_t recv_time;
	time_t send_time;
	std::string recv_data;
	std::string send_data;
	TL_SessionPtr session;
};
typedef boost::shared_ptr<TL_Packet> TL_PacketPtr;
} /* namespace net */
} /* namespace tidp */

#endif /* INCLUDE_NET_TL_PACKET_H_ */
