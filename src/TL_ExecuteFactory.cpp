/*
 * TL_ExecuteFactory.cpp
 *
 *  Created on: Aug 12, 2014
 *      Author: ever
 */

#include "TL_ExecuteFactory.h"
namespace tidp {
TL_ExecuteFactory::TL_ExecuteFactory() {
}

TL_ExecuteFactory::~TL_ExecuteFactory() {
}

TL_ExecutePtr TL_ExecuteFactory::create(unsigned command) {
	if (command == (unsigned) -1) {
		return TL_ExecutePtr(new TL_Execute());
	}
	return TL_ExecutePtr();
}
}
