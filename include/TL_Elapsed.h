/*
 * TL_Elapsed.h
 *
 *  Created on: Oct 17, 2014
 *      Author: ever
 */

#ifndef TL_ELAPSED_H_
#define TL_ELAPSED_H_
#include <iostream>
#include <sys/time.h>
#include <map>
namespace tidp {
using namespace std;
class TL_Elapsed {
public:
	TL_Elapsed();
	virtual ~TL_Elapsed();

	void start(int i = 0);
	void end(int i = 0);
	double elapsed(int i = 0);
private:
	map<int, pair<struct timeval, struct timeval> > _v;
};

} /* namespace tidp */

#endif /* TL_ELAPSED_H_ */
