/* 
 * File:   TL_Functor.h
 * Author: everpan
 *
 * Created on 2011年6月20日, 下午4:25
 */

#ifndef TL_FUNCTOR_H
#define	TL_FUNCTOR_H
#include "TL_Loki.h"
namespace tidp {

    template <class R, class TList>
    struct TC_FunctorImp {
        virtual R operator()() = 0;
        virtual TC_FunctorImp * clone() const = 0;

        virtual ~TC_FunctorImp() {
        }
    };

    template<typename R>
    struct TC_FunctorImp<R, tidp::EmptyType> {
        virtual R operator()() = 0;
        virtual TC_FunctorImp * clone() const = 0;

        virtual ~TC_FunctorImp() {
        };
    };

    template<typename R, typename P1>
    struct TC_FunctorImp<R, tidp::TypeList_1(P1)> {
        virtual R operator()(typename tidp::TypeTraits<P1>::ReferenceType p1) = 0;
        virtual TC_FunctorImp * clone() const = 0;

        virtual ~TC_FunctorImp() {
        };
    };

    template<typename R, typename P1, typename P2>
    struct TC_FunctorImp<R, tidp::TYPELIST_2(P1, P2)> {
        virtual R operator()(typename tidp::TypeTraits<P1>::ReferenceType p1,
                typename tidp::TypeTraits<P2>::ReferenceType p2) = 0;
        virtual TC_FunctorImp * clone() const = 0;

        virtual ~TC_FunctorImp() {
        };
    };

    template<typename R, typename P1, typename P2, typename P3>
    struct TC_FunctorImp<R, tidp::TYPELIST_3(P1, P2, P3)> {
        virtual R operator()(typename tidp::TypeTraits<P1>::ReferenceType p1,
                typename tidp::TypeTraits<P2>::ReferenceType p2,
                typename tidp::TypeTraits<P3>::ReferenceType p3) = 0;
        virtual TC_FunctorImp * clone() const = 0;

        virtual ~TC_FunctorImp() {
        };
    };

    template<typename R, typename P1, typename P2, typename P3, typename P4>
    struct TC_FunctorImp<R, tidp::TYPELIST_4(P1, P2, P3, P4)> {
        virtual R operator()(typename tidp::TypeTraits<P1>::ReferenceType p1,
                typename tidp::TypeTraits<P2>::ReferenceType p2,
                typename tidp::TypeTraits<P3>::ReferenceType p3,
                typename tidp::TypeTraits<P4>::ReferenceType p4) = 0;
        virtual TC_FunctorImp * clone() const = 0;

        virtual ~TC_FunctorImp() {
        };
    };

    template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5>
    struct TC_FunctorImp<R, tidp::TYPELIST_5(P1, P2, P3, P4, P5)> {
        virtual R operator()(typename tidp::TypeTraits<P1>::ReferenceType p1,
                typename tidp::TypeTraits<P2>::ReferenceType p2,
                typename tidp::TypeTraits<P3>::ReferenceType p3,
                typename tidp::TypeTraits<P4>::ReferenceType p4,
                typename tidp::TypeTraits<P5>::ReferenceType p5) = 0;
        virtual TC_FunctorImp * clone() const = 0;

        virtual ~TC_FunctorImp() {
        };
    };

    template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
    struct TC_FunctorImp<R, tidp::TYPELIST_6(P1, P2, P3, P4, P5, P6)> {
        virtual R operator()(typename tidp::TypeTraits<P1>::ReferenceType p1,
                typename tidp::TypeTraits<P2>::ReferenceType p2,
                typename tidp::TypeTraits<P3>::ReferenceType p3,
                typename tidp::TypeTraits<P4>::ReferenceType p4,
                typename tidp::TypeTraits<P5>::ReferenceType p5,
                typename tidp::TypeTraits<P6>::ReferenceType p6) = 0;
        virtual TC_FunctorImp * clone() const = 0;

        virtual ~TC_FunctorImp() {
        };
    };

    template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
    struct TC_FunctorImp<R, tidp::TYPELIST_7(P1, P2, P3, P4, P5, P6, P7)> {
        virtual R operator()(typename tidp::TypeTraits<P1>::ReferenceType p1,
                typename tidp::TypeTraits<P2>::ReferenceType p2,
                typename tidp::TypeTraits<P3>::ReferenceType p3,
                typename tidp::TypeTraits<P4>::ReferenceType p4,
                typename tidp::TypeTraits<P5>::ReferenceType p5,
                typename tidp::TypeTraits<P6>::ReferenceType p6,
                typename tidp::TypeTraits<P7>::ReferenceType p7) = 0;
        virtual TC_FunctorImp * clone() const = 0;

        virtual ~TC_FunctorImp() {
        };
    };

    template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
    struct TC_FunctorImp<R, tidp::TYPELIST_8(P1, P2, P3, P4, P5, P6, P7, P8)> {
        virtual R operator()(typename tidp::TypeTraits<P1>::ReferenceType p1,
                typename tidp::TypeTraits<P2>::ReferenceType p2,
                typename tidp::TypeTraits<P3>::ReferenceType p3,
                typename tidp::TypeTraits<P4>::ReferenceType p4,
                typename tidp::TypeTraits<P5>::ReferenceType p5,
                typename tidp::TypeTraits<P6>::ReferenceType p6,
                typename tidp::TypeTraits<P7>::ReferenceType p7,
                typename tidp::TypeTraits<P8>::ReferenceType p8) = 0;
        virtual TC_FunctorImp * clone() const = 0;

        virtual ~TC_FunctorImp() {
        };
    };

    template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
    struct TC_FunctorImp<R, tidp::TYPELIST_9(P1, P2, P3, P4, P5, P6, P7, P8, P9)> {
        virtual R operator()(typename tidp::TypeTraits<P1>::ReferenceType p1,
                typename tidp::TypeTraits<P2>::ReferenceType p2,
                typename tidp::TypeTraits<P3>::ReferenceType p3,
                typename tidp::TypeTraits<P4>::ReferenceType p4,
                typename tidp::TypeTraits<P5>::ReferenceType p5,
                typename tidp::TypeTraits<P6>::ReferenceType p6,
                typename tidp::TypeTraits<P7>::ReferenceType p7,
                typename tidp::TypeTraits<P8>::ReferenceType p8,
                typename tidp::TypeTraits<P9>::ReferenceType p9) = 0;
        virtual TC_FunctorImp * clone() const = 0;

        virtual ~TC_FunctorImp() {
        };
    };

    template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10>
    struct TC_FunctorImp<R, tidp::TYPELIST_10(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)> {
        virtual R operator()(typename tidp::TypeTraits<P1>::ReferenceType p1,
                typename tidp::TypeTraits<P2>::ReferenceType p2,
                typename tidp::TypeTraits<P3>::ReferenceType p3,
                typename tidp::TypeTraits<P4>::ReferenceType p4,
                typename tidp::TypeTraits<P5>::ReferenceType p5,
                typename tidp::TypeTraits<P6>::ReferenceType p6,
                typename tidp::TypeTraits<P7>::ReferenceType p7,
                typename tidp::TypeTraits<P8>::ReferenceType p8,
                typename tidp::TypeTraits<P9>::ReferenceType p9,
                typename tidp::TypeTraits<P10>::ReferenceType p10) = 0;
        virtual TC_FunctorImp * clone() const = 0;

        virtual ~TC_FunctorImp() {
        };
    };

    template<class ParentFunctor, typename Fun>
    class TC_FunctorHandler : public TC_FunctorImp<typename ParentFunctor::ResultType, typename ParentFunctor::ParamList> {
    public:
        typedef typename ParentFunctor::ResultType ResultType;

        TC_FunctorHandler(Fun fun) : _fun(fun) {

        }

        TC_FunctorHandler *clone() const {
            return new TC_FunctorHandler(*this);
        }

        ResultType operator()() {
            return _fun();
        }

        ResultType operator()(typename ParentFunctor::Reference1 p1) {
            return _fun(p1);
        }

        ResultType operator()(typename ParentFunctor::Reference1 p1,
                typename ParentFunctor::Reference2 p2) {
            return _fun(p1, p2);
        }

        ResultType operator()(typename ParentFunctor::Reference1 p1,
                typename ParentFunctor::Reference2 p2,
                typename ParentFunctor::Reference3 p3) {
            return _fun(p1, p2, p3);
        }

        ResultType operator()(typename ParentFunctor::Reference1 p1,
                typename ParentFunctor::Reference2 p2,
                typename ParentFunctor::Reference3 p3,
                typename ParentFunctor::Reference4 p4) {
            return _fun(p1, p2, p3, p4);
        }

        ResultType operator()(typename ParentFunctor::Reference1 p1,
                typename ParentFunctor::Reference2 p2,
                typename ParentFunctor::Reference3 p3,
                typename ParentFunctor::Reference4 p4,
                typename ParentFunctor::Reference5 p5) {
            return _fun(p1, p2, p3, p4, p5);
        }

        ResultType operator()(typename ParentFunctor::Reference1 p1,
                typename ParentFunctor::Reference2 p2,
                typename ParentFunctor::Reference3 p3,
                typename ParentFunctor::Reference4 p4,
                typename ParentFunctor::Reference5 p5,
                typename ParentFunctor::Reference6 p6) {
            return _fun(p1, p2, p3, p4, p5, p6);
        }

        ResultType operator()(typename ParentFunctor::Reference1 p1,
                typename ParentFunctor::Reference2 p2,
                typename ParentFunctor::Reference3 p3,
                typename ParentFunctor::Reference4 p4,
                typename ParentFunctor::Reference5 p5,
                typename ParentFunctor::Reference6 p6,
                typename ParentFunctor::Reference7 p7) {
            return _fun(p1, p2, p3, p4, p5, p6, p7);
        }

        ResultType operator()(typename ParentFunctor::Reference1 p1,
                typename ParentFunctor::Reference2 p2,
                typename ParentFunctor::Reference3 p3,
                typename ParentFunctor::Reference4 p4,
                typename ParentFunctor::Reference5 p5,
                typename ParentFunctor::Reference6 p6,
                typename ParentFunctor::Reference7 p7,
                typename ParentFunctor::Reference8 p8) {
            return _fun(p1, p2, p3, p4, p5, p6, p7, p8);
        }

        ResultType operator()(typename ParentFunctor::Reference1 p1,
                typename ParentFunctor::Reference2 p2,
                typename ParentFunctor::Reference3 p3,
                typename ParentFunctor::Reference4 p4,
                typename ParentFunctor::Reference5 p5,
                typename ParentFunctor::Reference6 p6,
                typename ParentFunctor::Reference7 p7,
                typename ParentFunctor::Reference8 p8,
                typename ParentFunctor::Reference9 p9) {
            return _fun(p1, p2, p3, p4, p5, p6, p7, p8, p9);
        }

        ResultType operator()(typename ParentFunctor::Reference1 p1,
                typename ParentFunctor::Reference2 p2,
                typename ParentFunctor::Reference3 p3,
                typename ParentFunctor::Reference4 p4,
                typename ParentFunctor::Reference5 p5,
                typename ParentFunctor::Reference6 p6,
                typename ParentFunctor::Reference7 p7,
                typename ParentFunctor::Reference8 p8,
                typename ParentFunctor::Reference9 p9,
                typename ParentFunctor::Reference10 p10) {
            return _fun(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
        }

    private:
        Fun _fun;
    };
}
#endif	/* TL_FUNCTOR_H */

