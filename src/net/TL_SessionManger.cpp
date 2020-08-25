/*
 * TL_SessionManger.cpp
 *
 *  Created on: Dec 27, 2014
 *      Author: ever
 */

#include <net/TL_SessionManger.h>

namespace tidp {
namespace net {
static void TL_SessionPtrInitFunDefault(TL_SessionPtr& ptr) {
	(void) ptr;
}
TL_SessionManger::TL_SessionManger() {
	_initFn = TL_SessionPtrInitFunDefault;
	_releasFn = std::bind(&TL_SessionManger::releaseRawPtr, this, std::placeholders::_1);
}

TL_SessionManger::~TL_SessionManger() {
	// TODO Auto-generated destructor stub
}

TL_SessionPtr& TL_SessionManger::getIdleSession() {
	//Lock lk(*this);
	if (_idle.size() == 0) {
		TL_SessionPtr sp(new TL_Session, _releasFn);
		_initFn(sp);
		_busy.insert(sp);
		return const_cast<TL_SessionPtr&>(*_busy.find(sp));
	} else {
		std::set<TL_Session*>::iterator it = _idle.begin();
		TL_Session * spt = *it;
		TL_SessionPtr sp(spt, _releasFn);
		_busy.insert(sp);
		_idle.erase(it);
		return const_cast<TL_SessionPtr&>(*_busy.find(sp));
	}
}
void TL_SessionManger::release(TL_SessionPtr& spt) {
	LOG() << debug << "TL_SessionManger::release " << spt.get() << endl;
	spt->getSocket().close();
	_busy.erase(spt);
}
void TL_SessionManger::autoReleaseInvalidSession(){
	std::set<TL_SessionPtr,TL_SessionPtrLess>::iterator it = _busy.begin();
	while(it != _busy.end()){
		const TL_SessionPtr& ptr = *it;
		if(!ptr->isValid()){
			LOG() << debug << "auto Release Invalid Session:" << ptr->getId() << "@" << ptr.get() << "|" << ptr->getSocket().getHost() << endl;
			_busy.erase(it++);
		}else{
			++it;
		}
	}
}
void TL_SessionManger::releaseRawPtr(TL_Session* ptr) {
	LOG() << debug << "TL_SessionManger::releaseRawPtr " << ptr << endl;
	ptr->close();
	_idle.insert(ptr);
}
size_t TL_SessionManger::size() {
	Lock lk(*this);
	return _busy.size() + _idle.size();
}
size_t TL_SessionManger::idleSize() {
	Lock lk(*this);
	return _idle.size();
}
size_t TL_SessionManger::busySize() {
	Lock lk(*this);
	return _busy.size();
}
std::pair<size_t, size_t> TL_SessionManger::idleBusySize() {
	Lock lk(*this);
	return std::make_pair(_idle.size(), _busy.size());
}
} /* namespace net */
} /* namespace tidp */
