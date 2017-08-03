#ifndef _SIGNAL_HANDLE_H_
#define _SIGNAL_HANDLE_H_

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <signal.h>
#include <map>
#include <utility>
#include "csapp.h"

/*
 * ԭ����c�����źŴ��������ڼ�ª,��ֱ������,���������������,��Ҫ��һ�¼򵥵ķ�װ.
 */

class SignalHandle : boost::noncopyable
{
public:
	using handler = boost::function<void()>; /* ������ */
	using Router = std::map<int, handler>; /* ·�� */

	SignalHandle();

	void addSigHandle(int signo, handler&& func);
	void BlockSigno(int signo); /* ����ĳ���ź� */

	handler& GetHandler(int signo) {
		return router_[signo];
	}
private:
	Router router_;
};

#endif
