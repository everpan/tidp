/*
 * TL_Packet.h
 *
 *  Created on: 2014年7月30日
 *      Author: ever
 */

#ifndef TSCHEDULEPACKET_H_
#define TSCHEDULEPACKET_H_
#include <string>
#include <time.h>
#include <boost/shared_ptr.hpp>
using namespace std;
namespace tidp {
class TL_Server;
typedef boost::shared_ptr<class TL_Session2> TL_SessionPtr;

struct TL_Packet {

public:
	enum PackTyep {
		UNKNOWN = -1, LINE_STRING, TL_BINARY, HTTP_GET, HTTP_POST, HTTP_HEAD, PROXY4, PROXY5
	};
	TL_Packet();
	~TL_Packet();
	PackTyep protocol_type;
	unsigned int command_id;
	unsigned int message_id;
	time_t recv_time;
	time_t send_time;

	string recv_data;
	string send_data;
	TL_Server * server;
	TL_SessionPtr session;
	void * extraData;
	void clear();
	int send();
};
typedef boost::shared_ptr<TL_Packet> TL_PacketPtr;
}
#endif /* TSCHEDULEPACKET_H_ */
