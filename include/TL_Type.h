/*
 * File:   TL_Type.h
 * Author: everpan
 *
 * Created on 2011年3月9日, 下午9:41
 */

#ifndef TL_TYPE_H
#define	TL_TYPE_H



#include <arpa/inet.h>


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <set>
#include <list>

#include "TL_Exp.h"

#undef TL_LITTLE_ENDIAN
//#define TL_LITTLE_ENDIAN
#ifndef __CYGWIN__

static __inline unsigned short
bswap_16(unsigned short __x) {
    return (__x >> 8) | (__x << 8);
}

static __inline unsigned int
bswap_32(unsigned int __x) {
    return (bswap_16(__x & 0xffff) << 16) | (bswap_16(__x >> 16));
}

static __inline unsigned long long
bswap_64(unsigned long long __x) {
    return (((unsigned long long) bswap_32(__x & 0xffffffffull)) << 32) | (bswap_32(__x >> 32));
}
#endif

namespace tidp {

    template <class T> inline T swap(const T t) {
        return t;
    }

    template <> __inline unsigned short swap(const unsigned short t) {
        return bswap_16(t);
    }

    template <> inline unsigned int swap(const unsigned int t) {
        return bswap_32(t);
    }

    template <> inline unsigned long long swap(const unsigned long long t) {
        return bswap_64(t);
    }

    template <> inline short swap(const short t) {
        return bswap_16(t);
    }

    template <> inline int swap(const int t) {
        return bswap_32(t);
    }

    template <> inline long long swap(const long long t) {
        return bswap_64(t);
    }

    /**
 该类中的buffer均未检测长度 由调用者负责
     */
    /*
    template <class T>
    class TL_TYPE : public TL_TYPE_T {
    public:

        TL_TYPE() {
        };

        TL_TYPE(T t) {
            _value = t;
        }
        //TL_TYPE(const TL_TYPE& orig);

        virtual ~TL_TYPE() {
        };

        typedef T vType;

        virtual size_t byteSize() {
            return sizeof (T);
        }

        virtual char* serialization(char* buffer) {
#ifdef TL_LITTLE_ENDIAN
     *(T*) buffer = _value;
#else
     *(T*) buffer = swap(_value);
#endif
            return buffer + sizeof (T);
        }

        virtual char* serialization(char* buffer) const {
#ifdef TL_LITTLE_ENDIAN
     *(T*) buffer = _value;
#else
     *(T*) buffer = swap(_value);
#endif

            return buffer + sizeof (T);
        }

        virtual const char* unserialization(const char* buffer) {
#ifdef TL_LITTLE_ENDIAN
            _value = *(T*) buffer;
#else
            T v = *(T*) buffer;
            _value = swap(v);
#endif
            return buffer + sizeof (T);
        }

        virtual void parse(const char* buffer, size_t len) {
            _value = (vType) strtoll(buffer, 0, 10);
        }

        virtual std::string toString() {
            static char c[64];
            memset(c, 0, 64);
            //int len = 64;
            char* p = toString(c);
            return std::string(c, p);
        }

        virtual std::string toString() const {
            static char c[64];
            memset(c, 0, 64);
            //int len = 64;
            char* p = toString(c);
            return std::string(c, p);
        }

        virtual char* toString(char* buffer) {
            int ret = sprintf(buffer, "%llu", (unsigned long long) _value);
            return buffer + ret;
        }

        virtual char* toString(char* buffer) const {
            int ret = sprintf(buffer, "%llu", (unsigned long long) _value);
            return buffer + ret;
        }

        virtual int hash() {
            return (int) _value;
        }

        vType getValue() {
            return _value;
        }

        vType getValue() const {
            return _value;
        }

        void setValue(const vType &v) {
            _value = v;
        }

        TL_TYPE<T> operator +(const TL_TYPE<T> &t) {
            TL_TYPE<T> t1;
            t1._value = this->_value + t._value;
            return t1;
        }

        TL_TYPE<T> & operator +=(const TL_TYPE<T> &t) {
            this->_value += t._value;
            return *this;
        }

        TL_TYPE<T> operator -(const TL_TYPE<T> &t) {
            TL_TYPE<T> t1;
            t1._value = this->_value - t._value;
            return t1;
        }

        TL_TYPE<T> & operator -=(const TL_TYPE<T> &t) {
            this->_value -= t._value;
            return *this;
        }

        TL_TYPE<T> operator *(const TL_TYPE<T> &t) {
            TL_TYPE<T> t1;
            t1._value = this->_value * t._value;
            return t1;
        }

        TL_TYPE<T> & operator *=(const TL_TYPE<T> &t) {
            this->_value *= t._value;
            return *this;
        }

        TL_TYPE<T> operator /(const TL_TYPE<T> &t) {
            if (t._value == 0) {
                //todo throw
            }
            TL_TYPE<T> t1;
            t1._value = this->_value / t._value;
            return t1;
        }

        TL_TYPE<T> & operator /=(const TL_TYPE<T> &t) {
            if (t._value == 0) {
                //todo throw
            }
            this->_value /= t._value;
            return *this;
        }

        TL_TYPE<T> & operator =(const TL_TYPE<T> & t) {
            _value = t._value;
            return *this;
        }

        const bool operator<(const TL_TYPE<T> & t) const {
            return this->_value < t._value;
        }

        const bool operator==(const TL_TYPE<T> &t) const {
            return this->_value == t._value;
        }

    private:
        vType _value;
    };

    template <> inline char* TL_TYPE<char>::toString(char* buffer) {
        int ret = sprintf(buffer, "%d", (int) _value);
        return buffer + ret;
    }

    template <> inline char* TL_TYPE<short>::toString(char* buffer) {
        int ret = sprintf(buffer, "%d", (int) _value);
        return buffer + ret;
    }

    template <> inline char* TL_TYPE<int>::toString(char* buffer) {
        int ret = sprintf(buffer, "%d", (int) _value);
        return buffer + ret;
    }

    template <> inline char* TL_TYPE<long>::toString(char* buffer) {
        int ret = sprintf(buffer, "%ld", _value);
        return buffer + ret;
    }

    template <> inline char* TL_TYPE<long long>::toString(char* buffer) {
        int ret = sprintf(buffer, "%lld", _value);
        return buffer + ret;
    }



    typedef TL_TYPE<char> TL_Sint8;
    typedef TL_TYPE<short> TL_Sint16;
    typedef TL_TYPE<int> TL_Sint32;
    typedef TL_TYPE<long long> TL_Sint64;

    typedef TL_TYPE<unsigned char> TL_Uint8;
    typedef TL_TYPE<unsigned short> TL_Uint16;
    typedef TL_TYPE<unsigned int> TL_Uint32;
    typedef TL_TYPE<unsigned long long> TL_Uint64;

    typedef TL_TYPE<time_t> TL_TIME;
    typedef TL_TYPE<pid_t> TL_PIDT;
    //----------- TL_String --------------------------------------------------------

    template<class U = TL_Uint8>
    class TL_String : public TL_TYPE_T {
    public:
        typedef std::string vType;

        TL_String() {
        }

        TL_String(const TL_String& orig) {
            this->_value = orig._value;
        }

        TL_String(const char* s) {
            _value.assign(s);
        }

        TL_String(const char*s, size_t l) {
            _value.assign(s, l);
        }

        vType getValue() {
            return _value;
        }

        void setValue(const vType& v) {
            _value = v;
        }

        virtual ~TL_String() {
        }

        virtual void parse(const char* buffer, size_t len) {
            _value.assign(buffer, len);
        }

        virtual char* serialization(char* buffer) {
            U l(_value.size());
            char* p = l.serialization(buffer);
            memcpy(p, _value.c_str(), l.getValue());
            return p + l.getValue();
        }

        virtual char* serialization(char* buffer) const {
            U l(_value.size());
            char* p = l.serialization(buffer);
            memcpy(p, _value.c_str(), l.getValue());
            return p + l.getValue();
        }

        virtual const char* unserialization(const char* buffer) {
            U l;
            const char*p = l.unserialization(buffer);
            _value.clear();
            _value.assign(p, l.getValue());
            return p + l.getValue();
        }

        virtual std::string toString() const {
            return _value;
        }

        virtual std::string toString() {
            return _value;
        }

        virtual char* toString(char* buffer) {
            memcpy(buffer, (void*) _value.c_str(), _value.size());
            return buffer + _value.size();
        }

        virtual char* toString(char* buffer) const {
            memcpy(buffer, (void*) _value.c_str(), _value.size());
            return buffer + _value.size();
        }

        virtual int hash() {
            //todo
            return 0;
        }

        virtual size_t byteSize() {
            return sizeof (typename U::vType) + _value.size();
        }

        TL_String operator +(const TL_String& s1) {
            TL_String s2;
            s2._value.assign(this->_value);
            s2._value.append(s1._value);
            return s2;
        }

        TL_String & operator=(const TL_String& s) {
            _value = s._value;
            return *this;
        }

        TL_String & operator+=(const TL_String& s) {
            this->_value.append(s._value);
            return *this;
        }

        const bool operator<(const TL_String& s) const {
            return this->_value < s._value ? true : false;
        }
    private:
        vType _value;
    };
     */
    //--------------------------------------------------------------------------
    /*
        template <class T, class U>
        char * serializationMap(char* buffer, const std::map<T, U>& m) {
            TL_Uint32 l(m.size());
            char* p = l.serialization(buffer);

            typename std::map<T, U>::const_iterator it = m.begin();
            while (it != m.end()) {
                p = it->first.serialization(p);
                p = it->second.serialization(p);
                ++it;
            }
            return p;
        }

        template <class T, class U>
        const char * unserializationMap(const char* buffer, std::map<T, U>& m) {
            TL_Uint32 l;
            const char* p = l.unserialization(buffer);
            size_t length = l.getValue();
            size_t i = 0;
            T t;
            U u;
            m.clear();
            while (i < length) {
                p = t.unserialization(p);
                p = u.unserialization(p);
                m.insert(std::make_pair(t, u));
                ++i;
            }
            return p;
        }

        //基本类型必须支持统一接口

        template <class T>
        char * serializationVector(char * buffer, const std::vector<T>& v) {
            TL_Uint32 l(v.size());
            char * p = l.serialization(buffer);
            typename std::vector<T>::const_iterator it = v.begin();
            while (it != v.end()) {
                p = serialization(p, *it);
                ++it;
            }
            return p;
        }

        template <class T>
        const char* unserializationVector(const char* buffer, std::vector<T>& v) {
            TL_Uint32 l;
            const char* p = l.unserialization(buffer);
            size_t len = l.getValue();
            size_t i = 0;
            T t;
            while (i < len) {
                p = unserialization(p, t);
                v.push_back(t);
                ++i;
            }
            return p;
        }

        template <class T>
        char * serializationSet(char * buffer, const std::set<T>& v) {
            TL_Uint32 l(v.size());
            char * p = l.serialization(buffer);
            typename std::set<T>::const_iterator it = v.begin();
            while (it != v.end()) {
                p = serialization(p, *it);
                ++it;
            }
            return p;
        }

        template <class T>
        const char* unserializationSet(const char* buffer, std::set<T>& v) {
            TL_Uint32 l;
            const char* p = l.unserialization(buffer);
            size_t len = l.getValue();
            size_t i = 0;
            T t;
            while (i < len) {
                p = unserialization(p, t);
                v.insert(t);
                ++i;
            }
            return p;
        }
     */

    //基本数据类型的序列化
    //为了效率统一不做越界检测
    // char short int longlong string

    /*
    template <class T> inline
    char* serialization(char* buffer, T& t) {
        const T tmp = t;
        return tmp.serialization(buffer);
        //throw TL_Exp("unsupport type.");
    }

    template <class T> inline
    char* serialization(char* buffer, const T& t) {
        return t.serialization(buffer);
    }

     */
    //serialization

    template <class T> inline
    char* serialization(char* buffer, const T& t) {
        return const_cast<T&> (t).serialization(buffer);
    }
    //char

    template <> inline
    char* serialization(char* buffer, const char& t) {
#ifdef TL_LITTLE_ENDIAN
        *(char*) buffer = t;
#else
        *(char*) buffer = swap(t);
#endif
        return buffer + sizeof (char);
    }
    //unsigned char

    template <> inline
    char* serialization(char* buffer, const unsigned char& t) {
#ifdef TL_LITTLE_ENDIAN
        *(unsigned char*) buffer = t;
#else
        *(unsigned char*) buffer = swap(t);
#endif
        return buffer + sizeof (unsigned char);
    }
    //short

    template <> inline
    char* serialization(char* buffer, const short& t) {
#ifdef TL_LITTLE_ENDIAN
        *(short*) buffer = t;
#else
        *(short*) buffer = swap(t);
#endif
        return buffer + sizeof (short);
    }
    //unsigned short

    template <> inline
    char* serialization(char* buffer, const unsigned short& t) {
#ifdef TL_LITTLE_ENDIAN
        *(unsigned short*) buffer = t;
#else
        *(unsigned short*) buffer = swap(t);
#endif
        return buffer + sizeof (unsigned short);
    }
    //int

    template <> inline
    char* serialization(char* buffer, const int& t) {
#ifdef TL_LITTLE_ENDIAN
        *(int*) buffer = t;
#else
        *((int*) buffer) = swap(t);
#endif
        return buffer + sizeof (int);
    }
    //unsigned int

    template <> inline
    char* serialization(char* buffer, const unsigned int& t) {
#ifdef TL_LITTLE_ENDIAN
        *(unsigned int*) buffer = t;
#else
        *(unsigned int*) buffer = swap(t);
#endif
        return buffer + sizeof (unsigned int);
    }
    //long

    template <> inline
    char* serialization(char* buffer, const long& t) {
#ifdef TL_LITTLE_ENDIAN
        *(long*) buffer = t;
#else
        *(long*) buffer = swap(t);
#endif
        return buffer + sizeof (long);
    }
    //unsigned long

    template <> inline
    char* serialization(char* buffer, const unsigned long& t) {
#ifdef TL_LITTLE_ENDIAN
        *(unsigned long*) buffer = t;
#else
        *(unsigned long*) buffer = swap(t);
#endif
        return buffer + sizeof (unsigned long);
    }
    //long long

    template <> inline
    char* serialization(char* buffer, const long long& t) {
#ifdef TL_LITTLE_ENDIAN
        *(long long *) buffer = t;
#else
        *(long long *) buffer = swap(t);
#endif
        return buffer + sizeof ( long long);
    }
    //unsigned long long

    template <> inline
    char* serialization(char* buffer, const unsigned long long& t) {
#ifdef TL_LITTLE_ENDIAN
        *(unsigned long long *) buffer = t;
#else
        *(unsigned long long *) buffer = swap(t);
#endif
        return buffer + sizeof (unsigned long long);
    }
    //string

    template <> inline
    char* serialization(char* buffer, const std::string& t) {
        std::string::size_type n = t.size();
        char * p = serialization(buffer, n);
        memcpy(p, t.c_str(), n);
        return p + n;
    }
    //vector

    template <class T> inline
    char* serialization(char* buffer, const std::vector<T>& v) {
        char * p = serialization(buffer, v.size());
        typename std::vector<T>::iterator it = v.begin();
        typename std::vector<T>::iterator itend = v.end();
        while (it != itend) {
            p = serialization(p, *it);
            ++it;
        }
        return p;
    };
    //set

    template <class T> inline
    char* serialization(char* buffer, const std::set<T>& s) {
        char * p = serialization(buffer, s.size());
        typename std::set<T>::iterator it = s.begin();
        typename std::set<T>::iterator itend = s.end();
        while (it != itend) {
            p = serialization(p, *it);
            ++it;
        }
        return p;
    };
    //map

    template<class K, class V> inline
    char* serialization(char* buffer, const std::map<K, V>& m) {
        using namespace std;
        char * p = serialization(buffer, m.size());
        typename std::map<K, V>::const_iterator it = m.begin();
        while (it != m.end()) {
            p = serialization(p, it->first);
            p = serialization(p, it->second);
            ++it;
        }
        return p;
    }
    //list

    template<class T> inline
    char* serialization(char* buffer, const std::list<T>& l) {
        char *p = serialization(buffer, l.size());
        typename std::list<T>::const_iterator it = l.begin();
        while (it != l.end()) {
            p = serialization(p, *it);
            ++it;
        }
        return p;
    }
    //unserialization

    template <class T> inline
    const char* unserialization(const char* buffer, T& t) {
        return t.unserialization(buffer);
    }

    template <> inline
    const char* unserialization(const char* buffer, char & t) {
#ifdef TL_LITTLE_ENDIAN
        t = *(char*) buffer;
#else
        t = *(char*) buffer;
        t = swap(t);
#endif
        return buffer + sizeof (char);
    }

    template <> inline
    const char* unserialization(const char* buffer, unsigned char & t) {
#ifdef TL_LITTLE_ENDIAN
        t = *(unsigned char*) buffer;
#else
        t = *(unsigned char*) buffer;
        t = swap(t);
#endif
        return buffer + sizeof (unsigned char);
    }

    template <> inline
    const char* unserialization(const char* buffer, short & t) {
#ifdef TL_LITTLE_ENDIAN
        t = *(short*) buffer;
#else
        t = *(short*) buffer;
        t = swap(t);
#endif
        return buffer + sizeof (short);
    }

    template <> inline
    const char* unserialization(const char* buffer, unsigned short & t) {
#ifdef TL_LITTLE_ENDIAN
        t = *(unsigned short*) buffer;
#else
        t = *(unsigned short*) buffer;
        t = swap(t);
#endif
        return buffer + sizeof (unsigned short);
    }

    template <> inline
    const char* unserialization(const char* buffer, int & t) {
#ifdef TL_LITTLE_ENDIAN
        t = *(int*) buffer;
#else
        t = *(int*) buffer;
        t = swap(t);
#endif
        return buffer + sizeof (int);
    }

    template <> inline
    const char* unserialization(const char* buffer, unsigned int & t) {
#ifdef TL_LITTLE_ENDIAN
        t = *(unsigned int*) buffer;
#else
        t = *(unsigned int*) buffer;
        t = swap(t);
#endif
        return buffer + sizeof (unsigned int);
    }

    template <> inline
    const char* unserialization(const char* buffer, long & t) {
#ifdef TL_LITTLE_ENDIAN
        t = *(long*) buffer;
#else
        t = *(long*) buffer;
        t = swap(t);
#endif
        return buffer + sizeof (long);
    }

    template <> inline
    const char* unserialization(const char* buffer, unsigned long & t) {
#ifdef TL_LITTLE_ENDIAN
        t = *(unsigned long*) buffer;
#else
        t = *(unsigned long*) buffer;
        t = swap(t);
#endif
        return buffer + sizeof (unsigned long);
    }

    template <> inline
    const char* unserialization(const char* buffer, long long & t) {
#ifdef TL_LITTLE_ENDIAN
        t = *(long long*) buffer;
#else
        t = *(long long*) buffer;
        t = swap(t);
#endif
        return buffer + sizeof (long long);
    }

    template <> inline
    const char* unserialization(const char* buffer, unsigned long long & t) {
#ifdef TL_LITTLE_ENDIAN
        t = *(unsigned long long*) buffer;
#else
        t = *(unsigned long long*) buffer;
        t = swap(t);
#endif
        return buffer + sizeof (unsigned long long);
    }

    template <> inline
    const char* unserialization(const char* buffer, std::string& t) {
        std::string::size_type n = 0;
        const char * p = unserialization(buffer, n);
        t.assign(p, n);
        return p + n;
    }

    template <class T> inline
    const char* unserialization(const char* buffer, std::vector<T>& v) {
        v.clear();
        typename std::vector<T>::size_type n = 0;
        const char * p = unserialization(buffer, n);
        T val;
        while (n > 0) {
            p = unserialization(p, val);
            v.push_back(val);
            --n;
        }
        return p;
    };

    template <class T> inline
    const char* unserialization(const char* buffer, std::set<T>& s) {
        s.clear();
        typename std::set<T>::size_type n = 0;
        const char * p = unserialization(buffer, n);
        T val;
        while (n > 0) {
            p = unserialization(p, val);
            s.insert(val);
            --n;
        }
        return p;
    };

    template<class K, class V> inline
    const char* unserialization(const char* buffer, std::map<K, V>& m) {
        typename std::map<K, V>::size_type n = 0;
        const char * p = unserialization(buffer, n);
        using namespace std;
        m.clear();
        K key;
        V val;
        while (n > 0) {
            p = unserialization(p, key);
            p = unserialization(p, val);
            m[key] = val;
            --n;
        };
        return p;
    }

    template<class T> inline
    const char* unserialization(const char* buffer, std::list<T>& l) {
        l.clear();
        typename std::list<T>::size_type n = 0;
        const char *p = unserialization(buffer, n);
        T t;
        for (typename std::list<T>::size_type i = 0;
                i < n; ++i) {
            p = unserialization(p, t);
            l.push_back(t);
        }
        return p;
    }
}
#endif	/* TL_TYPE_H */

