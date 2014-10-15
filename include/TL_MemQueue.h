/* 
 * File:   TL_MemQueue.h
 * Author: everpan
 *
 * Created on 2011年4月30日, 下午9:06
 */

#ifndef TL_MEMQUEUE_H
#define	TL_MEMQUEUE_H
#include <string>
#include <assert.h>
#include "TL_Exp.h"

namespace tidp {

    class TL_MemQueue {
    public:
        TL_MemQueue(char * addr, size_t len);
        TL_MemQueue();
        
        void init(char * addr, size_t len);
        virtual ~TL_MemQueue();
        void attache(char * addr, size_t len);
        void connect(char * addr, size_t len);


        bool push_back(const char* data, size_t len);
        bool pop_front(char*data, size_t & ilen);
        bool isFull(size_t len);
        bool isEmpty();
    protected:
        TL_MemQueue(const TL_MemQueue& orig);
        typedef struct head_block {
            size_t memsize; //内存大小
            size_t itemsize; //队列中进入元素的个数
            size_t top; //顶部元素索引
            size_t bot; //底部元素索引
        } /*__attribute__((packed))*/ HEAD_BLOCK;

    private:
        char * _addr;
        char * _addrend;
        //size_t _len;
        HEAD_BLOCK * _head;
    };
}
#endif	/* TL_MEMQUEUE_H */

