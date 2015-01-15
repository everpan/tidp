/*
 * TL_SessionManger.h
 *
 *  Created on: Dec 27, 2014
 *      Author: ever
 */

#ifndef INCLUDE_TL_SESSIONMANGER_H_
#define INCLUDE_TL_SESSIONMANGER_H_
#include <set>

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <net/TL_Session.h>
#include <TL_Logger.h>
#include <TL_ThreadLock.h>
#include <TL_Logger.h>
namespace tidp {
namespace net {
typedef boost::shared_ptr<TL_Session> TL_SessionPtr;
typedef boost::function<void (TL_SessionPtr& ptr)> TL_SessionPtrInitFun;
typedef boost::function<void (TL_Session* ptr) > TL_SessionReleaseFun;
struct TL_SessionPtrLess{
	bool operator ()(const TL_SessionPtr& x,const TL_SessionPtr& y){
		return x.get() < y.get();
	}
};
class TL_SessionManger : public TL_ThreadLock{
public:
	TL_SessionManger();
	virtual ~TL_SessionManger();

	TL_SessionPtr& getIdleSession();
	void release(TL_SessionPtr& spt);
	void autoReleaseInvalidSession();
	size_t size();
	size_t idleSize();
	size_t busySize();
	std::pair<size_t,size_t> idleBusySize();
protected:
	//no copy
	TL_SessionManger(const TL_SessionManger&);
	TL_SessionManger& operator = (const TL_SessionManger&);
private:
	void releaseRawPtr(TL_Session* ptr);
private:
	//初始化函数，默认不操作
	TL_SessionPtrInitFun _initFn;
	//释放函数
	TL_SessionReleaseFun _releasFn;
	std::set<TL_Session*> _idle;

	std::set<TL_SessionPtr,TL_SessionPtrLess> _busy;
};

} /* namespace net */
} /* namespace tidp */

#endif /* INCLUDE_TL_SESSIONMANGER_H_ */
