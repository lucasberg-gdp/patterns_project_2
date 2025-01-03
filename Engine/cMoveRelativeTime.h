#pragma once

#include "iCommand.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include "sPhysicsProperties.h"


// Move from the current location to a new location
// ... over a certain amount of time
class cMoveRelativeTime : public iCommand
{
	enum MoveType {
		Linear,
		EasyIn,
		EasyOut,
		EasyInAndOut
	};

	glm::vec3 m_startXYZ;
	glm::vec3 m_endXYZ;
	double m_timeToMove = 0.0;
	sPhysicsProperties* m_pObject = nullptr;
	// This is calculated
	double m_elapsedTime = 0.0;

	glm::vec3 m_velocity = glm::vec3(0.0f);
	glm::vec3 m_acceleration = glm::vec3(0.0f);

	MoveType m_moveType = MoveType::Linear;
public:
	// Call this to set the values
	// This specific to this particular command
	void Init(sPhysicsProperties* pObject, glm::vec3 endXYZ, double timeToMove);
	void Init(sPhysicsProperties* pObject, glm::vec3 endXYZ, double timeToMove, bool easeIn, bool easeOut);

	// From iCommand interface
	virtual void Start(void);
	virtual void Update(double deltaTime);
	virtual bool isFinished(void);
	virtual void OnFinished(void);
};
