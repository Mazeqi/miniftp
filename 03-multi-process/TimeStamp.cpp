#include "TimeStamp.h"
#include <sys/time.h>
#include <stdio.h>
#include <inttypes.h>

Timestamp Timestamp::now() {
	struct timeval tv;
	gettimeofday(&tv, NULL); /* NULL��ʾ���ǲ���Ҫʱ������Ϣ */
	int64_t seconds = tv.tv_sec; /* �õ���Ӧ������ */
	return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec); /* Timestamp�ڲ�����΢������ʾ��! */
}