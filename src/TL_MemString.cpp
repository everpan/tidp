/*
 * File:   TL_MemString.cpp
 * Author: everpan
 *
 * Created on 2011年6月7日, 下午8:30
 */

#include "TL_MemString.h"
namespace tidp {

    TL_MemString::~TL_MemString() {

    }

    int TL_MemString::init(size_t len) {
        destory();
        _len = len;
        _addr = (char*) malloc(len);
        _top = _bot = 0;

        return 0;
    }

    int TL_MemString::resize(size_t len) {
        //外部的内存 不能扩展。
        if (_isattache) return 0;
        if (len > _len) {
            //放大
            char * newaddr = (char*) malloc(len);
            if (_top < _bot) {
                memcpy(newaddr, _addr + _top, _bot - _top);
                _top = 0;
                _bot -= _top;
            } else if (_top > _bot) {
                size_t size = _len - _top;
                memcpy(newaddr, _addr + _top, size);
                memcpy(newaddr + size, _addr, _bot);
                _top = 0;
                _bot += size;
            } else {
                _top = _bot = 0;
            }
            destory();
            _addr = newaddr;
            _len = len;
        } else if (len < _len) {
            //todo 缩小
        }
        return len;
    }

    void TL_MemString::attache(char* addr, size_t len) {
        destory();
        _addr = addr;
        _len = len;
        _top = _bot = 0;
        _isattache = true;
    }

    bool TL_MemString::isFull(const size_t &len) {
        return reserve() > 0;
    }

    bool TL_MemString::isEmpty() const {
        return _top == _bot;
    }

    void TL_MemString::destory() {
        if (!_isattache && _addr != NULL) {
            delete _addr;
            _addr = 0;
            _top = _bot = 0;
            _len = 0;
        }
    }

    void TL_MemString::clear() {
        _top = _bot = 0;
    }

    size_t TL_MemString::reserve() {
        //return _len - size();
        if (_bot > _top) {
            return _len - _bot + _top;
        } else {
            return _top - _bot;
        }
    }

    size_t TL_MemString::size() {
        if (_bot >= _top) return _bot - _top;
        else return _len - _top + _bot;
    }

    size_t TL_MemString::capacity() {
        return _len;
    }
    const char* TL_MemString::c_str(){
        return _addr;
    }
    void TL_MemString::append(const TL_MemString& mc) {
        if (!mc.isEmpty()) {
            if (mc._bot - mc._top > 0) {
                push_backNT(mc._addr + mc._top, mc._bot - mc._top);
            } else {
                push_backNT(mc._addr + mc._top, mc._len - mc._top);
                push_backNT(mc._addr, mc._bot);
            }
        }
    }

    /*
    void TL_MemString::setSplitChar(char c) {
        _split_char = c;
    }*/

    void TL_MemString::append(const std::string& s) {
        push_backNT(s.c_str(), s.size());
    }

    bool TL_MemString::push_backNT(const char* addr, size_t len) {
        if (len == 0) return true;
        if (_bot >= _top) {
            if (_len - _bot >= len) {
                memcpy(_addr + _bot, addr, len);
                _bot += len;
            } else if (_len - _bot + _top > len) {
                memcpy(_addr + _bot, addr, _len - _bot);
                _bot = len - _len + _bot;
                memcpy(_addr, addr + _len - _bot, _bot);
            } else {
                //空间不够，2倍扩展 一直扩展就有问题，需要业务自己处理
                resize(_len * 2);
                push_backNT(addr, len);
            }
        } else {
            if (_top - _bot >= len) {
                memcpy(_addr + _bot, addr, len);
                _bot += len;
            } else {
                if (_len > 20480) {
                    resize(_len + 2048);
                } else {
                    resize(_len * 2);
                }
                push_backNT(addr, len);
            }
        }
        return true;
    }
    //no throw & no check
    /*
    void TL_MemString::pop_frontNT(const char* addr, size_t& len) {

    }*/

    /**
     *  内存写文件，并重置缓存
     * @param fd
     */
    int TL_MemString::write(int fd) {
        size_t size = 0;
        if (_top < _bot) {
            size = _bot - _top;
            ::write(fd, _addr + _top, size);
        } else if (_top < _bot) {
            size = _len - _top;
            ::write(fd, _addr + _top, _len - _top);
            ::write(fd, _addr, _bot);
            size += _bot;
        }
        clear();
        return size;
    }

    TL_MemString & TL_MemString::operator=(const std::string& s){
        clear();
        push_backNT(s.c_str(),s.size());
        return *this;
    }

    TL_MemString & TL_MemString::operator<<(char n) {
        push_backNT(&n, 1);
        return *this;
    }

    TL_MemString & TL_MemString::operator<<(unsigned char n) {
        int i = sprintf(_tmp, "%d", n);
        push_backNT(_tmp, i);
        return *this;
    }

    TL_MemString & TL_MemString::operator<<(short n) {
        int i = sprintf(_tmp, "%d", n);
        push_backNT(_tmp, i);
        return *this;
    }

    TL_MemString & TL_MemString::operator<<(unsigned short n) {
        int i = sprintf(_tmp, "%u", n);
        push_backNT(_tmp, i);
        return *this;
    }

    TL_MemString & TL_MemString::operator<<(int n) {
        int i = sprintf(_tmp, "%d", n);
        push_backNT(_tmp, i);
        return *this;
    }

    TL_MemString & TL_MemString::operator<<(unsigned int n) {
        int i = sprintf(_tmp, "%u", n);
        push_backNT(_tmp, i);
        return *this;
    }

    TL_MemString & TL_MemString::operator<<(long n) {
        int i = sprintf(_tmp, "%ld", n);
        push_backNT(_tmp, i);
        return *this;
    }

    TL_MemString & TL_MemString::operator<<(unsigned long n) {
        int i = sprintf(_tmp, "%lu", n);
        push_backNT(_tmp, i);
        return *this;
    }

    TL_MemString & TL_MemString::operator<<(long long n) {
        int i = sprintf(_tmp, "%lld", n);
        push_backNT(_tmp, i);
        return *this;
    }

    TL_MemString & TL_MemString::operator<<(unsigned long long n) {
        int i = sprintf(_tmp, "%llu", n);
        push_backNT(_tmp, i);
        return *this;
    }

    TL_MemString & TL_MemString::operator<<(double n) {
        int i = sprintf(_tmp, "%f", n);
        push_backNT(_tmp, i);
        return *this;
    }

    TL_MemString & TL_MemString::operator<<(float n) {
        int i = sprintf(_tmp, "%f", n);
        push_backNT(_tmp, i);
        return *this;
    }

    TL_MemString & TL_MemString::operator<<(const char* s) {
        push_backNT(s, strlen(s));
        return *this;
    }
    TL_MemString & TL_MemString::operator<<(void * s) {
        int i = sprintf(_tmp, "0x%llx", (unsigned long long) s);
        push_backNT(_tmp, i);
        return *this;
    }

    TL_MemString & TL_MemString::operator<<(const std::string& s) {
        push_backNT(s.c_str(), s.size());
        return *this;
    }

    TL_MemString& TL_MemString::operator <<(TL_MemStringFUN fun) {
        fun(*this);
        return *this;
    }

    TL_MemString& endl(TL_MemString& orig) {
        orig << "\n";
        return orig;
    }
}
