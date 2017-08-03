#ifndef _SPEED_BARRIER_H_
#define _SPEED_BARRIER_H_
#include <boost/noncopyable.hpp>
#include "TimeStamp.h"

/* ���ػ����ϴ��ٶ������� */
class SpeedBarrier : boost::noncopyable
{
public:
	void StartTimer()
	{
		start_ = Timestamp::now();
	}
	void limitSpeed(int64_t maxSpeed, size_t bytesTransed);
	static int64_t maxDownloadSpeed; /* ���������ٶ� */
	static int64_t maxUploadSpeed; /* �����ϴ��ٶ� */
private:
	void nanoSleep(int64_t microSeconds);
private:
	Timestamp start_; /* ��ʼ��ʱ�� */
};
#endif
