/*
 * TL_TL_TSD.h
 *
 *  Created on: 2013-8-29
 *      Author: ever
 */

#ifndef TL_TSD_H_
#define TL_TSD_H_
#include <pthread.h>
#include <set>
//thread store data.
namespace tidp {
template<class T>
class TL_TSD {
public:
	typedef pthread_key_t key_type;
	typedef T element_type;
	typedef void (*cleanup_fun_type)(T *);
	typedef void (*key_fun_type)(T *);
	explicit TL_TSD();
	explicit TL_TSD(void (*cf)(T *));
	virtual ~TL_TSD();

	key_type getKey() {
		return _key;
	}
	static void default_cleanup_fun(T * data) {
		if (data) {
			delete data;
			data = NULL;
		}
	}
	void reset(T * v) {
		T * before = get();
		if (v != before && before && _cf) {
			_cf(get());
		}
		pthread_setspecific(_key, v);
	}
	cleanup_fun_type getCF() {
		return _cf;
	}
	T* get() const {
		if (_status != 0)
			return NULL;
		return (T*) pthread_getspecific(_key);
	}
	T* operator->() const {
		return get();
	}
	T& operator*() const {
		return *get();
	}
private:
	int _status;
	key_type _key;
	cleanup_fun_type _cf;
};

template<class T> TL_TSD<T>::TL_TSD() {
	_status = -1;
	_key = 0;
	_cf = TL_TSD::default_cleanup_fun;
	_status = pthread_key_create(&_key, (void (*)(void*)) _cf);}

template<class T> TL_TSD<T>::TL_TSD(void (*cf)(T *)) {
	//_status = -1;
	_key = 0;
	_cf = cf;
	_status = pthread_key_create(&_key, (void (*)(void*)) _cf);}

template<class T> TL_TSD<T>::~TL_TSD() {
	if (_status == 0) {
		pthread_key_delete(_key);
	}
}
}
#endif /* TL_TSD_H_ */
