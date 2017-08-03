#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <algorithm>
#include <vector>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>
#include <errno.h>


class Buffer
{
public:
	static const size_t kCheapPrepend = 8;
	static const size_t kInitialSize = 1024;

	explicit Buffer(size_t initialSize = kInitialSize)
		: buffer_(kCheapPrepend + initialSize)
		, readerIndex_(kCheapPrepend)
		, writerIndex_(kCheapPrepend)
	{

	}
	// ���޸���ĳ�Ա������ֵ�ĺ�����Ҫ��const����,����һ�ֺõ�ϰ��
	size_t readableBytes() const { /* �ɶ����ֽ���Ŀ */
		return writerIndex_ - readerIndex_;
	}

	size_t writableBytes() const { /* ��д���ֽ���Ŀ */
		return buffer_.size() - writerIndex_;
	}

	size_t prependableBytes() const {
		return readerIndex_;
	}

	const char* peek() const { /* ͵�� */
		return begin() + readerIndex_; // �����￪ʼ��
	}

	const char* findEOL() const {
		const void* eol = memchr(peek(), '\n', readableBytes());
		return static_cast<const char*>(eol);
	}

	const char* findCRLF() const {
		const char* crlf = std::search(peek(), beginWrite(), kCRLF, kCRLF + 2);
		return crlf == beginWrite() ? NULL : crlf;
	}
	bool getLine(char *dest, size_t len); /* ��ȡһ������ */

	const char* findEOF() const {
		const void* eol = memchr(peek(), '\n', readableBytes());
		return static_cast<const char*>(eol);
	}

	const char* findEOL(const char* start) const {
		assert(peek() <= start);
		assert(start <= beginWrite());
		const void* eol = memchr(start, '\n', beginWrite() - start);
		return static_cast<const char*>(eol);
	}

	void retrieve(size_t len) {
		//mylog("len = %d, reableBytes() = %ld", len, readableBytes());
		assert(len <= readableBytes());
		if (len < readableBytes()) {
			readerIndex_ += len;
		}
		else {
			retrieveAll();
		}
	}

	void retrieveAll() {
		readerIndex_ = kCheapPrepend;
		writerIndex_ = kCheapPrepend;
	}

	void retrieveUntil(const char* end) {
		assert(peek() <= end);
		assert(end <= beginWrite());
		retrieve(end - peek());
	}

	void ensureWritableBytes(size_t len) { /* ��֤���㹻��д��ռ� */
		if (writableBytes() < len) {
			makeSpace(len);
		}
		assert(writableBytes() >= len);
	}

	 void  append(const char* data, size_t len) {
		ensureWritableBytes(len);
		std::copy(data, data + len, beginWrite());
		hasWritten(len);
	}

	void appendStr(const char* format, ...); /* ��ʽ������ */

	char* beginWrite() { /* д�����ʼ�ط� */
		return begin() + writerIndex_;
	}

	const char* beginWrite() const { /* const�汾 */
		return begin() + writerIndex_;
	}

	void hasWritten(size_t len) {
		assert(len <= writableBytes());
		writerIndex_ += len;
	}

	void unwrite(size_t len) { /* ���� */
		assert(len <= readableBytes());
		writerIndex_ -= len;
	}

	void prepend(const void* data, size_t  len) {
		assert(len <= prependableBytes());
		readerIndex_ -= len;
		const char *d = static_cast<const char*>(data);
		std::copy(d, d + len, begin() + readerIndex_);
	}

	size_t internalCapacity() const { /* ������С */
		return buffer_.capacity();
	}

	ssize_t readFd(int fd, int* savedErrno);
private:
	char* begin() {
		return &*buffer_.begin();
	}
	const char* begin() const {
		return &*buffer_.begin();
	}

	void makeSpace(size_t len) {
		if (writableBytes() + prependableBytes() < len + kCheapPrepend) {
			buffer_.resize(writerIndex_ + len); /* ���·���洢�ռ� */
		}
		else { /* ���ʣ��Ŀռ��С�㹻��! */
			assert(kCheapPrepend < readerIndex_);
			size_t readable = readableBytes(); /* �ɶ����ֽ���Ŀ */
			std::copy(begin() + readerIndex_,
				begin() + writerIndex_,
				begin() + kCheapPrepend);
			readerIndex_ = kCheapPrepend;
			writerIndex_ = readerIndex_ + readable;
			assert(readable == readableBytes());
		}
	}
private:
	std::vector<char> buffer_;
	size_t readerIndex_;
	size_t writerIndex_;

	static const char kCRLF[];
};

#endif
