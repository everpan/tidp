/*
 * TL_StringBuffer.h
 *
 *  Created on: Jan 15, 2014
 *      Author: ever
 */

#ifndef TL_STRINGBUFFER_H_
#define TL_STRINGBUFFER_H_
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <functional>
using namespace std;
namespace tidp {

/*
 *
 * */
class TL_Chunk {
private:
	char * _addr;
	size_t _totalSize;
	size_t _lineSize;
	int _avalidNum;
	int _nextAvalidNum;
	//Invalid
public:
	TL_Chunk(int totalSize, int lineSize);
	size_t size();
	size_t chunkSize();
	char * get();
	void release(char * addr);
};
class TL_AChunk{
	char * _addr;
	int len;
	int remain;
};
class TL_StringBuffer {
public:
	typedef void *(*get_t)();
	TL_StringBuffer();
	~TL_StringBuffer();
	void appendText(char c);
	void appendText(unsigned char uc);
	void appendText(short s);
	void appendText(int i);
	void appendText(long l);
	void appendText(float f);
	void appendText(double d);

	void appendText(const char* cstr);
	void appendData(const char* buf,int len);
	void appendText(const string& str);
	void appendText(const TL_StringBuffer& strbuf);
	int write(int fd);
	size_t size();
private:
	char * _tmp;
	vector<TL_AChunk> _memlink;
	int _currentId;
};

} /* namespace tidp */

#endif /* TL_STRINGBUFFER_H_ */
