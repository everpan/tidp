/*
 * TL_HttpRequest.h
 *
 *  Created on: Jan 14, 2014
 *      Author: ever
 */

#ifndef TL_HTTPREQUEST_H_
#define TL_HTTPREQUEST_H_
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
namespace tidp {

using namespace std;
class TL_HttpRequest {
public:
	TL_HttpRequest();
	enum HttpRequestType {
		HTTP_REQUEST_GET, HTTP_REQUEST_POST, HTTP_REQUEST_ILLEGAL
	};
	HttpRequestType getRequestType() const {
		return _type;
	}
	const map<string, string>& getHeaders() const;
	const string& getValue(const string& key) const;
	const string& getValue(const string& key, bool& found) const;
	const map<string, string>& getValues() const;
	void extractHeader(char* begin, char* end);
	void extractBody(char* begin, char* end);
	void setKeyValues(char* begin, char* end);

protected:
	static inline int hex2int(char ch) {
		if ('0' <= ch && ch <= '9') {
			return ch - '0';
		} else if ('A' <= ch && ch <= 'F') {
			return ch - 'A' + 10;
		} else if ('a' <= ch && ch <= 'f') {
			return ch - 'a' + 10;
		}

		//Logger::warning << "'" << ch << "' is not a hex number" << endl;

		return 0;
	}
private:
	const string _error_val;
	HttpRequestType _type;
	map<string, string> _headerFields;
	map<string, string> _requestFields;
	vector<string> _requestKeys;
	size_t _contentLength;
};

} /* namespace tidp */

#endif /* TL_HTTPREQUEST_H_ */
