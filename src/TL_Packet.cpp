/*
 * TL_Packet.cpp
 *
 *  Created on: 2014年7月30日
 *      Author: ever
 */

#include "TL_Packet.h"
#include "TL_Session2.h"
#ifndef RESERVE_SIZE
#define RESERVE_SIZE 1024
#endif
namespace tidp {

TL_Packet::TL_Packet() {
	protocol_type = UNKNOWN;
	command_id = (unsigned int) -1;
	message_id = 0;
	recv_time = 0;
	send_time = 0;
	server = NULL;
	extraData = NULL;
	recv_data.reserve(RESERVE_SIZE);
	send_data.reserve(RESERVE_SIZE);
}

TL_Packet::~TL_Packet() {
	// TODO Auto-generated destructor stub
}

void TL_Packet::clear() {
	protocol_type = UNKNOWN;
	message_id = 0;
	recv_time = send_time = 0;
	session.reset();
	recv_data.clear();
	send_data.clear();
}
int TL_Packet::send() {
	return session->send(send_data);
}
}

