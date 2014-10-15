/* 
 * File:   TL_TypeTraits.h
 * Author: everpan
 *
 * Created on 2012年3月30日, 下午9:14
 */

#ifndef TL_TypeTraits_H
#define	TL_TypeTraits_H

#include "TL_TypeList.h"
#include "TL_Loki.h"

//类型提取
namespace tidp {

    template<typename T>
    class TL_TypeTraits {
    private:

        ///////////////////////////////////////////////////////
        //提取引用的原始类型(即去掉引用类型)

        template<class U>
        struct ReferenceTraits {

            enum {
                result = false
            };
            typedef U Result;
        };

        template<class U>
        struct ReferenceTraits<U&> {

            enum {
                result = true
            };
            typedef U Result;
        };

        ///////////////////////////////////////////////////////
        //指针类型

        template<class U>
        struct PointerTraits {

            enum {
                result = false
            };
            typedef TL_NullType Result;
        };

        template<class U>
        struct PointerTraits<U*> {

            enum {
                result = true
            };
            typedef U Result;
        };

        template<class U>
        struct PointerTraits<U*&> {

            enum {
                result = true
            };
            typedef U Result;
        };
        
        template<typename U>
        struct PointerToMemberTraits {

            enum {
                result = false
            };
        };

        template<class U, class V>
        struct PointerToMemberTraits<U V::*> {

            enum {
                result = true
            };
        };

        template<class U, class V>
        struct PointerToMemberTraits<U V::*&> {

            enum {
                result = true
            };
        };

        ///////////////////////////////////////////////////////
        // const

        template<typename U>
        struct UnConstTraits {

            enum {
                result = false
            };
            typedef U Result;
        };

        template<typename U>
        struct UnConstTraits<const U> {

            enum {
                result = true
            };
            typedef U Result;
        };

        template<typename U>
        struct UnConstTraits<const U&> {

            enum {
                result = true
            };
            typedef U& Result;
        };

        ///////////////////////////////////////////////////////
        // volatile

        template<typename U>
        struct UnVolatileTraits {

            enum {
                result = false
            };
            typedef U Result;
        };

        template<typename U>
        struct UnVolatileTraits<volatile U> {

            enum {
                result = true
            };
            typedef U Result;
        };

        template<typename U>
        struct UnVolatileTraits<volatile U&> {

            enum {
                result = true
            };
            typedef U& Result;
        };
    public:
        //T是否是指针类型

        enum {
            isPointer = PointerTraits<T>::result
        };
        //T是否是引用类型

        enum {
            isReference = ReferenceTraits<T>::result
        };
        //T是否指向成员函数的指针

        enum {
            isMemberPointer = PointerToMemberTraits<T>::result
        };

        //T是否是Const类型

        enum {
            isConst = UnConstTraits<T>::result
        };
        //T是否是Volatile类型

        enum {
            isVolatile = UnVolatileTraits<T>::result
        };

        //如果T是指针类型,则获取T的原类型, 即去掉指针类型
        typedef typename PointerTraits<T>::Result PointeeType;
        //如果T是引用类型,则获取T的原类型, 即去掉引用类型
        typedef typename ReferenceTraits<T>::Result ReferencedType;
        //如果T是Const类型,则获取T的原类型, 即去掉Const类型
        typedef typename UnConstTraits<T>::Result NonConstType;
        //如果T是volatile类型,则获取T的原类型, 即去掉volatile类型
        typedef typename UnVolatileTraits<T>::Result NonVolatileType;
        //去掉const volatile类型
        typedef typename UnVolatileTraits<typename UnConstTraits<T>::Result>::Result UnqualifiedType;

    public:

        //////////////////////////////////////////////////////
        //
        typedef tidp::TLMaker<unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long>::Result UnsignedInts;
        typedef tidp::TLMaker<signed char, short, int, long, long long>::Result SignedInts;
        typedef tidp::TLMaker<bool, char, wchar_t>::Result OtherInts;
        typedef tidp::TLMaker<float, double, long double>::Result Floats;
        typedef tidp::TL_TypeList_2(tidp::TL_EmptyType, tidp::TL_NullType) TL_NullTypes;

        //无符号整形

        enum {
            isStdUnsignedInt = tidp::IndexOf<UnsignedInts, T>::value >= 0
        };
        //有符号整形

        enum {
            isStdSignedInt = tidp::IndexOf<SignedInts, T>::value >= 0
        };
        //整形

        enum {
            isStdInt = isStdUnsignedInt || isStdSignedInt || tidp::IndexOf<OtherInts, T>::value >= 0
        };
        //浮点类型

        enum {
            isStdFloat = tidp::IndexOf<Floats, T>::value >= 0
        };
        //数值类型

        enum {
            isStdArith = isStdInt || isStdFloat
        };
        //基础类型(包括void)

        enum {
            isStdFundamental = isStdArith || tidp::IndexOf < tidp::TL_TypeList_1(void), T>::value >= 0
        };
        //空类型

        enum {
            isTL_NullType = tidp::IndexOf<TL_NullTypes, T>::value >= 0
        };
        //简单类型

        enum {
            isBaseType = isStdArith || isPointer || isMemberPointer
        };

        //对于复杂类型, 获取数据的引用类型, 即加上引用类型
        typedef typename TypeSelect<isBaseType, T, ReferencedType&>::Result ReferenceType;

        //对于复杂类型且非空类型, 获取数据的引用类型, 即加上引用类型
        //typedef typename TypeSelect<isBaseType || isTL_NullType, T, ReferencedType&>::Result ReferenceTypeEx;

        //获取数据的原类型, 消除引用的引用这种情况
        typedef typename TypeSelect<!isReference, T, ReferencedType&>::Result ParameterType;
    };
}
#endif	/* TL_TypeTraits_H */

