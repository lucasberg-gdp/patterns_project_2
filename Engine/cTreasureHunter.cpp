#include "cTreasureHunter.h"
#include "MathUtils.h"
#include <glm/gtx/norm.hpp>

int cTreasureHunter::totalTreasuresCollected = 0;

cTreasureHunter::cTreasureHunter(cMesh* mesh, cDungeonRoom* initialRoom)
{
	m_objectMesh = mesh;
	currentRoom = initialRoom;
	currentRoom->hasBeenVisited = true;
	LookForTreasure();
}

void cTreasureHunter::Update()
{
	float squaredDistance = glm::distance2(GetObjectMesh()->drawPosition, targetRoom->position);
	float distance = std::sqrt(squaredDistance);

	if (distance < 0.2f)
	{
		if (targetRoom->hasTreasure)
		{
			if (targetRoom->treasure->bIsVisible)
			{
				this->treasuresCollected++;
				totalTreasuresCollected++;

				//std::cout << "Treasure hunter found treasure! Total for this hunter: " << treasuresCollected;
				//std::cout << "Total for the map: " << totalTreasuresCollected << std::endl;

				//if (totalTreasuresCollected >= 250)
				//{
				//	std::cout << "All treasured collected!" << std::endl;
				//}

				targetRoom->treasure->bIsVisible = false;
			}
		}

		targetRoom->hasBeenVisited = true;

		GetObjectMesh()->drawPosition = targetRoom->position;
		currentRoom = targetRoom;

		LookForTreasure();
	}

	if (!isMovingToNextNode)
	{
		switch (currentMovementDirection)
		{
		case Up:
			GetObjectMesh()->drawPosition.x += hunterVelocity;
			FaceDirection(glm::vec3(0.0f, 0.0f, 1.0f));
			break;
		case Down:
			GetObjectMesh()->drawPosition.x -= hunterVelocity;
			FaceDirection(glm::vec3(0.0f, 0.0f, -1.0f));
			break;
		case Left:
			GetObjectMesh()->drawPosition.z += hunterVelocity;
			FaceDirection(glm::vec3(-1.0f, 0.0f, 0.0f));
			break;
		case Right:
			GetObjectMesh()->drawPosition.z -= hunterVelocity;
			FaceDirection(glm::vec3(1.0f, 0.0f, 0.0f));
			break;
		case NoMovement:
			break;
		}
	}


}


void cTreasureHunter::LookForTreasure()
{
	bool validRoom = false;

	while (!validRoom)
	{
		unsigned int unvisitedRooms = 0;
		unsigned int unvisitedRoomsId = 0;

		for (unsigned int i = 0; i < currentRoom->surroundingRooms.size(); i++)
		{
			if (!currentRoom->surroundingRooms[i]->hasBeenVisited)
			{
				if (currentRoom->GetRoomAbove() == currentRoom->surroundingRooms[i])
				{
					unvisitedRooms++;
					unvisitedRoomsId = i;
				}
				else if (currentRoom->GetRoomBelow() == currentRoom->surroundingRooms[i])
				{
					unvisitedRooms++;
					unvisitedRoomsId = i;
				}
				else if (currentRoom->GetRoomLeft() == currentRoom->surroundingRooms[i])
				{
					unvisitedRooms++;
					unvisitedRoomsId = i;
				}
				else if (currentRoom->GetRoomRight() == currentRoom->surroundingRooms[i])
				{
					unvisitedRooms++;
					unvisitedRoomsId = i;
				}
			}
		}

		if (unvisitedRooms >= 1)
		{
			float upDifference = currentRoom->surroundingRooms[unvisitedRoomsId]->position.x - currentRoom->position.x;
			float leftRightDifference = currentRoom->surroundingRooms[unvisitedRoomsId]->position.z - currentRoom->position.z;

			if ((upDifference == 1.0f) && (leftRightDifference == 0.0f))
			{
				currentMovementDirection = Up;
				targetRoom = currentRoom->surroundingRooms[unvisitedRoomsId];
				return;
			}

			if ((upDifference == -1.0f) && (leftRightDifference == 0.0f))
			{
				currentMovementDirection = Down;
				targetRoom = currentRoom->surroundingRooms[unvisitedRoomsId];
				return;
			}

			if ((upDifference == 0.0f) && (leftRightDifference == 1.0f))
			{
				currentMovementDirection = Left;
				targetRoom = currentRoom->surroundingRooms[unvisitedRoomsId];
				return;
			}

			if ((upDifference == 0.0f) && (leftRightDifference == -1.0f))
			{
				currentMovementDirection = Right;
				targetRoom = currentRoom->surroundingRooms[unvisitedRoomsId];
				return;
			}

		}

		int randomRoomIndex = MathUtils::GetRandomInt(0, (int)currentRoom->surroundingRooms.size() - 1);

		float upDifference = currentRoom->surroundingRooms[randomRoomIndex]->position.x - currentRoom->position.x;
		float leftRightDifference = currentRoom->surroundingRooms[randomRoomIndex]->position.z - currentRoom->position.z;

		if ((upDifference == 1.0f) && (leftRightDifference == 0.0f))
		{
			currentMovementDirection = Up;
			targetRoom = currentRoom->surroundingRooms[randomRoomIndex];
			validRoom = true;
		}

		if ((upDifference == -1.0f) && (leftRightDifference == 0.0f))
		{
			currentMovementDirection = Down;
			targetRoom = currentRoom->surroundingRooms[randomRoomIndex];
			validRoom = true;
		}

		if ((upDifference == 0.0f) && (leftRightDifference == 1.0f))
		{
			currentMovementDirection = Left;
			targetRoom = currentRoom->surroundingRooms[randomRoomIndex];
			validRoom = true;
		}

		if ((upDifference == 0.0f) && (leftRightDifference == -1.0f))
		{
			currentMovementDirection = Right;
			targetRoom = currentRoom->surroundingRooms[randomRoomIndex];
			validRoom = true;
		}
	}
}

bool cTreasureHunter::HasCheckedRoom(int id)
{
	for (unsigned int i = 0; i < roomsCheckedIds.size(); i++)
	{
		if (roomsCheckedIds[i] == id)
		{
			return true;
		}
	}
	return false;
}

void cTreasureHunter::GetListOfUnvisitedRooms(std::vector<cDungeonRoom*>& unvisitedRooms)
{
	for (unsigned int i = 0; i < this->currentRoom->surroundingRooms.size(); i++)
	{
		if (!HasCheckedRoom(currentRoom->surroundingRooms[i]->id))
		{
			unvisitedRooms.push_back(currentRoom->surroundingRooms[i]);
		}
	}

}

//DWORD cTreasureHunter::UpdateTreasureHunter(LPVOID lpParameter)
//{
//	return 0;
//}

