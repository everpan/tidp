/*
 * TL_MD5.h
 *
 *  Created on: Aug 18, 2014
 *      Author: ever
 */

#ifndef TL_MD5_H
#define TL_MD5_H
#include <stdio.h>
#include <string.h>
#include <string>

#include <iostream>

#include "TL_Exp.h"
namespace tidp {

#ifndef GET_ULONG_LE
#define GET_ULONG_LE(n,b,i)                             \
{                                                       \
    (n) = ( (unsigned long) (b)[(i)    ]       )        \
        | ( (unsigned long) (b)[(i) + 1] <<  8 )        \
        | ( (unsigned long) (b)[(i) + 2] << 16 )        \
        | ( (unsigned long) (b)[(i) + 3] << 24 );       \
}
#endif

#ifndef PUT_ULONG_LE
#define PUT_ULONG_LE(n,b,i)                             \
{                                                       \
    (b)[(i)    ] = (unsigned char) ( (n)       );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 3] = (unsigned char) ( (n) >> 24 );       \
}
#endif

class TL_MD5 {
public:
	TL_MD5();
	~TL_MD5();
	typedef unsigned char *POINTER;
	typedef unsigned short int UINT2;
	//typedef unsigned long int UINT4;
	typedef unsigned int UINT4;

	typedef struct {
		/**
		 * state (ABCD)
		 */
		//unsigned long state[4];
		UINT4 state[4];

		/**
		 * number of bits, modulo 2^64 (lsb first)
		 */
		//unsigned long count[2];
		UINT4 count[2];

		/**
		 * input buffer
		 */
		unsigned char buffer[64];
	} MD5_CTX;

	static void md5init(MD5_CTX * context);
	static void md5update(MD5_CTX * ctx, const char * input, size_t inputLen);
	static void md5final(unsigned char digest[16], MD5_CTX * ctx);
	static void md5process(MD5_CTX *ctx, const unsigned char data[64]);

	static void md5file(const char* filename, std::string& out);
	static void md5file(const std::string& filename, std::string& out);

	static void md5str(const char * str, int len, std::string& out);
	static void md5str(const std::string& str, std::string& out);

	static void bin2str(const void *buf, size_t len, char * out);
	static void bin2str(const void *buf, size_t len, std::string& out);

	void md5str2(const char * buf, int len, std::string& out);
	void md5str2(const std::string& str, std::string& out);

private:
	static const char b2smapU[16]; //upper
	static const char b2smapL[16]; //lower
	static unsigned char PADDING[64];

	MD5_CTX _ctx;
	char _bin_ret[16];
};
}
#endif /* TL_MD5_H */
