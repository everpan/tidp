/* 
 * File:   TL_TypeList.h
 * Author: everpan
 *
 * Created on 2012年3月30日, 下午9:15
 */

#ifndef TL_TypeList_H
#define	TL_TypeList_H
namespace tidp {
    class TL_NullType;

    class TL_EmptyType {
    };

    template<class T>
    struct TL_Type2Type {
        typedef T OrigType;
    };

    template<int v>
    struct TL_Int2Type {

        enum {
            value = v
        };
    };

    template<class Head, class Tail>
    struct TL_TypeList {
        typedef Head H;
        typedef Tail T;
    };

#define TL_TypeList_1(T1) TL_TypeList<T1,tidp::TL_NullType>
#define TL_TypeList_2(T1, T2) TL_TypeList<T1, tidp::TL_TypeList_1(T2)>
#define TL_TypeList_3(T1, T2, T3) TL_TypeList<T1, tidp::TL_TypeList_2(T2, T3)>
#define TL_TypeList_4(T1, T2, T3, T4) TL_TypeList<T1, tidp::TL_TypeList_3(T2, T3, T4)>
#define TL_TypeList_5(T1, T2, T3, T4, T5) TL_TypeList<T1, tidp::TL_TypeList_4(T2, T3, T4, T5)>
#define TL_TypeList_6(T1, T2, T3, T4, T5, T6) TL_TypeList<T1, tidp::TL_TypeList_5(T2, T3, T4, T5, T6)>
#define TL_TypeList_7(T1, T2, T3, T4, T5, T6, T7) TL_TypeList<T1, tidp::TL_TypeList_6(T2, T3, T4, T5, T6, T7)>
#define TL_TypeList_8(T1, T2, T3, T4, T5, T6, T7, T8) TL_TypeList<T1, tidp::TL_TypeList_7(T2, T3, T4, T5, T6, T7, T8)>
#define TL_TypeList_9(T1, T2, T3, T4, T5, T6, T7, T8, T9) TL_TypeList<T1, tidp::TL_TypeList_8(T2, T3, T4, T5, T6, T7, T8, T9)>
#define TL_TypeList_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) TL_TypeList<T1, tidp::TL_TypeList_9(T2, T3, T4, T5, T6, T7, T8, T9, T10)>
#define TL_TypeList_11(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) TL_TypeList<T1, tidp::TL_TypeList_10(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)>
#define TL_TypeList_12(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) TL_TypeList<T1, tidp::TL_TypeList_11(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)>
#define TL_TypeList_13(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13) TL_TypeList<T1, tidp::TL_TypeList_12(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)>
#define TL_TypeList_14(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14) TL_TypeList<T1, tidp::TL_TypeList_13(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)>
#define TL_TypeList_15(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) TL_TypeList<T1, tidp::TL_TypeList_14(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)>
#define TL_TypeList_16(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16) TL_TypeList<T1, tidp::TL_TypeList_15(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16)>
#define TL_TypeList_17(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17) TL_TypeList<T1, tidp::TL_TypeList_16(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17)>
#define TL_TypeList_18(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18) TL_TypeList<T1, tidp::TL_TypeList_17(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18)>
#define TL_TypeList_19(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19) TL_TypeList<T1, tidp::TL_TypeList_18(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19)>
#define TL_TypeList_20(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20) TL_TypeList<T1, tidp::TL_TypeList_19(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20)> 

    template<class TList> struct Length;
    template<>
    struct Length<TL_NullType>
    {
        enum { value = 0 };
    };
    template<class Head, class Tail>
    struct Length<TL_TypeList<Head, Tail> >
    {
        enum { value = 1 + Length<Tail>::value };
    };
    
    
    template<class TList, unsigned int i> struct TypeAt;
    template<class Head, class Tail> struct TypeAt<TL_TypeList<Head, Tail>, 0>
    {
        typedef Head Result;
    };
    template<class Head, class Tail, unsigned int i> struct TypeAt<TL_TypeList<Head, Tail>, i>
    {
        typedef typename TypeAt<Tail, i-1>::Result Result;
    };

    template<class TList, unsigned int index, typename DefaultType = TL_NullType> struct TypeAtNonStrict
    {
        typedef DefaultType Result;
    };
    template <class Head, class Tail, typename DefaultType> struct TypeAtNonStrict<TL_TypeList<Head, Tail>, 0, DefaultType>
    {
        typedef Head Result;
    };
    template <class Head, class Tail, unsigned int i, typename DefaultType> struct TypeAtNonStrict<TL_TypeList<Head, Tail>, i, DefaultType>
    {
        typedef typename TypeAtNonStrict<Tail, i - 1, DefaultType>::Result Result;
    };
    
    /**
     * 取链表上类型为T的序号, 没有则返回-1
     */
    template<class TList, class T> struct IndexOf;
    template<class T> struct IndexOf<TL_NullType, T>
    {
        enum { value = -1 };
    };
    template<class Tail, class T> struct IndexOf<TL_TypeList<T,Tail>, T>
    {
        enum { value = 0 };
    };
    template<class Head, class Tail, class T> struct IndexOf<TL_TypeList<Head, Tail>, T>
    {
    private:
        enum { temp = IndexOf<Tail, T>::value };
    public:
        enum { value = temp == -1 ? -1 : 1 + temp };
    };

    /**
     * Append, 添加到链表尾部
     */
    template<class TList, class T> struct Append;
    template<> struct Append<TL_NullType, TL_NullType>
    {
        typedef TL_NullType Result;
    };
    template<class T> struct Append<TL_NullType,T>
    {
        typedef TL_TypeList_1(T) Result;
    };
    template<class Head, class Tail> struct Append<TL_NullType, TL_TypeList<Head, Tail> >
    {
        typedef TL_TypeList<Head, Tail> Result;
    };
    template<class Head, class Tail, class T> struct Append<TL_TypeList<Head, Tail>, T>
    {
        typedef TL_TypeList<Head, typename Append<Tail, T>::Result> Result;
    };

    /**
     * Erase 删除
     */
    template<class TList, class T> struct Erase;
    template<class T> struct Erase<TL_NullType, T>
    {
        typedef TL_NullType Result;
    };
    template<class T, class Tail> struct Erase<TL_TypeList<T, Tail>, T>
    {
        typedef Tail Result;
    };
    template<class Head, class Tail, class T> struct Erase<TL_TypeList<Head, Tail>, T>
    {
        typedef TL_TypeList<Head, typename Erase<Tail, T>::Result> Result;
    };

    /**
     * EraseAll 删除
     */
    template<class TList, class T> struct EraseAll;
    template<class T> struct EraseAll<TL_NullType, T>
    {
        typedef TL_NullType Result;
    };
    template<class T, class Tail> struct EraseAll<TL_TypeList<T, Tail>, T>
    {
        typedef typename EraseAll<Tail, T>::Result Result;
    };
    template<class Head, class Tail, class T> struct EraseAll<TL_TypeList<Head, Tail>, T>
    {
        typedef TL_TypeList<Head, typename EraseAll<Tail, T>::Result> Result;
    };
    
    //TLMaker 生成TL_TypeList类型
    template<class T1=TL_NullType,  class T2=TL_NullType,  class T3=TL_NullType,  class T4=TL_NullType,  class T5=TL_NullType,
             class T6=TL_NullType,  class T7=TL_NullType,  class T8=TL_NullType,  class T9=TL_NullType,  class T10=TL_NullType,
             class T11=TL_NullType, class T12=TL_NullType, class T13=TL_NullType, class T14=TL_NullType, class T15=TL_NullType,
             class T16=TL_NullType, class T17=TL_NullType, class T18=TL_NullType, class T19=TL_NullType, class T20=TL_NullType>
    struct TLMaker
    {
    private:
        typedef TL_TypeList_20(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10,
                            T11, T12, T13, T14, T15, T16, T17, T18, T19, T20) TmpList;
    public:
        typedef typename EraseAll<TmpList, TL_NullType>::Result Result;
    };
    
    

    
    //boots
    //一元函数
    template <class Operation>
    struct unary_traits_imp;

    template <class Operation>
    struct unary_traits_imp<Operation*> {
        typedef Operation function_type;
        typedef const function_type & param_type;
        typedef typename Operation::result_type result_type;
        typedef typename Operation::argument_type argument_type;
    };

    template <class R, class A>
    struct unary_traits_imp<R(*)(A)> {
        typedef R(*function_type)(A);
        typedef R(*param_type)(A);
        typedef R result_type;
        typedef A argument_type;
    };

    //二元函数
    template <class Operation>
    struct binary_traits_imp;

    template <class Operation>
    struct binary_traits_imp<Operation*> {
        typedef Operation function_type;
        typedef const function_type & param_type;
        typedef typename Operation::result_type result_type;
        typedef typename Operation::first_argument_type first_argument_type;
        typedef typename Operation::second_argument_type second_argument_type;
    };

    template <class R, class A1, class A2>
    struct binary_traits_imp<R(*)(A1, A2)> {
        typedef R(*function_type)(A1, A2);
        typedef R(*param_type)(A1, A2);
        typedef R result_type;
        typedef A1 first_argument_type;
        typedef A2 second_argument_type;
    };




#endif	/* TL_TypeList_H */

