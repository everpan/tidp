/*
 * TL_Http.h
 *
 *  Created on: Oct 3, 2014
 *      Author: ever
 */

#ifndef TL_HTTP_H_
#define TL_HTTP_H_

#include <stdlib.h>
#include <string.h>

#include <map>
#include <string>
using namespace std;
namespace tidp {
class TL_Http {
public:
	enum HttpMethodType {
		HTTP_UNKNOWN, HTTP_GET, //get
		HTTP_POST,
		HTTP_PUT,
		HTTP_HEAD,
		HTTP_TRACE,
		HTTP_OPTIONS,
		HTTP_DELETE,
		HTTP_RESPONSE
	};
	TL_Http();
	virtual ~TL_Http();

	void clear();

	void setType(HttpMethodType);
	int parse(const char* buff, int len);
	bool isComplete();
	const string& getHead(const string& key);
	void setHead(const string& key, const string& val);
	const string& getBody();
	void setBody(const string& content);
	void appendBody(const string& content);
	void appendBody(const char* cbuff, int len);
	int send(int fd);
private:
	HttpMethodType parseMethod(const char * buff);
private:
	bool _is_complete;
	HttpMethodType _http_type;
	int _contend_len;
	int _status_code;
	string _reason_phrase; //OK | NO OK
	string _version; //HTTP/<major>.<minor>
	string _body;
	string _raw_content; //cache
	map<string, string> _header;
};

} /* namespace tidp */

#endif /* TL_HTTP_H_ */
