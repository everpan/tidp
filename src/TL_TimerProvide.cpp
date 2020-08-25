/*
 * TL_TimerProvide.cpp
 *
 *  Created on: Jan 5, 2015
 *      Author: ever
 */

#include "TL_TimerProvide.h"

#define TL_TIMEPROVIDE_PIPE_PATH "/tmp/timeProvid.fifo"
namespace tidp {
#ifdef __linux__

    TL_TimerProvide::TL_TimerProvide() {
        /*_fifo = mkfifo(TL_TIMEPROVIDE_PIPE_PATH, 0777);
         if (_fifo != 0) {
         ostringstream os;
         os << "mkfifo(" << TL_TIMEPROVIDE_PIPE_PATH << ") failed:" << _fifo;
         throw TL_Exception(os.str(), errno);
         }*/
        _epfd = epoll_create(1);
        if (_epfd < 0) {
            ostringstream os;
            os << "epoll_create failed:" << _epfd;
            throw TL_Exception(os.str(), errno);
        }
        /*
         struct epoll_event event;
         event.events = EPOLLIN;
         int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, _fifo, &event);
         if (ret != 0) {
         ostringstream os;
         os << "epoll_ctl ADD:" << _fifo << " failed:" << ret;
         throw TL_Exception(os.str(), errno);
         }
         */
    }

    TL_TimerProvide::~TL_TimerProvide() noexcept(true) {
        // TODO Auto-generated destructor stub
    }

    TL_TimerProvide *TL_TimerProvide::getInstance() {
        TL_TimerProvide *p = TL_Singleton<TL_TimerProvide>::getInstance();
        if (!p->isRunning()) {
            cout << "TL_TimerProvide auto start......" << endl;
            p->start();
        }
        return p;
    }

    void TL_TimerProvide::run() {
        _running = 1;
        int ret = 0;
        struct epoll_event event;
        int cycle = 0;
        int time_out = 97;
        int time_out_intval = time_out * 1000;
        while (_running) {
            ret = epoll_wait(_epfd, &event, 1, time_out);
            if (ret == -1 && errno == EINTR) {
                _tv.tv_usec += time_out_intval + cycle;
                if (_tv.tv_usec > 999999) {
                    sync();
                    cycle = 0;
                } else {
                    ++cycle;
                }
            } else if (ret == 0) {
                _tv.tv_usec += time_out_intval + cycle;
                if (_tv.tv_usec > 999999) {
                    sync();
                    cycle = 0;
                } else {
                    ++cycle;
                }
            } else if (ret > 0) {
                //string s = toString(_tm_fmt + "\n");
                cout << toString() << endl;
            }
            if (cycle > 1000 / time_out) {
                cycle = 0;
                cout << "sync:------" << endl;
                sync();
            }
            //if(cycle % 10 == 0)
            cout << _tv.tv_sec << "." << _tv.tv_usec << " " << cycle << " " << time(0) << endl;
        }
    }

    TL_Datetime TL_TimerProvide::getDatetime() {
        return TL_Datetime(_tv);
    }

    void TL_TimerProvide::updateDatetime(TL_Datetime &dt) {
        dt.setTime(_tv);
    }

#endif
} /* namespace tidp */
