/*
 * TL_TimerProvide.h
 *
 *  Created on: Jan 5, 2015
 *      Author: ever
 */

#ifndef SRC_TL_TIMERPROVIDE_H_
#define SRC_TL_TIMERPROVIDE_H_

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef __linux__
//linux
#include <sys/epoll.h>

#endif

#include <TL_Singleton.h>
#include <TL_Datetime.h>
#include <TL_Thread.h>

#ifndef TL_TIMEPROVIDE_PIPE_PATH
#define TL_TIMEPROVIDE_PIPE_PATH "/tmp/timeProvid.fifo"
#endif

namespace tidp {

    class TL_TimerProvide : public TL_Singleton<TL_TimerProvide>, public TL_Datetime, public TL_Thread {
    public:
        TL_TimerProvide();

        virtual ~TL_TimerProvide() noexcept(true) ;

        static TL_TimerProvide *getInstance();

        virtual void run();

        TL_Datetime getDatetime();

        void updateDatetime(TL_Datetime &dt);

    private:
        //int _fifo;
        int _epfd;
    };

} /* namespace tidp */

#endif /* SRC_TL_TIMERPROVIDE_H_ */
