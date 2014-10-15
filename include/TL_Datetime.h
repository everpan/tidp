/* 
 * File:   TL_Datetime.h
 * Author: everpan
 *
 * Created on 2011年3月28日, 上午11:42
 */

#ifndef TL_DATETIME_H
#define	TL_DATETIME_H
#include <time.h>
#include <sys/time.h>
#include <string>
#include <string.h>
#include "TL_Exp.h"
#include <iostream>
#define SECONDS_OF_A_MINUTE 60

namespace tidp {
    
    class TL_Datetime {
    public:
        TL_Datetime();
        TL_Datetime(time_t t);
        TL_Datetime(struct timeval tv);
        TL_Datetime(const TL_Datetime& orig);
        TL_Datetime(const std::string &stime, const std::string &sformat);
        virtual ~TL_Datetime();
        void setTime(const time_t& t);
        void setInterval(int v);
        int getInterval();
        void addMonth(int mon);
        void setTimeStringFormat(const std::string& fmt);
        time_t getTimestamp();
        struct timeval getTimeOfDay();
        void sync();
        void syncOnlySecond();
        struct timeval getTimeval();
        void setCronTime(char* c);
        void reset();
        std::string toString();
        std::string toString(const std::string& fmt);

        static std::string tm2str(const time_t& t, const std::string& fmt);
        static time_t parse(const std::string &stime, const std::string &sformat);
        static bool is_leap_year(int year);
        static void localtime(time_t t,struct tm& ptm,int timez=8);
        TL_Datetime operator ++(int);
        TL_Datetime & operator ++(); //前置
        TL_Datetime operator +(int v);
        TL_Datetime & operator +=(int v);

        TL_Datetime operator --(int);
        TL_Datetime & operator --();
        TL_Datetime operator -(int v);
        TL_Datetime & operator -=(int v);


        //struct timeval operator - (const TL_Datetime& d);
        const bool operator<(const TL_Datetime& d);
        const bool operator==(const TL_Datetime& d);
        const bool operator<(const time_t& t) const;
        const bool operator==(const time_t& t) const;

        static const char MON_MAX_DAY[12];
        static const char LOCALTIME_MON1[12];
        static const char LOCALTIME_MON2[12];
    private:
        //time_t _t;
        struct timeval _tv;
        int _interval;
        
        std::string _tm_fmt;
    };
}
#endif	/* TL_DATETIME_H */

