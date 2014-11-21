/*
 * TL_Pool.h
 *
 *  Created on: Nov 10, 2014
 *      Author: ever
 */

#ifndef TL_POOL_H_
#define TL_POOL_H_

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <set>
#include "TL_Singleton.h"
using namespace std;
namespace tidp {
template<typename T>
class TL_PoolDefaultInit {
public:
	static T * create() {
		return new T;
	}
	static void destory(T * t) {
		delete t;
	}
};
template<class T, template<class > class C = TL_PoolDefaultInit>
class TL_Pool: public TL_Singleton<TL_Pool<T, C> >, public TL_ThreadLock {
public:
	TL_Pool() :
			_max(10) ,_init_fun(TL_Pool<T, C>::defaultInit){

	}
	T * get() {
		Lock lk(*this);
		while (_idle.size() == 0) {
			if (_busy.size() >= _max) {
				wait();
			} else {
				T * t = C<T>::create();
				_init_fun(t);
				_busy.insert(t);
				return t;
			}
		}
		typename set<T *>::iterator it = _idle.begin();
		T * t = *it;
		_idle.erase(it);
		_busy.insert(t);
		return t;
	}

	void release(T * t) {
		Lock lk(*this);
		_busy.erase(t);
		_idle.insert(t);
	}

	class Ptr {
	public:
		Ptr() :
				_t(TL_Pool<T, C>::getInstance()->get()) {

		}
		~Ptr() {
			TL_Pool<T, C>::getInstance()->release(_t);
			_t = NULL;
		}
		static TL_Pool<T, C> * pool() {
			return TL_Pool<T, C>::getInstance();
		}

		T * operator ->() {
			return _t;
		}
		T & operator *() {
			return *_t;
		}
		T * operator ->() const {
			return _t;
		}
		T & operator *() const {
			return *_t;
		}
	protected:
		Ptr(const Ptr&);
		Ptr & operator = (const Ptr&);
	private:
		T * _t;
	};
	typedef typename boost::function<void(T *)> init_function_t;
	void setInitFunction(init_function_t f){
		_init_fun = f;
	}

private:
	static void defaultInit(T * t){
		//cout << "default: init" << endl;
		(void)t;
	}
	void checkSize() {
		if (_busy.size() + _idle.size() > _max) {
			while (_idle.size() > 0) {

			}
		}
	}
	unsigned int _max;
	init_function_t _init_fun;
	set<T *> _idle;
	set<T *> _busy;

};

}
#endif /* TL_POOL_H_ */
