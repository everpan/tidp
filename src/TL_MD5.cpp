/*
 * TL_MD5.cpp
 *
 *  Created on: Sep 4, 2014
 *      Author: ever
 */

#include <TL_MD5.h>

namespace tidp {

unsigned char TL_MD5::PADDING[64] =
		{ 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
const char TL_MD5::b2smapU[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
const char TL_MD5::b2smapL[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
TL_MD5::TL_MD5() {
	// TODO Auto-generated constructor stub

}

TL_MD5::~TL_MD5() {
	// TODO Auto-generated destructor stub
}

void TL_MD5::md5init(MD5_CTX * context) {
	context->count[0] = context->count[1] = 0;
	context->state[0] = 0x67452301; /* load magic initialization constants.*/
	context->state[1] = 0xefcdab89;
	context->state[2] = 0x98badcfe;
	context->state[3] = 0x10325476;
}
void TL_MD5::md5update(MD5_CTX * ctx, const char * input, size_t inputLen) {
	unsigned int fill;
	unsigned long left;
	unsigned char * ucinput = (unsigned char *) input;
	if (inputLen <= 0)
		return;
	left = ctx->count[0] & 0x3F;
	fill = 64 - left;
	ctx->count[0] += inputLen;
	ctx->count[0] &= 0xFFFFFFFF;
	if (ctx->count[0] < (unsigned long) inputLen)
		ctx->count[1]++;
	if (left && inputLen >= fill) {
		memcpy((void *) (ctx->buffer + left), (void *) ucinput, fill);
		md5process(ctx, ctx->buffer);
		ucinput += fill;
		inputLen -= fill;
		left = 0;
	}
	while (inputLen >= 64) {
		md5process(ctx, (const unsigned char*) ucinput);
		ucinput += 64;
		inputLen -= 64;
	}
	if (inputLen > 0) {
		memcpy((void *) (ctx->buffer + left), (void *) ucinput, inputLen);
	}

}
void TL_MD5::md5final(unsigned char digest[16], MD5_CTX * ctx) {
	unsigned long last, padn;
	unsigned long high, low;
	char msglen[8];

	high = (ctx->count[0] >> 29) | (ctx->count[1] << 3);
	low = (ctx->count[0] << 3);

	PUT_ULONG_LE(low, msglen, 0);
	PUT_ULONG_LE(high, msglen, 4);

	last = ctx->count[0] & 0x3F;
	padn = (last < 56) ? (56 - last) : (120 - last);

	md5update(ctx, (const char *) PADDING, padn);
	md5update(ctx, msglen, 8);

	PUT_ULONG_LE(ctx->state[0], digest, 0);
	PUT_ULONG_LE(ctx->state[1], digest, 4);
	PUT_ULONG_LE(ctx->state[2], digest, 8);
	PUT_ULONG_LE(ctx->state[3], digest, 12);
}

void TL_MD5::md5process(MD5_CTX *ctx, const unsigned char data[64]) {

	unsigned long X[16], A, B, C, D;

	GET_ULONG_LE(X[0], data, 0);
	GET_ULONG_LE(X[1], data, 4);
	GET_ULONG_LE(X[2], data, 8);
	GET_ULONG_LE(X[3], data, 12);
	GET_ULONG_LE(X[4], data, 16);
	GET_ULONG_LE(X[5], data, 20);
	GET_ULONG_LE(X[6], data, 24);
	GET_ULONG_LE(X[7], data, 28);
	GET_ULONG_LE(X[8], data, 32);
	GET_ULONG_LE(X[9], data, 36);
	GET_ULONG_LE(X[10], data, 40);
	GET_ULONG_LE(X[11], data, 44);
	GET_ULONG_LE(X[12], data, 48);
	GET_ULONG_LE(X[13], data, 52);
	GET_ULONG_LE(X[14], data, 56);
	GET_ULONG_LE(X[15], data, 60);

#define S(x,n) ((x << n) | ((x & 0xFFFFFFFF) >> (32 - n)))

#define P(a,b,c,d,k,s,t)                                \
	    {                                                       \
	        a += F(b,c,d) + X[k] + t; a = S(a,s) + b;           \
	    }

	A = ctx->state[0];
	B = ctx->state[1];
	C = ctx->state[2];
	D = ctx->state[3];

#define F(x,y,z) (z ^ (x & (y ^ z)))

	P(A, B, C, D, 0, 7, 0xD76AA478);
	P(D, A, B, C, 1, 12, 0xE8C7B756);
	P(C, D, A, B, 2, 17, 0x242070DB);
	P(B, C, D, A, 3, 22, 0xC1BDCEEE);
	P(A, B, C, D, 4, 7, 0xF57C0FAF);
	P(D, A, B, C, 5, 12, 0x4787C62A);
	P(C, D, A, B, 6, 17, 0xA8304613);
	P(B, C, D, A, 7, 22, 0xFD469501);
	P(A, B, C, D, 8, 7, 0x698098D8);
	P(D, A, B, C, 9, 12, 0x8B44F7AF);
	P(C, D, A, B, 10, 17, 0xFFFF5BB1);
	P(B, C, D, A, 11, 22, 0x895CD7BE);
	P(A, B, C, D, 12, 7, 0x6B901122);
	P(D, A, B, C, 13, 12, 0xFD987193);
	P(C, D, A, B, 14, 17, 0xA679438E);
	P(B, C, D, A, 15, 22, 0x49B40821);

#undef F

#define F(x,y,z) (y ^ (z & (x ^ y)))

	P(A, B, C, D, 1, 5, 0xF61E2562);
	P(D, A, B, C, 6, 9, 0xC040B340);
	P(C, D, A, B, 11, 14, 0x265E5A51);
	P(B, C, D, A, 0, 20, 0xE9B6C7AA);
	P(A, B, C, D, 5, 5, 0xD62F105D);
	P(D, A, B, C, 10, 9, 0x02441453);
	P(C, D, A, B, 15, 14, 0xD8A1E681);
	P(B, C, D, A, 4, 20, 0xE7D3FBC8);
	P(A, B, C, D, 9, 5, 0x21E1CDE6);
	P(D, A, B, C, 14, 9, 0xC33707D6);
	P(C, D, A, B, 3, 14, 0xF4D50D87);
	P(B, C, D, A, 8, 20, 0x455A14ED);
	P(A, B, C, D, 13, 5, 0xA9E3E905);
	P(D, A, B, C, 2, 9, 0xFCEFA3F8);
	P(C, D, A, B, 7, 14, 0x676F02D9);
	P(B, C, D, A, 12, 20, 0x8D2A4C8A);

#undef F

#define F(x,y,z) (x ^ y ^ z)

	P(A, B, C, D, 5, 4, 0xFFFA3942);
	P(D, A, B, C, 8, 11, 0x8771F681);
	P(C, D, A, B, 11, 16, 0x6D9D6122);
	P(B, C, D, A, 14, 23, 0xFDE5380C);
	P(A, B, C, D, 1, 4, 0xA4BEEA44);
	P(D, A, B, C, 4, 11, 0x4BDECFA9);
	P(C, D, A, B, 7, 16, 0xF6BB4B60);
	P(B, C, D, A, 10, 23, 0xBEBFBC70);
	P(A, B, C, D, 13, 4, 0x289B7EC6);
	P(D, A, B, C, 0, 11, 0xEAA127FA);
	P(C, D, A, B, 3, 16, 0xD4EF3085);
	P(B, C, D, A, 6, 23, 0x04881D05);
	P(A, B, C, D, 9, 4, 0xD9D4D039);
	P(D, A, B, C, 12, 11, 0xE6DB99E5);
	P(C, D, A, B, 15, 16, 0x1FA27CF8);
	P(B, C, D, A, 2, 23, 0xC4AC5665);

#undef F

#define F(x,y,z) (y ^ (x | ~z))

	P(A, B, C, D, 0, 6, 0xF4292244);
	P(D, A, B, C, 7, 10, 0x432AFF97);
	P(C, D, A, B, 14, 15, 0xAB9423A7);
	P(B, C, D, A, 5, 21, 0xFC93A039);
	P(A, B, C, D, 12, 6, 0x655B59C3);
	P(D, A, B, C, 3, 10, 0x8F0CCC92);
	P(C, D, A, B, 10, 15, 0xFFEFF47D);
	P(B, C, D, A, 1, 21, 0x85845DD1);
	P(A, B, C, D, 8, 6, 0x6FA87E4F);
	P(D, A, B, C, 15, 10, 0xFE2CE6E0);
	P(C, D, A, B, 6, 15, 0xA3014314);
	P(B, C, D, A, 13, 21, 0x4E0811A1);
	P(A, B, C, D, 4, 6, 0xF7537E82);
	P(D, A, B, C, 11, 10, 0xBD3AF235);
	P(C, D, A, B, 2, 15, 0x2AD7D2BB);
	P(B, C, D, A, 9, 21, 0xEB86D391);

#undef F

	ctx->state[0] += A;
	ctx->state[1] += B;
	ctx->state[2] += C;
	ctx->state[3] += D;
}

void TL_MD5::md5file(const char* filename, std::string& out) {
	FILE * fp = fopen(filename, "r");
	if (fp == NULL) {
		throw TL_Exception(std::string("can not open ").append(filename), -1);
	}
	char buffer[256]; // = new char[1024];
	MD5_CTX ctx;
	md5init(&ctx);
	int ret = -1;
	do {
		ret = fread(buffer, 1, 1024, fp);
		if (ret < 0) {
			throw TL_Exception("fread file fail.", ferror(fp));
		}
		md5update(&ctx, buffer, ret);
	} while (ret > 0);
	md5final((unsigned char*) buffer, &ctx);
	bin2str(buffer, 16, out);
	//delete[] buffer;
}
void TL_MD5::md5file(const std::string& filename, std::string& out) {
	md5file(filename.c_str(), out);
}

void TL_MD5::md5str(const char * str, int len, std::string& out) {
	MD5_CTX ctx;
	md5init(&ctx);
	md5update(&ctx, str, len);
	char buffer[16];
	md5final((unsigned char*) buffer, &ctx);
	bin2str(buffer, 16, out);
}

void TL_MD5::md5str(const std::string& str, std::string& out) {
	md5str(str.c_str(), str.size(), out);
}

void TL_MD5::md5str2(const char * buf, int len, std::string& out) {
	md5init(&_ctx);
	md5update(&_ctx, buf, len);
	md5final((unsigned char*) _bin_ret, &_ctx);
	bin2str(_bin_ret, 16, out);
}

void TL_MD5::md5str2(const std::string& str, std::string& out) {
	md5str(str.c_str(), str.size(), out);
}

void TL_MD5::bin2str(const void *buf, size_t len, std::string& out) {
	out.clear();

	if (buf == NULL || len <= 0) {
		return;
	}
	out.reserve(len * 2);
	const char * p = (const char *) buf;
	for (size_t i = 0; i < len; ++i, ++p) {
		out.append(1, TL_MD5::b2smapL[(*p & 0xF0) >> 4]);
		out.append(1, TL_MD5::b2smapL[*p & 0x0F]);
	}
}

void TL_MD5::bin2str(const void *buf, size_t len, char * out) {
	if (buf == NULL || len <= 0) {
		return;
	}
	const char * p = (const char *) buf;
	int len2 = (len << 1);
	//std::cout << "bin2str:" << len2 << std::endl;
	for (int i = 0; i < len2; i+=2, ++p) {
		out[i] = TL_MD5::b2smapL[(*p & 0xF0) >> 4];
		out[i+1] = TL_MD5::b2smapL[*p & 0x0F];
	}
}
} /* namespace tidp */
