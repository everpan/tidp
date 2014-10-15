/*
 * TL_Http.cpp
 *
 *  Created on: Oct 3, 2014
 *      Author: ever
 */

#include "TL_Http.h"

namespace tidp {

TL_Http::TL_Http() {
	//_is_complete = false;
	//_http_type = HTTP_UNKNOWN;
	//_contend_len = 0;
	clear();
}

TL_Http::~TL_Http() {
}

void TL_Http::clear() {
	_is_complete = false;
	_http_type = HTTP_UNKNOWN;
	_contend_len = 0;
	_header.clear();
	_body.clear();
}
void TL_Http::setType(HttpMethodType type) {
	_http_type = HTTP_UNKNOWN;
}
int TL_Http::parse(const char* buff, int len) {
	return 0;
}
bool TL_Http::isComplete() {
	return _is_complete;
}
const string& TL_Http::getHead(const string& key) {
	static string empty;
	map<string, string>::iterator it = _header.find(key);
	if (it == _header.end()) {
		return empty;
	}
	return it->second;
}
void TL_Http::setHead(const string& key, const string& val) {
	_header[key] = val;
}
const string& TL_Http::getBody() {
	return _body;
}
void TL_Http::setBody(const string& content) {
	_body = content;
}
void TL_Http::appendBody(const string& content) {
	_body.append(content);
}
void TL_Http::appendBody(const char* cbuff, int len) {
	_body.append(cbuff, len);
}
int TL_Http::send(int fd) {
	return 0;
}
} /* namespace tidp */
