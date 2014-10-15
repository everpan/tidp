/*
 * TL_Client.cpp
 *
 *  Created on: Aug 13, 2014
 *      Author: ever
 */

#include "TL_Client.h"

TL_Client::TL_Client() {
	_clientfd = -1;
	_sock_family = -1;
	_port = -1;
	//_buffer = NULL;
	_buffer = (char *) malloc(CACHE_SIZE);
}

TL_Client::~TL_Client() {
	if (_buffer) {
		free(_buffer);
		_buffer = NULL;
	}
}

void TL_Client::connect(const string& host, short port) {
	close();
	_host = host;
	_port = port;
	connect();
}

void TL_Client::connect() {
	if (_port > 0) {
		_clientfd = TL_Common::connect(_host, _port);
	} else {
		_clientfd = TL_Common::connectUnix(_host);
	}
}

void TL_Client::close() {
	if (_clientfd > 0) {
		::close(_clientfd);
		_clientfd = -1;
	}
}
void TL_Client::sendAndRecv(const string& send_data, string& recv_data) {
	unsigned int ret = 0;
	unsigned int len = send_data.size();
	int x = 0;
	const char * p = send_data.c_str();
	while (ret < len) {
		x = write(_clientfd, p + ret, len - ret);
		if (x < 0) {
			throw TL_Exception("write failed.", errno);
		} else {
			ret += x;
		}
	}
	//recv
	recv_data.clear();
	do {
		x = read(_clientfd, _buffer, CACHE_SIZE);
		if (x < 0) {
			throw TL_Exception("read failed.", errno);
		} else {
			recv_data.append(_buffer, x);
			if (_buffer[x - 1] == '\n') {
				break;
			}
		}
	} while (1);

}
void TL_Client::setNonblock(int fd) {
	int flag = fcntl(fd, F_GETFL, 0);
	flag |= O_NONBLOCK;
	fcntl(fd, F_SETFL, flag);
	int opt = 1;
	int ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (ret < 0) {
		cout << "set sock[" << fd << "] opt error:" << strerror(errno) << endl;
		//exit(-1);
	}
}
