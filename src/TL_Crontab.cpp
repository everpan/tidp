/*
 * TL_Crontab.cpp
 *
 *  Created on: Aug 13, 2014
 *      Author: ever
 */

#include "TL_Crontab.h"

namespace tidp {

const char* TL_Crontab::TYPE2FMTSTR[6]{"%Y%m%d%H%M%S", //default
    "%Y%m%d%H%M", "%Y%m%d%H", "%Y%m%d", "%Y%m", "%Y%m%d"};

TL_Crontab::TL_Crontab() {
    // TODO Auto-generated constructor stub

}

TL_Crontab::~TL_Crontab() {
    // TODO Auto-generated destructor stub
}

bool TL_Crontab::check(const std::string& conf, const char* tm) {
    const char* p = conf.c_str();
    char r = -1;
    unsigned char i = 0;
    while (*p != 0) {
        switch (*p) {
        case 1: //number
            ++p;
            if (r == -1 && tm[i] == *p)
                r = *p;
            break;
        case 2: //,
            if (r != -1) {
                //next
                char f = 0;
                while (f != 6 && *p != 0) {
                    ++p;
                    switch (*p) {
                    case 1:
                    case 4:
                        ++p;
                        break;
                    case 2:
                    case 5:
                        break;
                    case 3:
                        ++p;
                        ++p;
                        break;
                    case 6:
                        f = 6;
                        --p;
                        break;
                    }
                }
            }
            break;
        case 3: //-
            ++p;
            if (r == -1) {
                if (tm[i] >= *p && tm[i] <= *(p + 1)) {
                    r = tm[i];
                }
            }
            ++p;
            break;
        case 4: // /
            ++p;
            if (r != -1) {
                if (r % *p != 0) {
                    r = -1;
                }
            }
            break;
        case 5: //*
            r = tm[i];
            break;
        case 6: //space
            if (r == -1)
                return false;
            else
                r = -1;
            ++i;
            break;
        default:
        {
            char c[100];
            snprintf(c, 100, "check cron error: %d", (int) *p);
            throw TL_Exp(__FILE__, __LINE__, c, -1);
        }
        } //end switch
        ++p;
    } //end while
    return r != -1;
}

std::string TL_Crontab::parseErrStr(const std::string& str, int pos, int line) {
    char c[256];
    snprintf(c, 256, "TL_Crontab::parse cron : %s error@ %d[%c] line:%d", str.c_str(), pos, str[pos], line);
    return std::string(c);
}

/*
//http://www.quartz-scheduler.org/documentation/quartz-2.2.x/tutorials/crontrigger
Field Name	Mandatory	Allowed Values	Allowed Special Characters
Seconds         YES                 0-59                , - * /
Minutes         YES                 0-59                , - * /
Hours           YES                 0-23                , - * /
Day of month	YES                 1-31                , - * ? / L W
Month           YES                 1-12 or JAN-DEC	, - * /
Day of week	YES                 1-7 or SUN-SAT	, - * ? / L #
Year            NO                  empty, 1970-2099	, - * /
 */
void TL_Crontab::parse(const string& str, string& s) {
    const char * p = str.c_str();
    s.clear();
    struct symbol s1;
    s1.value = 0;
    s1.type = 0; // 1-number
    int pos = 0;
    std::vector<struct symbol> v;
    //词法解析
    while (*p != 0) {
        //[0-9]
        if (*p >= '0' && *p <= '9') {
            s1.value = s1.value * 10 + *p - '0';
            s1.type = 1;
            s1.pos = pos;
            //__added by everpan 2011-12-29
            if (*(p + 1) == 0) {
                v.push_back(s1);
                break;
            }
            //__end
        } else {
            if (s1.type != 0)
                v.push_back(s1);

            if (*p == '-' || *p == ',' || *p == '/' || *p == '*') {
                s1.value = *p;
                s1.type = 2;
                s1.pos = pos;
                v.push_back(s1);
            } else if (*p == ' ' || *p == '\t') {
                if (v.size() > 0 && v.rbegin()->value == ' ' && v.rbegin()->type == 2) {
                    ++pos;
                    ++p;
                    continue;
                }
                s1.value = ' ';
                s1.type = 2;
                s1.pos = pos;
                v.push_back(s1);
            } else {
                //不支持的字符
                throw TL_Exp(parseErrStr(str, pos, __LINE__));
            }
            s1.value = 0, s1.type = 0;
        }
        ++pos;
        ++p;
    } //end while
    //语法解析
    std::vector<struct symbol>::iterator it1 = v.begin();
    std::vector<struct symbol>::iterator it2;
    std::vector<struct symbol>::iterator it3;
    while (it1 != v.end()) {
        it2 = it1 + 1;
        if (it2 == v.end()) {
            if (it1->type == 1) {
                s.push_back(1);
                s.push_back(it1->value);
            } else if (it1->type == 2 && it1->value == '*') {
                s.push_back(5);
            } else {
                throw TL_Exp(parseErrStr(str, it1->pos, __LINE__));
            }
            it1 = it2;
        } else if (it1->type == 2) {
            if (it1->value == '/') {
                if (it2->type != 1) {
                    throw TL_Exp(parseErrStr(str, it1->pos, __LINE__));
                }
                s.push_back(4);
                s.push_back(it2->value);
                it1 = ++it2;
            } else if (it1->value == '*') {
                s.push_back(5);
                ++it1;
            } else if (it1->value == ' ') {
                s.push_back(6);
                ++it1;
            } else if (it1->value == ',') {
                s.push_back(2);
                ++it1;
            } else {
                throw TL_Exp(parseErrStr(str, it1->pos, __LINE__));
            }
        } else {
            if (it1->type == 1 && it2->type == 2) {
                it3 = it2 + 1;
                if (it3 == v.end()) {
                    if (it2->value != ' ')
                        throw TL_Exp(parseErrStr(str, it2->pos, __LINE__));
                    else {
                        s.push_back(1);
                        s.push_back(it1->value);
                        it1 = it3;
                    }
                } else if (it2->value == ',') {
                    s.push_back(1);
                    s.push_back(it1->value);
                    s.push_back(2);
                    if (it3->type == 2 && it3->value != '*') {
                        throw TL_Exp(parseErrStr(str, it3->pos, __LINE__));
                    }
                    it1 = it3;
                } else if (it2->value == '-') {
                    if (it3->type != 1) {
                        throw TL_Exp(parseErrStr(str, it3->pos, __LINE__));
                    } else {
                        s.push_back(3);
                        s.push_back(it1->value);
                        s.push_back(it3->value);
                    }
                    ++it3;
                    if (it3 != v.end() && it3->type != 2) {
                        throw TL_Exp(parseErrStr(str, it3->pos, __LINE__));
                    }
                    it1 = it3;
                } else if (it2->value == '/') {
                    if (it3->type != 1) {
                        throw TL_Exp(parseErrStr(str, it3->pos, __LINE__));
                    } else {
                        s.push_back(1);
                        s.push_back(it1->value);
                        s.push_back(4);
                        s.push_back(it3->value);
                    }
                    ++it3;
                    if (it3 != v.end() && it3->type != 1) {
                        throw TL_Exp(parseErrStr(str, it3->pos, __LINE__));
                    }
                    it1 = it3;
                } else if (it2->value == ' ') {
                    s.push_back(1);
                    s.push_back(it1->value);
                    s.push_back(6);
                    if (it3->type == 2 && it3->value != '*') {
                        throw TL_Exp(parseErrStr(str, it3->pos, __LINE__));
                    }
                    it1 = it3;
                }
            } else {
                throw TL_Exp(parseErrStr(str, it2->pos, __LINE__));
            }
        }
    }
}

std::string TL_Crontab::parse(const std::string& str) {
    std::string s;
    TL_Crontab::parse(str, s);
    return s;
}

} /* namespace tidp */
