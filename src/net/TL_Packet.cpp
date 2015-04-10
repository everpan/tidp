/*
 * TL_Packet.cpp
 *
 *  Created on: Dec 28, 2014
 *      Author: ever
 */

#include <net/TL_Packet.h>

namespace tidp {
namespace net {

TL_Packet::TL_Packet() {
	// TODO Auto-generated constructor stub
	sid = 0;
	cmdid = 0;
	recv_time = 0;
	send_time = 0;
}

TL_Packet::~TL_Packet() {
	// TODO Auto-generated destructor stub
}

} /* namespace net */
} /* namespace tidp */
