#include "cCommandGroup.h"

cCommandGroup::cCommandGroup()
{
	// point the iterator to the begining of the serial vector
	// (which is also the end() if there aren't any commands
	this->itCurSerialCommand = this->m_vecSerialCommands.begin();
}

void cCommandGroup::addSerial(iCommand* pNewSerialCommand)
{
	this->m_vecSerialCommands.push_back(pNewSerialCommand);
	return;
}

void cCommandGroup::addParallel(iCommand* pNewParallelCommand)
{
	if (pNewParallelCommand)
	{
		this->m_vecParallelCommands.push_back(pNewParallelCommand);
	}

	return;
}

void cCommandGroup::Start(void)
{
	// TODO: 
	if (m_vecSerialCommands.size() > 0)
	{
		this->itCurSerialCommand = this->m_vecSerialCommands.begin();
		(*itCurSerialCommand)->Start();
	}

	for (iCommand* pCurPC : this->m_vecParallelCommands)
	{
		pCurPC->Start();
	}

	m_IsActive = true;
}

void cCommandGroup::Update(double deltaTime)
{
	if (!m_IsActive)
	{
		return;
	}

	// If the parallel commands are NOT done, call update
	for (iCommand* pCurPC : this->m_vecParallelCommands)
	{
		if (!pCurPC->isFinished())
		{
			pCurPC->Update(deltaTime);
		}
	}

	if (!this->m_vecSerialCommands.empty())
	{
		// There is at least one command in the vector of serial commands. 
		// Is this one done?
		if (!this->m_vecSerialCommands[0]->isFinished())
		{
			// Call update
			this->m_vecSerialCommands[0]->Update(deltaTime);
		}
		else
		{
			this->m_vecSerialCommands[0]->OnFinished();

			// TODO: Do we actually delete these? 
			// (Something to think about because if we don't, there's a memory leak)
			delete this->m_vecSerialCommands[0];
			this->m_vecSerialCommands.erase(this->m_vecSerialCommands.begin());

			if (!this->m_vecSerialCommands.empty())
			{
				this->m_vecSerialCommands[0]->Start();
			}
		}
	}

	return;
}
bool cCommandGroup::isFinished(void)
{
	// All parallel commands are done when all the commands in vector reutrn isFinished
	for (iCommand* pCurPC : this->m_vecParallelCommands)
	{
		if (!pCurPC->isFinished())
		{
			// One of these ISN'T done
			return false;
		}
	}

	// Serial: is the iterator pointing to the end of the vector?
	if (!this->m_vecSerialCommands.empty())
	{
		// No, so there are still active serial commands
		return false;
	}

	// every command is done
	return true;
}

void cCommandGroup::OnFinished(void)
{
	// TODO: 
	return;
}