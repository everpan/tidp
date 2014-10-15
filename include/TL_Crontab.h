/*
 * TL_Crontab.h
 *
 *  Created on: Aug 13, 2014
 *      Author: ever
 */

#ifndef TL_CRONTAB_H_
#define TL_CRONTAB_H_
#include "TL_Exp.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

namespace tidp {
using namespace std;
class TL_Crontab {
public:
	struct symbol {
		char type;
		char value;
		int pos;
	};
	TL_Crontab();
	virtual ~TL_Crontab();
	static const char* TYPE2FMTSTR[6];

	static std::string parseErrStr(const std::string& str, int pos, int line);
	static string parse(const string& rawstr);
	static bool check(const std::string& conf, const char* tm);
};

} /* namespace tidp */

#endif /* TL_CRONTAB_H_ */
