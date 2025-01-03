#pragma once
#include "cGameObject.h"
#include "cMazeMaker.h"

class cTreasureHunter : public cGameObject
{
public:
	float hunterVelocity = 0.1f;

	enum MovementDiretion {
		Up,
		Down,
		Left,
		Right,
		NoMovement
	};

	cTreasureHunter(cMesh* mesh, cDungeonRoom* initialRoom);

	void Update();

	void LookForTreasure();

	int treasuresCollected = 0;

	static int totalTreasuresCollected;

	cDungeonRoom* currentRoom = nullptr;
	cDungeonRoom* targetRoom = nullptr;

	std::vector<int> roomsCheckedIds;

	bool HasCheckedRoom(int id);
	
	MovementDiretion currentMovementDirection = NoMovement;
	bool isMovingToNextNode = false;

	void GetListOfUnvisitedRooms(std::vector<cDungeonRoom*>& unvisitedRooms);

	int KeepGoingSameDirection = 3;
};

