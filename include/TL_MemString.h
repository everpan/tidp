/*
 * File:   TL_MemString.h
 * Author: everpan
 *
 * Created on 2011年6月7日, 下午8:30
 */
//纯粹的内存cache 类似于string，无格式。
//无锁版，多线程下需要自己额外加锁
#ifndef TL_MemString_H
#define	TL_MemString_H
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include <iostream>
namespace tidp {
class TL_MemString;
typedef TL_MemString& (*TL_MemStringFUN)(TL_MemString& orig);

class TL_MemString {
public:

	enum {
		eStr = 1, eBit = 2
	};

	TL_MemString(size_t len) :
			_addr(NULL), _top(0), _bot(0), _len(0), _isattache(false) {
		//_split_char = 0;
		if (len > 0) {
			init(len);
		}
	}

	TL_MemString() :
			_addr(NULL), _top(0), _bot(0), _len(512), _isattache(false) {
		init(_len);
	}

	virtual ~TL_MemString();
	int init(size_t len);
	int resize(size_t len);
	//外界内存为cache
	void attache(char* addr, size_t len);
	bool isFull(const size_t &len);
	bool isEmpty() const;
	bool isDestory();

	void destory();
	void clear();
	//保留控件，free空间
	size_t reserve();
	//使用空间
	size_t size();
	size_t capacity();

	const char* c_str();
	void append(const TL_MemString& mc);
	void append(const std::string& s);

	bool push_backNT(const char* addr, size_t len); //no throw
	//void pop_frontNT(const char* addr,size_t& len);
	//写入到某个文件
	int write(int fd);

	TL_MemString& operator=(const std::string& s);

	TL_MemString & operator<<(char n);
	TL_MemString & operator<<(unsigned char n);

	TL_MemString & operator<<(short n);
	TL_MemString & operator<<(unsigned short n);

	TL_MemString & operator<<(int n);
	TL_MemString & operator<<(unsigned int n);

	TL_MemString & operator<<(long n);
	TL_MemString & operator<<(unsigned long n);

	TL_MemString & operator<<(long long n);
	TL_MemString & operator<<(unsigned long long n);

	TL_MemString & operator<<(double n);
	TL_MemString & operator<<(float n);
	//TL_MemString & operator<<(const char[] s);
	//TL_MemString & operator<<(TL_MemString& mstr,const char* s);
	TL_MemString & operator<<(const char* s);
	TL_MemString & operator<<(void * s);
	TL_MemString & operator<<(const std::string& s);

	TL_MemString& operator <<(TL_MemStringFUN fun);
protected:
	TL_MemString(const TL_MemString& orig);
	TL_MemString& operator =(const TL_MemString&);
private:
	char * _addr;
	size_t _top;
	size_t _bot;
	size_t _len;
	bool _isattache;
	int _mod;
	char _tmp[64];
	//char _split_char;
};

TL_MemString& endl(TL_MemString& orig);
}
#endif	/* TL_MemString_H */

