#include "cCommandFactory.h"

// Abstract factory: includes specific classes here
// NEVER in the header
#include "cMoveRelativeTime.h"
#include "cFollowObject_A.h"
#include "cFollowBezierCubicCurve.h"
#include "cOrientTo.h"
#include "cLookAtObject.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "cWaitTimer.h"
#include <sstream>
#include "cLightSwitch.h"
#include "cAddVelocity.h"
#include "cFlickerLight.h"

cCommandFactory::cCommandFactory()
{
	this->m_pPhysicEngine = NULL;
	this->m_vecMeshes = NULL;
}

void cCommandFactory::SetPhysics(cPhysics* m_pPhysicEngine)
{
	this->m_pPhysicEngine = m_pPhysicEngine;
	return;
}

// Maybe also the list of meshes?
void cCommandFactory::setVectorOfMeshes(std::vector< cMesh* >* p_vecMeshes)
{
	this->m_vecMeshes = p_vecMeshes;
	return;
}

iCommand* cCommandFactory::CreateCommandObject(
	std::string command,
	std::vector< std::string > vecDetails)
{
	std::vector<std::string> commands;
	std::stringstream ss(command);
	std::string word;

	while (ss >> word) {
		commands.push_back(word);
	}

	if (commands[0] == "AddVelocity")
	{
		cAddVelocity* addVelocity = new cAddVelocity();

		sPhysicsProperties* pMeshToControl = m_pPhysicEngine->findShapeByFriendlyName(vecDetails[0]);

		glm::vec3 velocityDirection = glm::vec3(
			atof(vecDetails[1].c_str()),
			atof(vecDetails[2].c_str()),
			atof(vecDetails[3].c_str()));

		double timeToAddVelocity = atof(vecDetails[4].c_str());
		float velocity = (float)atof(vecDetails[5].c_str());
		double totalTime = atof(vecDetails[6].c_str());

		addVelocity->Init(pMeshToControl, timeToAddVelocity, totalTime, velocityDirection, velocity);
		return addVelocity;
	}
	if (commands[0] == "LookAt")
	{
		std::string movementType = commands[1];
		std::string friendlyName = commands[2];

		cLookAtObject* lookAtCommand = new cLookAtObject();

		sPhysicsProperties* pMeshToControl = m_pPhysicEngine->findShapeByFriendlyName(vecDetails[0]);
		sPhysicsProperties* pMeshToLookAt = m_pPhysicEngine->findShapeByFriendlyName(friendlyName);

		float rotationSpeed = (float)atof(vecDetails[1].c_str());

		if (movementType == "Linear")
		{
			lookAtCommand->Init(pMeshToControl, pMeshToLookAt, rotationSpeed);
			return lookAtCommand;
		}
		if (movementType == "EaseIn")
		{
			lookAtCommand->Init(pMeshToControl, pMeshToLookAt, rotationSpeed, true, false);
			return lookAtCommand;
		}
		if (movementType == "EaseOut")
		{
			lookAtCommand->Init(pMeshToControl, pMeshToLookAt, rotationSpeed, false, true);
			return lookAtCommand;
		}
		if (movementType == "EaseInAndOut")
		{
			lookAtCommand->Init(pMeshToControl, pMeshToLookAt, rotationSpeed, true, true);
			return lookAtCommand;
		}
	}
	if (commands[0] == "TurnLight")
	{
		cLightSwitch* lightSwitchCommand = new cLightSwitch();

		std::string lightName = vecDetails[0];

		double timeToSwitchLight = atof(vecDetails[1].c_str());

		if (commands[1] == "On")
		{
			lightSwitchCommand->Init(lightName, timeToSwitchLight, true);
			return lightSwitchCommand;
		}
		else if (commands[1] == "Off")
		{
			lightSwitchCommand->Init(lightName, timeToSwitchLight, false);
			return lightSwitchCommand;
		}
	}
	if (command == "Move Relative Linear")
	{
		cMoveRelativeTime* pMoveCommand = new cMoveRelativeTime();

		sPhysicsProperties* pMeshToControl = m_pPhysicEngine->findShapeByFriendlyName(vecDetails[0]);		// "New_Viper_Player"
		glm::vec3 destinationXYZ = glm::vec3(
			    atof(vecDetails[1].c_str()),
				atof(vecDetails[2].c_str()),
				atof(vecDetails[3].c_str()));
		
		float timeToMove = (float)atof(vecDetails[4].c_str());

		pMoveCommand->Init(pMeshToControl, destinationXYZ, timeToMove);
		return pMoveCommand;
	}
	if (command == "Follow Object, offset & max velocity")
	{
		iCommand* pFollow = new cFollowObject_A();

		return pFollow;
	}
	if (command == "Move Relative EaseIn")
	{
		cMoveRelativeTime* pMoveCommand = new cMoveRelativeTime();

		sPhysicsProperties* pMeshToControl = m_pPhysicEngine->findShapeByFriendlyName(vecDetails[0]);		// "New_Viper_Player"
		glm::vec3 destinationXYZ = glm::vec3(
			atof(vecDetails[1].c_str()),
			atof(vecDetails[2].c_str()),
			atof(vecDetails[3].c_str()));

		float timeToMove = (float)atof(vecDetails[4].c_str());

		pMoveCommand->Init(pMeshToControl, destinationXYZ, timeToMove, true, false);
		return pMoveCommand;
	}
	if (command == "Move Relative EaseOut")
	{
		cMoveRelativeTime* pMoveCommand = new cMoveRelativeTime();

		sPhysicsProperties* pMeshToControl = m_pPhysicEngine->findShapeByFriendlyName(vecDetails[0]);		// "New_Viper_Player"
		glm::vec3 destinationXYZ = glm::vec3(
			atof(vecDetails[1].c_str()),
			atof(vecDetails[2].c_str()),
			atof(vecDetails[3].c_str()));

		float timeToMove = (float)atof(vecDetails[4].c_str());

		pMoveCommand->Init(pMeshToControl, destinationXYZ, timeToMove, false, true);
		return pMoveCommand;
	}
	if (command == "Move Relative EaseInAndOut")
	{
		cMoveRelativeTime* pMoveCommand = new cMoveRelativeTime();

		sPhysicsProperties* pMeshToControl = m_pPhysicEngine->findShapeByFriendlyName(vecDetails[0]);
		glm::vec3 destinationXYZ = glm::vec3(
			atof(vecDetails[1].c_str()),
			atof(vecDetails[2].c_str()),
			atof(vecDetails[3].c_str()));

		float timeToMove = (float)atof(vecDetails[4].c_str());

		pMoveCommand->Init(pMeshToControl, destinationXYZ, timeToMove, true, true);
		return pMoveCommand;
	}
	if (command == "Orient Relative EaseIn")
	{
		cOrientTo* orientCommand = new cOrientTo();

		sPhysicsProperties* pMeshToControl = m_pPhysicEngine->findShapeByFriendlyName(vecDetails[0]);
		glm::vec3 eulerOrientation = glm::vec3(
			glm::radians(atof(vecDetails[1].c_str())),
			glm::radians(atof(vecDetails[2].c_str())),
			glm::radians(atof(vecDetails[3].c_str())));

		float timeToMove = (float)atof(vecDetails[4].c_str());

		orientCommand->Init(pMeshToControl, eulerOrientation, timeToMove, true, false);
		return orientCommand;
	}
	if (command == "Orient Relative EaseOut")
	{
		cOrientTo* orientCommand = new cOrientTo();

		sPhysicsProperties* pMeshToControl = m_pPhysicEngine->findShapeByFriendlyName(vecDetails[0]);
		glm::vec3 eulerOrientation = glm::vec3(
			glm::radians(atof(vecDetails[1].c_str())),
			glm::radians(atof(vecDetails[2].c_str())),
			glm::radians(atof(vecDetails[3].c_str())));

		float timeToMove = (float)atof(vecDetails[4].c_str());

		orientCommand->Init(pMeshToControl, eulerOrientation, timeToMove, false, true);
		return orientCommand;
	}
	if (command == "Orient Relative EaseInAndOut")
	{
		cOrientTo* orientCommand = new cOrientTo();

		sPhysicsProperties* pMeshToControl = m_pPhysicEngine->findShapeByFriendlyName(vecDetails[0]);
		glm::vec3 eulerOrientation = glm::vec3(
			glm::radians(atof(vecDetails[1].c_str())),
			glm::radians(atof(vecDetails[2].c_str())),
			glm::radians(atof(vecDetails[3].c_str())));

		float timeToMove = (float)atof(vecDetails[4].c_str());

		orientCommand->Init(pMeshToControl, eulerOrientation, timeToMove, true, true);
		return orientCommand;
	}
	if (command == "Follow Curve CubicBezier")
	{
		cFollowBezierCubicCurve* followBezierCommand = new cFollowBezierCubicCurve();

		sPhysicsProperties* pMeshToControl = m_pPhysicEngine->findShapeByFriendlyName(vecDetails[0]);		// "New_Viper_Player"

		std::vector<glm::vec3> controlPoints;

		glm::vec3 controlPoint1 = glm::vec3(
			atof(vecDetails[1].c_str()),
			atof(vecDetails[2].c_str()),
			atof(vecDetails[3].c_str()));

		glm::vec3 controlPoint2 = glm::vec3(
			atof(vecDetails[4].c_str()),
			atof(vecDetails[5].c_str()),
			atof(vecDetails[6].c_str()));

		glm::vec3 controlPoint3 = glm::vec3(
			atof(vecDetails[7].c_str()),
			atof(vecDetails[8].c_str()),
			atof(vecDetails[9].c_str()));

		glm::vec3 controlPoint4 = glm::vec3(
			atof(vecDetails[10].c_str()),
			atof(vecDetails[11].c_str()),
			atof(vecDetails[12].c_str()));

		controlPoints.push_back(controlPoint1);
		controlPoints.push_back(controlPoint2);
		controlPoints.push_back(controlPoint3);
		controlPoints.push_back(controlPoint4);

		float timeToMove = (float)atof(vecDetails[13].c_str());

		followBezierCommand->Init(pMeshToControl, controlPoints, timeToMove);
		return followBezierCommand;
	}
	if (command == "Follow Linear Facing")
	{

	}
	if (command == "Wait Time")
	{
		cWaitTimer* timerCommand = new cWaitTimer();

		double timeToWait = atof(vecDetails[1].c_str());

		timerCommand->Init(timeToWait);
		return timerCommand;
	}

	return NULL;
}

iCommand* cCommandFactory::CreateTriggerCommandObject(std::string command, std::vector<std::string> vecDetails)
{
	if (command == "Follow Triggered Linear")
	{
		cFollowObject_A* followObject = new cFollowObject_A();

		sPhysicsProperties* pMeshToTrigger = m_pPhysicEngine->findShapeByFriendlyName(vecDetails[0]);		// "New_Viper_Player"
		sPhysicsProperties* pMeshToControl = m_pPhysicEngine->findShapeByFriendlyName(vecDetails[1]);		// "New_Viper_Player"

		std::vector<glm::vec3> controlPoints;

		//AddLocationTriggerCommand("camera", "beholder", "Follow Triggered Linear",
		//	1000.0, 0.0, 1000.0, --centerPosition
		//	800.0, --radius
		//	5.0, --time
		//	300.0, --followDistance
		//	0.0, 0.0, 0.0, --offset
		//	30.0, --maxFollowSpeed
		//	500.0, 700.0) --minAccRange, maxAccRange


		float timeToMove = (float)atof(vecDetails[6].c_str());

		glm::vec3 offSet = glm::vec3(
			atof(vecDetails[8].c_str()),
			atof(vecDetails[9].c_str()),
			atof(vecDetails[10].c_str()));

		float followDistance = (float)atof(vecDetails[7].c_str());
		float maxFollowSpeed = (float)atof(vecDetails[11].c_str());
		float minAccRange = (float)atof(vecDetails[12].c_str());
		float maxAccRange = (float)atof(vecDetails[13].c_str());

		followObject->Init(pMeshToControl, pMeshToTrigger, timeToMove,
			followDistance, offSet, maxFollowSpeed, minAccRange, maxAccRange
			);
		return followObject;
	}

	return NULL;
}

iCommand* cCommandFactory::CreateLoopingCommandObject(std::string command, std::vector<std::string> vecDetails)
{
	std::vector<std::string> commands;
	std::stringstream ss(command);
	std::string word;

	while (ss >> word) 
	{
		commands.push_back(word);
	}

	if (commands[0] == "FlickerLight")
	{
		std::string lightName = vecDetails[0];

		glm::vec4 attenuation1 = glm::vec4(
			atof(vecDetails[1].c_str()),
			atof(vecDetails[2].c_str()),
			atof(vecDetails[3].c_str()),
			atof(vecDetails[4].c_str()));

		glm::vec4 attenuation2 = glm::vec4(
			atof(vecDetails[5].c_str()),
			atof(vecDetails[6].c_str()),
			atof(vecDetails[7].c_str()),
			atof(vecDetails[8].c_str()));

		double flickerSpeed = atof(vecDetails[9].c_str());
		cFlickerLight* flickerLight = new cFlickerLight();
		flickerLight->Init(lightName, attenuation1, attenuation2, flickerSpeed);
		return flickerLight;
	}

	return nullptr;
}
