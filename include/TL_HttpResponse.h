/*
 * TL_HttpResponse.h
 *
 *  Created on: Jan 15, 2014
 *      Author: ever
 */

#ifndef TL_HTTPRESPONSE_H_
#define TL_HTTPRESPONSE_H_

#include <string>
using namespace std;

#include "TL_MemString.h"
namespace tidp {

class TL_HttpResponse {
public:
	enum HttpResponseCode {
		OK = 200,
		CREATED = 201,
		ACCEPTED = 202,
		PARTIAL = 203,
		NO_RESPONSE = 204,
		MOVED = 301,
		FOUND = 302,
		METHOD = 303,
		NOT_MODIFIED = 304,
		BAD = 400,
		UNAUTHORIZED = 401,
		PAYMENT = 402,
		FORBIDDEN = 403,
		NOT_FOUND = 404,
		ERROR_CODE = 500,
		NOT_IMPLEMENTED = 501,
	};
	TL_HttpResponse(HttpResponseCode);
	string getResponseString(HttpResponseCode code);
	string getHead();
	void getHead(TL_MemString& mstr);
	void setBody(const TL_MemString *body);
private:
	HttpResponseCode _code;
	TL_MemString *_body;
	string _contentType;
};

} /* namespace tidp */

#endif /* TL_HTTPRESPONSE_H_ */
