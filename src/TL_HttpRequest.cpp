/*
 * TL_HttpRequest.cpp
 *
 *  Created on: Jan 14, 2014
 *      Author: ever
 */

#include <TL_HttpRequest.h>

namespace tidp {
TL_HttpRequest::TL_HttpRequest() {
	_type = HTTP_REQUEST_ILLEGAL;
	_contentLength = 0;
}
const map<string, string>& TL_HttpRequest::getHeaders() const {
	return _headerFields;
}
const string& TL_HttpRequest::getValue(const string& key) const {

	map<string, string>::const_iterator it = _requestFields.find(key);

	if (it != _requestFields.end()) {
		return it->second;
	} else {
		return _error_val;
	}
}
const string& TL_HttpRequest::getValue(const string& key, bool& found) const {
	static const string error = "";

	map<string, string>::const_iterator it = _requestFields.find(key);

	if (it != _requestFields.end()) {
		found = true;
		return it->second;
	} else {
		found = false;
		return _error_val;
	}
}
const map<string, string>& TL_HttpRequest::getValues() const {
	return _requestFields;
}
void TL_HttpRequest::extractHeader(char* begin, char* end) {
	// 1. split header into lines at "\r\n"
	// 2. split lines at " "
	// 3. split GET/POST etc. requests

	//
	// check for '\n' (we check for '\r' later)
	//
	static const char CR = '\r';
	static const char NL = '\n';
	static const char SPC = ' ';

	int lineNum = 0;
	for (char* start = begin; start < end;) {
		char* findnl = start;

		for (; findnl < end && *findnl != NL; ++findnl) {
		}

		if (findnl == end) {
			break;
		}

		char* endnl = findnl;

		//
		// check for '\r'
		//

		if (endnl > start && *(endnl - 1) == CR) {
			endnl--;
		}

		if (endnl > start) {

			//
			// split line at spaces
			//

			char* space = start;

			for (; space < endnl && *space != SPC; ++space) {
			}

			if (space < endnl) {
				if (space > start) {
					char* colon = space;

					if (*(colon - 1) == ':') {
						--colon;
					}

					string key(start, colon);

					// check for request type (GET/POST in line 0),
					// path and parameters
					if (lineNum == 0) {
						if (key == "POST") {
							_type = HTTP_REQUEST_POST;
						} else if (key == "GET") {
							_type = HTTP_REQUEST_GET;
						}

						if (_type != HTTP_REQUEST_ILLEGAL) {
							char* reqe = space + 1;

							// delete "HTTP/1.1" from request
							for (; reqe < endnl && *reqe != SPC; reqe++) {
							}

							string value(space + 1, reqe);
							_headerFields[key] = value;

							// split requestPath and parameters
							char* parm = space + 1;

							for (; parm < reqe && *parm != '?'; parm++) {
							}

							if (parm < reqe) {
								setKeyValues(parm + 1, reqe);
							}
						}
					} else {
						string value(space + 1, endnl);
						_headerFields[key] = value;
					}
				}
			} else {
				string key(start, endnl);
				_headerFields[key] = "";
			}

			start = end + 1;
		}

		start = findnl + 1;
		lineNum++;
	}					//for

	if (_headerFields.find("Content-Length") != _headerFields.end()) {
		_contentLength = strtol(_headerFields["Content-Length"].c_str(), NULL,
				10);
	}
}

void TL_HttpRequest::extractBody(char* begin, char* end) {
	//post
	setKeyValues(begin, end);
}

void TL_HttpRequest::setKeyValues(char* begin, char* end) {
	enum {
		KEY, VALUE
	} phase = KEY;
	enum {
		NORMAL, HEX1, HEX2
	} reader = NORMAL;

	int hex = 0;

	static const char AMB = '&';
	static const char EQUAL = '=';
	static const char PERCENT = '%';
	static const char PLUS = '+';
	static const char SPC = ' ';

	char * buffer = begin;
	char * keyStart = buffer;
	char * keyPtr = keyStart;
	char * valueStart = buffer;
	char * valuePtr = valueStart;

	for (; buffer < end; ++buffer) {
		char next = *buffer;

		if (phase == KEY && next == EQUAL) {
			phase = VALUE;

			valueStart = buffer + 1;
			valuePtr = valueStart;

			continue;
		} else if (next == AMB) {
			phase = KEY;

			string keyStr(keyStart, keyPtr);
			keyStart = buffer + 1;
			keyPtr = keyStart;

			string valueStr(valueStart, valuePtr);
			valueStart = buffer + 1;
			valuePtr = valueStart;

			_requestFields[keyStr] = valueStr;

			continue;
		} else if (next == PERCENT) {
			reader = HEX1;
			continue;
		} else if (reader == HEX1) {
			hex = (hex2int(next) << 4);
			reader = HEX2;
			continue;
		} else if (reader == HEX2) {
			hex += hex2int(next);
			reader = NORMAL;
			next = (char) hex;
		} else if (next == PLUS) {
			next = SPC;
		}

		if (phase == KEY) {
			*keyPtr++ = next;
		} else {
			*valuePtr++ = next;
		}
	}

	if (keyStart < keyPtr) {
		string keyStr(keyStart, keyPtr);
		string valueStr(valueStart, valuePtr);

		_requestFields[keyStr] = valueStr;
	}
}
} /* namespace tidp */
