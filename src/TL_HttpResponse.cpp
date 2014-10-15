/*
 * TL_HttpResponse.cpp
 *
 *  Created on: Jan 15, 2014
 *      Author: ever
 */

#include <TL_HttpResponse.h>

namespace tidp {
TL_HttpResponse::TL_HttpResponse(HttpResponseCode code) :
		_code(code), _contentType("text/plain;charset=utf-8") {

}
string TL_HttpResponse::getResponseString(HttpResponseCode code) {
	switch (code) {

	//  Success 2xx
	case OK:
		return "200 OK";
	case CREATED:
		return "201 Created";
	case ACCEPTED:
		return "202 Accepted";
	case PARTIAL:
		return "203 Partial Information";
	case NO_RESPONSE:
		return "204 No Response";

		//  Redirection 3xx
	case MOVED:
		return "301 Moved";
	case FOUND:
		return "302 Found";
	case METHOD:
		return "303 Method";
	case NOT_MODIFIED:
		return "304 Not Modified";

		//  Error 4xx, 5xx
	case BAD:
		return "400 Bad Request";
	case UNAUTHORIZED:
		return "401 Unauthorized";
	case PAYMENT:
		return "402 Payment Required";
	case FORBIDDEN:
		return "403 Forbidden";
	case NOT_FOUND:
		return "404 Not Found";
	case ERROR_CODE:
		return "500 Internal Error";
	case NOT_IMPLEMENTED:
		return "501 Not implemented";

		// default
	default:
		return "404 Not Found";
	}
}
string TL_HttpResponse::getHead() {
	return "HTTP/1.1 200 OK\r\n"
			"Server: TOLAP\r\n"
			"Connection: Keep-Alive\r\n"
			"Content-Type: text/plain;charset=utf-8\r\n"
			"Content-Length: 0\r\n";
}
void TL_HttpResponse::getHead(TL_MemString& mstr) {
	mstr << "HTTP/1.1 " << this->getResponseString(_code) << "\r\n"
			<< "Connection: Keep-Alive\r\nContent-Type: " << _contentType
			<< "\r\nContent-Length: \r" << endl;
}
void TL_HttpResponse::setBody(const TL_MemString *body) {
	_body = const_cast<TL_MemString *>(body);
}
} /* namespace tidp */
