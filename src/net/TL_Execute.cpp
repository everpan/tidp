/*
 * TL_Execute.cpp
 *
 *  Created on: Dec 28, 2014
 *      Author: ever
 */

#include <net/TL_Execute.h>

namespace tidp {
namespace net {

TL_Execute::TL_Execute() {
	// TODO Auto-generated constructor stub

}

TL_Execute::~TL_Execute() {
	// TODO Auto-generated destructor stub
}
void TL_Execute::execute(TL_PacketPtr& packet){
	//just echo data
	//LOG() << debug << packet->session->getId() << endl;
	packet->session->send(packet->recv_data.c_str(),packet->recv_data.size());
}
} /* namespace net */
} /* namespace tidp */
