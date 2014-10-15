/*
 * File:   TL_Cgi.h
 * Author: everpan
 *
 * Created on 2011年6月15日, 下午5:14
 *
 *
 * value的最大字符数为1K，可以更改TL_Cgi_TMP_LEN参数扩展，
 * todo 修改逻辑成自动扩展。
 */

#ifndef TL_CGI_H
#define	TL_CGI_H
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <string.h>
#include "TL_Exp.h"
namespace tidp {

    class TL_Cgi {
    public:
        TL_Cgi();

        virtual ~TL_Cgi();

        void clear();
        std::string& operator[](const std::string& str);
        const std::string& getValue(const std::string& str);
        //void parse(const std::string& str);
        void decodeUrl(const std::string& str);
        void decodeUrl(char const* s,int len);
        void encodeUrl(std::string& rstr);
        static void encodeUrl(const std::map<std::string,std::string>&in,std::string& out);
        void encodeXml(std::string& rstr);
        void encodeJson(std::string& rstr);

        //static char * urlEncode(char const*s, int len, char **rs, int &rlen);
        static int urlEscape(char const*s, int len, char **outbuffer, int *outbufflen);
        static int urlEscape(const std::string& in,std::string& out);
        static char * rawUrlEncode(char const *s, int len, int *new_length);
        static int urlDecode(char* str, int len);
        static int rawUrlDecode(char *str, int len);
        static int htoi(char *s);
    private:
        std::multimap<std::string, std::string> _param;
        static const char hexchars[];
        static const unsigned TL_Cgi_TMP_DEFAULT_LEN;

        char * _tmp;//[TL_Cgi_TMP_LEN];
        int _tmp_len;

        void remallocTmp(int len);
    protected:
        TL_Cgi(const TL_Cgi& orig);
    };
}
#endif	/* TL_CGI_H */

