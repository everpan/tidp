/*
 * File:   TL_Exp.cpp
 * Author: everpan
 *
 * Created on 2011年3月15日, 上午11:48
 */

#include "TL_Exp.h"
namespace tidp {

TL_Exp::TL_Exp(const std::string &buffer)
		: _buffer(buffer), _code(0) {
	//    getBacktrace();
}

TL_Exp::TL_Exp(const std::string &buffer, int err) {
	_buffer = buffer + " :" + strerror(err);
	_code = err;
	//    getBacktrace();
}
TL_Exp::TL_Exp(const std::string &head, const std::string& msg, int lineno) {
	// = head;
	_buffer = msg;
	_line_no = lineno;
}
TL_Exp::TL_Exp(const std::string& file, int lineno, const std::string& msg, int errcode) throw () {
	std::ostringstream os;
	os << file << ":" << lineno << " " << msg << ":" << strerror(errcode);
	_buffer = os.str();
	_code = errcode;
	_line_no = lineno;
}
TL_Exp::TL_Exp(const char * file, int lineno, const char * func, const std::string& msg, int errcode) throw () {
	std::ostringstream os;
	os << file << ":" << lineno << ":" << func << " " << msg << ":" << strerror(errcode);
	_buffer = os.str();
	_code = errcode;
	_line_no = lineno;
}
TL_Exp::~TL_Exp() throw () {
}

const char* TL_Exp::what() const throw () {
	//TL_Exp::what
	return _buffer.c_str();
}

}
