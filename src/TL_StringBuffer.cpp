/*
 * TL_StringBuffer.cpp
 *
 *  Created on: Jan 15, 2014
 *      Author: ever
 */

#include <TL_StringBuffer.h>

namespace tidp {

TL_Chunk::TL_Chunk(int totalSize, int lineSize) :
		_addr(0), _totalSize(totalSize), _lineSize(lineSize), _avalidNum(0), _nextAvalidNum(
				0) {
	if (_lineSize < _totalSize) {
		if (_lineSize < 4)
			_lineSize = 4;
		//int lineNum = 0;
		if (_totalSize % _lineSize == 0)
			_avalidNum = _totalSize / _lineSize;
		else {
			_avalidNum = _totalSize / _lineSize + 1;
			_totalSize = _avalidNum * _lineSize;
		}
		_addr = (char*) malloc(_totalSize);
		if (_addr == NULL) {
			//todo
		}
		for (int i = 0; i < _avalidNum; ++i) {
			*(int*) (_addr + i * _lineSize) = i + 1;
		}
	}
	//todo throw
}
size_t TL_Chunk::size() {
	return _totalSize;
}
size_t TL_Chunk::chunkSize() {
	return _lineSize;
}
char * TL_Chunk::get() {
	return _addr + _nextAvalidNum * _lineSize;
}
void TL_Chunk::release(char * addr) {
	if (_nextAvalidNum > 0 && ((addr - _addr) % _lineSize == 0)
			&& ((addr - _addr) / (int) _lineSize <= _avalidNum)) {
		*(int*) addr = _nextAvalidNum;
		_nextAvalidNum = (addr - _addr) / _lineSize;
	}
}

TL_StringBuffer::TL_StringBuffer() :
		_currentId(0) {
	_tmp = new char[64];
}
TL_StringBuffer::~TL_StringBuffer() {
	if (_tmp)
		delete _tmp;
}
void TL_StringBuffer::appendText(char c) {

}
void TL_StringBuffer::appendText(int i) {

}
void TL_StringBuffer::appendText(long l) {
}
void TL_StringBuffer::appendText(const string& str) {
}
void TL_StringBuffer::appendText(const char* cstr) {
}
void TL_StringBuffer::appendData(const char* buf, int len) {
}
void TL_StringBuffer::appendText(const TL_StringBuffer& strbuf) {
}
} /* namespace tidp */
