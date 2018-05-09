/*
 * File:   TL_Cgi.cpp
 * Author: everpan
 *
 * Created on 2011年6月15日, 下午5:14
 */

#include "TL_Cgi.h"

namespace tidp {
const char TL_Cgi::hexchars[] = "0123456789ABCDEF";
const unsigned TL_Cgi::TL_Cgi_TMP_DEFAULT_LEN = 1024 * 3;

TL_Cgi::TL_Cgi() {
    _tmp = NULL;
    remallocTmp(TL_Cgi_TMP_DEFAULT_LEN);
}

TL_Cgi::~TL_Cgi() {
    if (_tmp) {
        delete _tmp;
        _tmp = NULL;
    }
}

void TL_Cgi::clear() {
    _param.clear();
}

std::string& TL_Cgi::operator[](const std::string& str) {
    std::multimap<std::string, std::string>::iterator it = _param.find(str);
    if (it == _param.end()) {
        return _param.insert(std::multimap<std::string, std::string>::value_type(str, ""))->second;
    }
    return it->second;
}

void TL_Cgi::decodeUrl(const std::string& str) {
    decodeUrl(str.c_str(), str.size());
}

void TL_Cgi::decodeUrl(char const* s, int len) {
    _param.clear();
    if (_tmp_len < len) {
        remallocTmp(len);
    }
    memcpy(_tmp, s, len);
    char* from = _tmp;
    char* end = _tmp + len;

    char* key = _tmp;
    char* val = _tmp;
    int key_len = 0;
    int val_len = 0;
    while (from < end) {
        if (*from == '=') {
            val = from + 1;
            key_len = from - key;
            val_len = 0;
        } else if (*from == '&') {
            val_len = from - val;
            key_len = urlDecode(key, key_len);
            val_len = urlDecode(val, val_len);
            _param.insert(
                    std::multimap<std::string, std::string>::value_type(std::string(key, key_len),
                    std::string(val, val_len)));
            key = from + 1;
            key_len = 0;
            val_len = 0;
        }
        ++from;
    }
    val_len = from - val;
    if (key_len > 0 /*&& val_len > 0*/) {
        key_len = urlDecode(key, key_len);
        if (val_len > 0)
            val_len = urlDecode(val, val_len);
        _param.insert(
                std::multimap<std::string, std::string>::value_type(std::string(key, key_len),
                std::string(val, val_len)));
    }
}

void TL_Cgi::encodeUrl(const std::map<std::string, std::string>&in, std::string& out) {
    out.clear();
    std::map<std::string, std::string>::const_iterator it = in.begin();
    char *tmp = (char*) malloc(1024);
    if (tmp == NULL) {
        throw TL_Exp("TL_Cgi::encodeUrl malloc error.");
    }
    int len = 1024;
    int ret_str_len = 0;
    while (it != in.end()) {
        //len = 1024;
        //urlEncode(it->first.c_str(), it->first.size(), &tmp, len);
        ret_str_len = urlEscape(it->first.c_str(), it->first.size(), &tmp, &len);
        if (!out.empty()) {
            out.append(1, '&');
        }
        out.append(tmp, ret_str_len);
        out.append(1, '=');
        ret_str_len = urlEscape(it->second.c_str(), it->second.size(), &tmp, &len);
        out.append(tmp, ret_str_len);
        ++it;
    }
    free(tmp);
}

void TL_Cgi::encodeUrl(std::string& rstr) {
    rstr = "";
    std::multimap<std::string, std::string>::iterator it = _param.begin();
    int len = 0;
    while (it != _param.end()) {
        //t->first
        //len = _tmp_len;
        len = urlEscape(it->first.c_str(), it->first.size(), &_tmp, &_tmp_len);
        if (len == 0) {
            ++it;
            continue;
        }
        if (rstr.size() > 0)
            rstr += "&";
        rstr += std::string(_tmp, len);

        len = urlEscape(it->second.c_str(), it->second.size(), &_tmp, &_tmp_len);
        if (len == 0) {
            ++it;
            continue;
        }
        rstr += "=" + std::string(_tmp, len);
        ++it;
    }
}

int TL_Cgi::urlEscape(const char *s, int len, char **outbuffer, int *outbufflen) {
    unsigned char c;
    unsigned char *to, *start;
    unsigned char const *from, *end;
    if (*outbuffer == NULL) {
        *outbuffer = (char *) malloc(3 * len + 10);
    } else if (*outbufflen < 3 * len + 1) {
        *outbuffer = (char *) realloc(*outbuffer, 3 * len + 10);
    }
    if (*outbuffer == NULL) {
        throw TL_Exp("TL_Cgi::urlEncode realloc error.");
        return 0;
    }
    *outbufflen = 3 * len + 10;
    from = (unsigned char *) s;
    end = (unsigned char *) s + len;
    start = to = (unsigned char *) *outbuffer;

    while (from < end) {
        c = *from++;

        if (c == ' ') {
            *to++ = '+';
        } else if ((c < '0' && c != '-' && c != '.') || (c < 'A' && c > '9') || (c > 'Z' && c < 'a' && c != '_')
                || (c > 'z' && c != '~')) {
            to[0] = '%';
            to[1] = hexchars[c >> 4];
            to[2] = hexchars[c & 15];
            to += 3;
        } else {
            *to++ = c;
        }
    }
    *to = 0;
    return to - start;
    //return (char *) start;
}

int TL_Cgi::urlEscape(const std::string& in, std::string& out) {
    if (in.size() == 0) {
        return 0;
    }
    out.clear();
    out.reserve(in.size() * 3);
    unsigned char c;
    unsigned char const *from, *end;
    from = (unsigned char *) in.c_str();
    end = (unsigned char *) from + in.size();

    while (from < end) {
        c = *from;
        ++from;
        if (c == ' ') {
            out.append(1, '+');
        } else if ((c < '0' && c != '-' && c != '.') || (c < 'A' && c > '9') || (c > 'Z' && c < 'a' && c != '_')
                || (c > 'z' && c != '~')) {
            out.append(1, '%');
            out.append(1, hexchars[c >> 4]);
            out.append(1, hexchars[c & 15]);
        } else {
            out.append(1, c);
        }
    }
    return out.size();
}

int TL_Cgi::urlDecode(char *str, int len) {
    char *dest = str;
    char *data = str;

    while (len--) {
        if (*data == '+') {
            *dest = ' ';
        } else if (*data == '%' && len >= 2 && isxdigit((int) *(data + 1)) && isxdigit((int) *(data + 2))) {
            *dest = (char) htoi(data + 1);

            data += 2;
            len -= 2;
        } else {
            *dest = *data;
        }
        data++;
        dest++;
    }
    *dest = '\0';
    return dest - str;
}

int TL_Cgi::rawUrlDecode(char *str, int len) {
    char *dest = str;
    char *data = str;

    while (len--) {
        if (*data == '%' && len >= 2 && isxdigit((int) *(data + 1)) && isxdigit((int) *(data + 2))) {
            *dest = (char) htoi(data + 1);
            data += 2;
            len -= 2;
        } else {
            *dest = *data;
        }
        data++;
        dest++;
    }
    *dest = '\0';
    return dest - str;
}

int TL_Cgi::htoi(char *s) {
    int value;
    int c;

    c = ((unsigned char *) s)[0];
    if (c > 64 && c < 71) {
        //A-F
        c -= 55;
    } else if (c > 96 && c < 103) {
        //a-f
        c -= 87;
    } else if (c > 48 && c < 58) {
        c -= 48;
    } else {
        c = 0;
    }
    value = c * 16;
    c = ((unsigned char *) s)[1];
    if (c > 64 && c < 71) {
        //A-F
        c -= 55;
    } else if (c > 96 && c < 103) {
        //a-f
        c -= 87;
    } else if (c > 48 && c < 58) {
        c -= 48;
    } else {
        c = 0;
    }
    value += c;
    return value;
}

void TL_Cgi::remallocTmp(int len) {
    if (_tmp)
        delete _tmp;
    _tmp = NULL;
    _tmp = (char*) malloc(len);
    _tmp_len = len;
    if (_tmp == NULL) {
        throw TL_Exp("TL_Cgi::decodeUrl:malloc failed!");
    }
}
}
