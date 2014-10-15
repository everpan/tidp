/*
 * File:   TL_Object.h
 * Author: everpan
 *
 * Created on 2011年3月9日, 下午9:36
 */

#ifndef TL_OBJECT_H
#define	TL_OBJECT_H

#include <string>
//LittleEndian
#define TL_LITTLE_ENDIAN 1


namespace tidp
{

    class TL_Object
    {
    public:

        virtual ~TL_Object()
        {
        };
        //序列化至buffer开始的内存块,并返回结束内存地址的下一个地址
        //这里buffer的大小由上层调用者负责检查,否则会引起堆内存错误.
        virtual char* serialization(char* buffer) = 0;
        virtual char* serialization(char* buffer) const = 0;
        //反序列化
        virtual const char* unserialization(const char* buffer) = 0;
        //返回string形式
        virtual std::string toString() = 0;
        //字符串形式,返回结束内存地址的下一个地址
        virtual char* toString(char* buffer) = 0;
        //hash算法
        virtual int hash() = 0;
        //返回存储需要的字节容量
        virtual size_t byteSize() = 0;
    private:
    };

    class TL_TYPE_T : public TL_Object
    {
    public:
        virtual char* serialization(char* buffer) = 0;
        virtual char* serialization(char* buffer) const = 0;
        virtual const char* unserialization(const char* buffer) = 0;
        virtual std::string toString() = 0;
        virtual char* toString(char* buffer) = 0;
        virtual char* toString(char* buffer) const = 0;
        virtual int hash() = 0;
        virtual size_t byteSize() = 0;
        virtual void parse(const char* buffer, size_t len) = 0;

        virtual ~TL_TYPE_T()
        {
        }
    };
}
#endif	/* TL_OBJECT_H */

