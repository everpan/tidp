/* 
 * File:   TL_DictMap.h
 * Author: everpan
 *
 * Created on 2011年6月22日, 下午3:41
 */

#ifndef TL_DICTMAP_H
#define	TL_DICTMAP_H
#include <map>
#include <stdlib.h>
#include "TL_Exp.h"
namespace tidp {

    template <class T>
    class TL_DictMap {
    public:

        TL_DictMap() {
            _kv.clear();
            _vk.clear();
            _k = 0;
            _min_k = 0;
        }

        void erase(unsigned int k) {
            std::map<unsigned int, T>::iterator it = _kv.find(k);
            if (it != _kv.end()) {
                _kv.erase(it);
                _vk.erase(_kv[k]);
            }
        }

        T operator[](unsigned int k) {
            typename std::map<unsigned int, T>::iterator it = _kv.find(k);
            if (it == _kv.end()) {
                char c[100];
                sprintf(c, "TL_DictMap::can't find key[%d].", k);
                throw TL_Exp(c);
            }
            return it->second;
        }

        unsigned int operator[](const T& v) {
            std::map<T, unsigned int>::iterator it = _vk.find(v);
            if (it == _vk.end()) {
                char c[100];
                sprintf(c, "TL_DictMap::can't find value");
                //todo
                throw TL_Exp(c);
            }
            return 0;
        }

        void add(unsigned k, const T& v) {
            if(k==(unsigned int)(-1)){
                throw TL_Exp("TL_DictMap::add the key is invaild.k=unsigned int)(-1)");
            }
            typename std::map<unsigned int, T>::iterator it = _kv.find(k);
            if (it != _kv.end() && it->second != v) {
                _vk.erase(_kv[k]);
            }
            _kv[k] = v;
            _vk[v] = k;
        }

        unsigned int add(const T& v) {
            typename std::map<T, unsigned int>::iterator it = _vk.find(v);
            if (it == _vk.end()) {
                ++_k;
                while (_kv.find(_k) != _kv.end()) {
                    ++_k;
                }
                _kv[_k] = v;
                _vk[v] = _k;
                return _k;
            }
            return it->second;
        }
        unsigned int findk(const T& v) {
            typename std::map<T, unsigned int>::iterator it = _vk.find(v);
            if (it == _vk.end()) {
                return (unsigned int)(-1);
            }
            return it->second;
        }
        /*
        T findV(unsigned int k){
            
        }*/
    private:
        std::map<unsigned int, T> _kv;
        std::map<T, unsigned int> _vk;
        unsigned int _k;
        unsigned int _min_k;
    };
}

#endif	/* TL_DICTMAP_H */

