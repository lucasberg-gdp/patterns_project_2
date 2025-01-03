#pragma once
#include "iCommand.h"
class cWaitTimer : public iCommand
{
	double m_ElapsedTime;
	double m_TimeToWait;

public:
	void Init(double timer);

	// Inherited via iCommand
	void Start(void) override;
	void Update(double deltaTime) override;
	bool isFinished(void) override;
	void OnFinished(void) override;
};

