/*
 * TL_Execute.cpp
 *
 *  Created on: Jul 17, 2014
 *      Author: ever
 */

#include "TL_Execute.h"
namespace tidp{
TL_Execute::TL_Execute() {
	// TODO Auto-generated constructor stub

}

TL_Execute::~TL_Execute() {
	// TODO Auto-generated destructor stub
}
/*
 * 默认处理方式为echo
 * */
void TL_Execute::execute(TL_PacketPtr& packet) {
	//cout << "execute:" << packet->command_id << "|" << packet->recv_data.size() << "|" << packet->recv_data << endl;
	packet->send_data = packet->recv_data;

	//tidp::TL_Cgi cgi;
	//cgi.decodeUrl(packet->recv_data);
	//std::string tmp;
	//cgi.encodeUrl(tmp);
	//cout << "cgi:" << cgi["cmd"] << "|" << tmp << endl;
	//packet->send_data.append(1,'\n');
	cout << "send:" << packet->send_data.size() << "|" << packet->send() << endl;
}

TL_Execute* TL_Execute::create(){
	return new TL_Execute();
}
}


