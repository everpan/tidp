/*
 * TL_ExecuteFactory.h
 *
 *  Created on: Dec 28, 2014
 *      Author: ever
 */

#ifndef INCLUDE_NET_TL_EXECUTEFACTORY_H_
#define INCLUDE_NET_TL_EXECUTEFACTORY_H_

#include <memory>
#include <net/TL_Execute.h>

namespace tidp {
    namespace net {
/*
class TL_Execute;
class TL_ExecuteThread;
class TL_ExecuteFactory;
class TL_ExecuteThreadFactory;
typedef std::shared_ptr<TL_Execute> TL_ExecutePtr;
typedef std::shared_ptr<TL_ExecuteThread> TL_ExecuteThreadPtr;
*/
        class TL_ExecuteFactory {
        public:
            TL_ExecuteFactory();

            virtual ~TL_ExecuteFactory();

            virtual TL_ExecutePtr create(unsigned int cmdid);
        };

        typedef std::shared_ptr<TL_ExecuteFactory> TL_ExecuteFactoryPtr;
    } /* namespace net */
} /* namespace tidp */

#endif /* INCLUDE_NET_TL_EXECUTEFACTORY_H_ */
