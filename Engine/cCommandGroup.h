#pragma once

#include "iCommand.h"
#include <vector>

class cCommandGroup : public iCommand
{
public:
	cCommandGroup();
	void addSerial(iCommand* pNewSerialCommand);
	void addParallel(iCommand* pNewSepNewParallelCommandrialCommand);

	// From the iCommand interface
	virtual void Start(void);
	virtual void Update(double deltaTime);
	virtual bool isFinished(void);
	virtual void OnFinished(void);

private:
	bool m_IsActive = false;

	std::vector< iCommand* > m_vecSerialCommands;
	std::vector< iCommand* >::iterator itCurSerialCommand; // = beginning 
	std::vector< iCommand* > m_vecParallelCommands;
};