/* 
 * File:   TL_Rlog.h
 * Author: everpan
 *
 * Created on 2011年4月21日, 上午9:29
 */

#ifndef TL_RLOG_H
#define	TL_RLOG_H

#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string>
#include <fstream>
#include <sstream>
#include "TL_Exp.h"
#include "TL_Datetime.h"
namespace tidp
{
    class TL_Rlog;
    typedef TL_Rlog& (*__Roll_Func)(TL_Rlog&);
    class TL_Rlog
    {
    public:

        enum Log_Level
        {
            NO_LOG = 1,
            ERROR_LOG = 2,
            NORMAL_LOG = 3,
            DEBUG_LOG = 4
        };

        enum title_flag
        {
            F_Time = 0,
            F_Module = 1,
            F_PID = 2,
            F_PthreadID = 3,
            F_DEBUGTIP = 4
        };

        static const unsigned FLUSH_COUNT = 32;
        TL_Rlog();


        void Init(const std::string& sPreFix, const std::string& module, size_t maxsize, size_t maxnum = 10);
        void set_titleflag(title_flag f);
        void set_level(const Log_Level & lv);
        bool check_level();
        void head();

        TL_Rlog& debug(){return *this;}

        TL_Rlog & operator<<(char n);
        TL_Rlog & operator<<(unsigned char n);

        TL_Rlog & operator<<(short n);
        TL_Rlog & operator<<(unsigned short n);

        TL_Rlog & operator<<(int n);
        TL_Rlog & operator<<(unsigned int n);

        TL_Rlog & operator<<(long n);
        TL_Rlog & operator<<(unsigned long n);

        TL_Rlog & operator<<(long long n);
        TL_Rlog & operator<<(unsigned long long n);

        TL_Rlog & operator<<(double n);
        TL_Rlog & operator<<(float n);
        //TL_Rlog & operator<<(float n);
        TL_Rlog & operator<<(const std::string& s);
        TL_Rlog& operator<<(__Roll_Func func){ return (*func)(*this);}
        virtual ~TL_Rlog();
    protected:

        friend TL_Rlog& endl(TL_Rlog& log);
        friend TL_Rlog& debug(TL_Rlog& log);
        friend TL_Rlog& error(TL_Rlog& log);
        friend TL_Rlog& normal(TL_Rlog& log);
        friend TL_Rlog& nolog(TL_Rlog& log);


    private:
        TL_Rlog(const TL_Rlog& orig);
        int _fd;
        Log_Level _setlevel;
        Log_Level _level;
        unsigned _buf_count;

        unsigned _max_log_size;
        unsigned _max_log_num;

        std::string _module;
        std::string _filename;
        std::string _head;
        //std::string _time_format;
        unsigned _pid;
        std::string _debugtip;

        bool _flags[4];
        bool _lock;
        char _buffer[1024 * 8];
        //std::string _log_buffer;
        // _log_buffer;
        std::ostringstream _log_buffer;
        TL_Datetime _cur_time;
    };

    inline TL_Rlog& endl(TL_Rlog& log)
    {
        log._log_buffer << "\n";
        log.head();
        log.check_level();

        write(log._fd, log._head.c_str(), log._head.size());
        std::string buffer = log._log_buffer.str();
        write(log._fd, buffer.c_str(), buffer.size());
        log._log_buffer.str("");
        return log;
    }

    inline TL_Rlog& debug(TL_Rlog& log)
    {
        log.set_level(log.DEBUG_LOG);
        return log;
    }
    
}





#endif	/* TL_RLOG_H */

