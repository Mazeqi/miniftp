#include "SpeedBarrier.h"
#include <time.h>
#include <errno.h>

int64_t SpeedBarrier::maxDownloadSpeed = 1024 * 1024;
int64_t SpeedBarrier::maxUploadSpeed = 1024 * 1024;

void SpeedBarrier::limitSpeed(int64_t maxSpeed, size_t bytesTransed) {
	/* ��ʼ�����ٶ� */
	Timestamp now = Timestamp::now();
	int64_t timePassed = now.microSecondsSinceEpoch() - start_.microSecondsSinceEpoch();
	int64_t speedNow = bytesTransed / (static_cast<double>(timePassed) / Timestamp::kMicroSecondsPerSecond); /* ���ÿ���Ӵ��͵��ֽ��� */
	if (speedNow > maxSpeed) {
		/* Ȼ�����Ǿͱ������� */
		int64_t diff = speedNow - maxSpeed; /* �������֮��Ĳ�ֵ */
		int64_t sleepMicroSeconds = (diff / static_cast<double>(maxSpeed)) * timePassed;
		nanoSleep(sleepMicroSeconds);
		//usleep(sleepMicroSeconds); // usleepҪ�����С��1000000,Ҳ����1��,���ǲ���ʵ��.
	}
	start_ = Timestamp::now(); /* ���¼�ʱ */
}

void SpeedBarrier::nanoSleep(int64_t microSeconds) {
	struct timespec timeSpan;
	timeSpan.tv_sec = microSeconds / Timestamp::kMicroSecondsPerSecond;
	// 1΢�����1000����
	timeSpan.tv_nsec = (microSeconds - timeSpan.tv_sec * Timestamp::kMicroSecondsPerSecond) * 1000; /* ���뼶��ľ��� */
	
	int res;
	do {
		res = nanosleep(&timeSpan, &timeSpan);
	} while (res == -1 && errno == EINTR); /* ���ܱ��ź��ж�,����Ҫ��do while��ʽ */
}