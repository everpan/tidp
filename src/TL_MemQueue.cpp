/* 
 * File:   TL_MemQueue.cpp
 * Author: everpan
 * 
 * Created on 2011年4月30日, 下午9:06
 */

#include "TL_MemQueue.h"
namespace tidp {

    TL_MemQueue::TL_MemQueue() {
        _head = NULL;
        _addr = NULL;
        _addrend = NULL;
    }

    TL_MemQueue::TL_MemQueue(char * addr, size_t len) : _addr(addr) {
        init(addr, len);
    }

    TL_MemQueue::~TL_MemQueue() {
    }

    void TL_MemQueue::init(char* addr, size_t len) {
        assert(len > sizeof (HEAD_BLOCK));
        _head = (HEAD_BLOCK *) _addr;
        _head->memsize = len - sizeof (HEAD_BLOCK);
        _head->top = 0;
        _head->bot = 0;

        _addr += sizeof (HEAD_BLOCK);
        _addrend = _addr + len;
    }

    void TL_MemQueue::connect(char* addr, size_t len) {
        _head = (HEAD_BLOCK*) addr;
        if (_head->memsize != len - sizeof (HEAD_BLOCK)) {
            throw TL_Exp("TL_MemQueue::connect size is error.");
        }
        _addr = addr + sizeof (HEAD_BLOCK);
        _addrend = _addr + len;
    }

    bool TL_MemQueue::push_back(const char* data, size_t len) {
        if (isFull(len)) {
            return false;
        }
        size_t i = _head->memsize - _head->bot;
        if ((int)i < 0) {
            throw TL_Exp("[TL_MemQueue::push_back] bot is overflow.");
        }
        if (i == 0) {
            memcpy(_addr, (void*) &len, sizeof (size_t));
            memcpy(_addr + sizeof (size_t), data, len);
            _head->bot = sizeof (size_t) + len;
        } else if (i < sizeof (size_t)) {
            memcpy(_addr + _head->bot, (char*) &len, i);
            memcpy(_addr, ((char*) &len) + i, sizeof (size_t) - i);
            i = sizeof (size_t) - i;
            memcpy(_addr + i, data, len);
            _head->bot = i + len;
        } else if (i >= sizeof (size_t)) {
            memcpy(_addr + _head->bot, (void*) &len, sizeof (size_t));
            i = i - sizeof (size_t);
            if (i == 0) {
                memcpy(_addr, data, len);
                _head->bot = len;
            } else if (i < len) {
                memcpy(_addr + _head->bot + sizeof (size_t), data, i);
                memcpy(_addr, data + i, len - i);
                _head->bot = len - i;
            } else if (i == len) {
                memcpy(_addr + _head->bot + sizeof (size_t), data, i);
                _head->bot = 0;
            } else { //i > len
                memcpy(_addr + _head->bot + sizeof (size_t), data, len);
                _head->bot += sizeof (size_t) + len;
            }
        }
        return true;
    }

    bool TL_MemQueue::pop_front(char*data, size_t & ilen) {
        if (isEmpty()) {
            return false;
        }
        size_t len = 0;
        size_t i = _head->memsize - _head->top;
        if ((int)i < 0) {
            throw TL_Exp("[TL_MemQueue::push_back] bot is overflow.");
        }
        if (i == 0) {
            memcpy((void*) &len, _addr, sizeof (size_t));
            memcpy(data, _addr + sizeof (size_t), len);
            _head->top = sizeof (size_t) + len;
        } else if (i < sizeof (size_t)) {
            memcpy((char*) &len, _addr + _head->top, i);
            memcpy(((char*) &len) + i, _addr, sizeof (size_t) - i);
            memcpy(data, _addr + sizeof (size_t) - i, len);
            _head->top = sizeof (size_t) - i + len;
        } else { // i >= sizeof(size_t)
            memcpy((void*) &len, _addr + _head->top, sizeof (size_t));
            i -= sizeof (size_t);
            if (i > len) {
                memcpy(data, _addr + _head->top + sizeof (size_t), len);
                _head->top += sizeof (size_t) + len;
            } else if (i == len) {
                memcpy(data, _addr + _head->top + sizeof (size_t), len);
                _head->top = 0;
            } else if (i == 0) {
                memcpy(data, _addr, len);
                _head->top = len;
            } else {
                memcpy(data, _addr + _head->top + sizeof (size_t), i);
                memcpy(data + i, _addr, len - i);
                _head->top = len - i;
            }
        }
        ilen = len;
        return true;
    }

    bool TL_MemQueue::isFull(size_t len) {
        if (_head->top <= _head->bot) {
            return (_head->memsize - _head->bot + _head->top) < (sizeof (size_t) + len);
        } else {
            return (_head->top - _head->bot) < (sizeof (size_t) + len);
        }
    }

    bool TL_MemQueue::isEmpty() {
        return _head->top == _head->bot;
    }
}
