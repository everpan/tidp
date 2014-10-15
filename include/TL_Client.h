/*
 * TL_Client.h
 *
 *  Created on: Aug 13, 2014
 *      Author: ever
 */

#ifndef TSCLIENT_H_
#define TSCLIENT_H_

#include <iostream>
#include <string>
#include "TL_Common.h"
#include <errno.h>
#include <unistd.h>
/*

#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>


*/
using namespace tidp;

using namespace std;

#define CACHE_SIZE 1024

class TL_Client {
public:
	TL_Client();
	virtual ~TL_Client();

	void connect(const string& host, short port = -1);
	void connect();

	void close();
	void sendAndRecv(const string& send_data, string& recv_data);

	static void setNonblock(int fd);


private:
	int _clientfd;
	int _sock_family;
	short _port;
	char * _buffer;
	string _host;

};

#endif /* TSCLIENT_H_ */
