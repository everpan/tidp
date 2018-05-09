/*
 * TL_Pool.h
 *
 *  Created on: Nov 10, 2014
 *      Author: ever
 */

#ifndef TL_POOL_H_
#define TL_POOL_H_

#include "config.h"
#if USE_BOOST
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
using namespace boost;
#else
#include <memory>
#include <functional>
//namespace boost=std;
#endif
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
			_max(10), _init_fun(TL_Pool<T, C>::defaultInit) {

	}
	void setMax(unsigned int size) {
		_max = size;
	}
	unsigned int getMax() {
		return _max;
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
				cout << "get from create " << t << endl;
				return t;
			}
		}
		typename set<T *>::iterator it = _idle.begin();
		T * t = *it;
		_idle.erase(it);
		_busy.insert(t);
		cout << "get from cache " << t << endl;
		return t;
	}

	void release(T * t) {
		Lock lk(*this);
		cout << "release :" << t << endl;
		_busy.erase(t);
		_idle.insert(t);
	}

	class ScopePtr {
	public:
		ScopePtr() :
				_t(pool()->get()) {

		}
		~ScopePtr() {
			pool()->release(_t);
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
		ScopePtr(const ScopePtr&);
		ScopePtr & operator =(const ScopePtr&);
	private:
		T * _t;
	};
/*
	class SharedPtr {
	public:
		SharedPtr() :
				_pn(1), _t(pool()->get()) {

		}
		~SharedPtr() {
			int _n = __sync_sub_and_fetch(&_pn, 1);
			if (_n == 0 && _t) {
				pool()->release(_t);
				_t = NULL;
			}
		}
		static TL_Pool<T, C> * pool() {
			return TL_Pool<T, C>::getInstance();
		}

		SharedPtr(const SharedPtr& sp) {
			_pn = __sync_add_and_fetch(const_cast<volatile void*>(&sp._pn), 1);
			_t(sp._t);
		}
		//这种方式好像有不必要的申请，不建议使用。
		const SharedPtr& operator =(const SharedPtr& sp) {
			this->~SharedPtr();
			_pn = __sync_add_and_fetch(&sp._pn, 1);
			_t(sp._t);
			return *this;
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

	private:
		int _pn;
		T * _t;
	};
*/
	typedef function<void(T *)> init_function_t;
	void setInitFunction(init_function_t f) {
		_init_fun = f;
	}

private:
	static void defaultInit(T * t) {
		(void) t;
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
