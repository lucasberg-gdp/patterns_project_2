#include "cWaitTimer.h"

void cWaitTimer::Init(double timer)
{
	m_TimeToWait = timer;
}

void cWaitTimer::Start(void)
{
	m_ElapsedTime = 0.0;
}

void cWaitTimer::Update(double deltaTime)
{
	m_ElapsedTime += deltaTime;
}

bool cWaitTimer::isFinished(void)
{
	// Has the amount of time passed
	if (this->m_ElapsedTime >= this->m_TimeToWait)
	{
		// We've arrived

		OnFinished();
		return true;
	}

	// Keep going...
	return false;
}

void cWaitTimer::OnFinished(void)
{
}
