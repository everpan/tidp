/*
 * TL_Elapsed.cpp
 *
 *  Created on: Oct 17, 2014
 *      Author: ever
 */

#include <TL_Elapsed.h>

namespace tidp {

TL_Elapsed::TL_Elapsed() {
    // TODO Auto-generated constructor stub

}

TL_Elapsed::~TL_Elapsed() {
    // TODO Auto-generated destructor stub
}

void TL_Elapsed::start(int i) {
    gettimeofday(&_v[i].first, NULL);
}

void TL_Elapsed::end(int i) {
    gettimeofday(&_v[i].second, NULL);
}

double TL_Elapsed::elapsed(int i) {
    const struct timeval& tv1 = _v[i].first;
    const struct timeval& tv2 = _v[i].second;
    double s = (tv2.tv_sec - tv1.tv_sec) * 1000000.0 + (tv2.tv_usec - tv1.tv_usec);
    return s;
}
} /* namespace tidp */
