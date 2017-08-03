#include "SignalHandle.h"

static SignalHandle* handle; /* dirty�Ĵ���Ҫ��װ���� */

static void sighandler(int);

static void addsig(int sig) {
	struct sigaction sa;
	bzero(&sa, sizeof(sa));
	sa.sa_handler = sighandler;
	sa.sa_flags |= SA_RESTART;
	utility::Sigfillset(&sa.sa_mask);
	sigaction(sig, &sa, NULL);
}


static void sighandler(int signo) { /* Ϊ�˷�ֹ��Ⱦ����Ӧ��,��������Ϊ��static���� */
	boost::function<void()> func = handle->GetHandler(signo);
	if (func)
		func();
}

void SignalHandle::BlockSigno(int signo) { /* ������ĳ���ź� */
	sigset_t signal_mask;
	utility::Sigemptyset(&signal_mask);
	utility::Sigaddset(&signal_mask, signo);
	utility::Sigprocmask(SIG_BLOCK, &signal_mask, NULL);
}

void SignalHandle::addSigHandle(int signo, handler&& func) {
	router_[signo] = std::move(func);
	//utility::Signal(SIGURG, sighandler); /* ע���źŴ����� */
	addsig(signo);
}

SignalHandle::SignalHandle() { /* ��ʼ������ */
	handle = this; /* ��¼��ָ���ֵ */
}
