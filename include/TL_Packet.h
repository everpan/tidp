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
#include <arpa/inet.h>
#include <boost/shared_ptr.hpp>
#include <iostream>
using namespace std;

#define DEFAULT_EXECUTE_COMMAND_ID ((unsigned) -1)

namespace tidp {
class TL_Server;
typedef boost::shared_ptr<class TL_Session> TL_SessionPtr;

struct TL_Packet {
	struct Head {
		unsigned int packMagic;
		unsigned int packLen;
		unsigned int packCmdId;
		unsigned int packSeqId;
		unsigned int packVersion;

		char * packData;
		Head() {
			clear();
		}
		void clear() {
			packMagic = htonl(0x02300230);
			packLen = 0;
			packCmdId = DEFAULT_EXECUTE_COMMAND_ID;
			packSeqId = 0;
			packVersion = 0x20141104;
			packData = NULL;
		}

		void toBuffer(char * p) {
			unsigned int *up = (unsigned int*) p;
			*up = htonl(packMagic);
			++up;
			*up = htonl(packLen);
			++up;
			*up = htonl(packCmdId);
			++up;
			*up = htonl(packSeqId);
			++up;
			*up = htonl(packVersion);
			packData = p + byteSize();
			//++up;
		}

		void fromBuffer(const char * p) {
			const unsigned int * iptr = (const unsigned int *) p;
			packMagic = ntohl(*iptr);
			++iptr;
			packLen = ntohl(*iptr);
			++iptr;
			packCmdId = ntohl(*iptr);
			++iptr;
			packSeqId = ntohl(*iptr);
			++iptr;
			packVersion = ntohl(*iptr);
			++iptr;
			packData = const_cast<char*>(p + byteSize());
		}
		static unsigned int byteSize() {
			return sizeof(int) * 5;
		}
		void toString() {
			cout << packMagic << "|" << packLen << "|" << packVersion << "|"
					<< packCmdId;
		}
	};
public:
	enum PackTyep {
		UNKNOWN = -1,
		LINE_STRING,
		TL_BINARY,
		HTTP_GET,
		HTTP_POST,
		HTTP_HEAD,
		PROXY4,
		PROXY5
	};
	TL_Packet();
	~TL_Packet();
	PackTyep protocol_type;
	//unsigned int command_id;
	//unsigned int message_id;
	Head head;
	time_t recv_time;
	time_t send_time;

	string recv_data;
	string send_data;
	TL_Server * server;
	TL_SessionPtr session;
	void * extraData;
	void clear();
	int send();
	void closeSession();
	int sendWithHead();
};
typedef boost::shared_ptr<TL_Packet> TL_PacketPtr;
}
#endif /* TSCHEDULEPACKET_H_ */
