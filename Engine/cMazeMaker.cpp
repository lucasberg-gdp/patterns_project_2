#include "cMazeMaker.h"

#include <fstream>
#include "cCube.h"
#include "cScene.h"
#include "cColor.h"

extern cScene* g_currentScene;

cMazeMaker::cMazeMaker()
{
	this->m_maze_size[2] = { 0 };

	this->m_start_axis = 0;
	this->m_start_side = 0;

	static const unsigned int UP = 0;
	static const unsigned int DOWN = 1;
	static const unsigned int LEFT = 2;
	static const unsigned int RIGHT = 3;

	return;
}

// Select a random direction based on our options, append it to the current path, and move there
bool cMazeMaker::m_randomMove(bool first_move)
{
	int random_neighbor;
	std::vector< std::vector< int > > unvisited_neighbors;

	for (int direction = 0; direction < 4; direction++)
	{
		int possible_pmd[2] = { 0, 0 };

		if (direction == UP)
		{
			possible_pmd[1] = -1;
		}
		else if (direction == DOWN)
		{
			possible_pmd[1] = 1;
		}
		else if (direction == LEFT)
		{
			possible_pmd[0] = -1;
		}
		else
		{
			possible_pmd[0] = 1;
		}

		if (this->m_dfs_path.back()[0] + possible_pmd[0] * 2 > 0 &&
			this->m_dfs_path.back()[0] + possible_pmd[0] * 2 < this->m_maze_size[0] - 1 &&
			this->m_dfs_path.back()[1] + possible_pmd[1] * 2 > 0 &&
			this->m_dfs_path.back()[1] + possible_pmd[1] * 2 < this->m_maze_size[1] - 1)
		{
			if (!maze[this->m_dfs_path.back()[1] + possible_pmd[1] * 2]
				[this->m_dfs_path.back()[0] + possible_pmd[0] * 2][1])
			{
				std::vector< int > possible_move_delta = { possible_pmd[0], possible_pmd[1] };

				unvisited_neighbors.push_back(possible_move_delta);
			}
		}
	}

	if (unvisited_neighbors.size() > 0)
	{
		random_neighbor = rand() % unvisited_neighbors.size();

		for (int a = 0; a < !first_move + 1; a++)
		{
			std::vector< int > new_location;

			new_location.push_back(this->m_dfs_path.back()[0] + unvisited_neighbors[random_neighbor][0]);
			new_location.push_back(this->m_dfs_path.back()[1] + unvisited_neighbors[random_neighbor][1]);

			this->m_dfs_path.push_back(new_location);

			this->maze[this->m_dfs_path.back()[1]][this->m_dfs_path.back()[0]][0] = false;
			this->maze[this->m_dfs_path.back()[1]][this->m_dfs_path.back()[0]][1] = true;
		}

		return true;
	}
	else
	{
		return false;
	}
}

bool cMazeMaker::m_validInteger(char* cstr)
{
	std::string str(cstr);

	for (char& c : str)
	{
		if (!isdigit(c))
		{
			return false;
		}
	}

	return true;
}
// The fun part ;)
bool cMazeMaker::GenerateMaze(unsigned int width, unsigned int height)
{
	this->m_maze_size[0] = width;
	this->m_maze_size[1] = height;

	if ((width < 5) || (height < 5))
	{
		return false;
	}

	// The width and height must be greater than or equal to 5 or it won't work
	// The width and height must be odd or else we will have extra walls
	for (int a = 0; a < 2; a++)
	{
		if (this->m_maze_size[a] < 5)
		{
			this->m_maze_size[a] = 5;
		}
		else if (this->m_maze_size[a] % 2 == 0)
		{
			this->m_maze_size[a]--;
		}
	}

	this->m_initializeMaze();
	this->m_randomPoint(false);
	this->m_randomPoint(true);

	bool first_move = true;
	bool success = true;


	while ((int)this->m_dfs_path.size() > 1 - first_move)
	{
		if (!success)
		{
			this->m_dfs_path.pop_back();

			if (!first_move && this->m_dfs_path.size() > 2)
			{
				this->m_dfs_path.pop_back();
			}
			else
			{
				break;
			}

			success = true;
		}

		while (success)
		{
			success = this->m_randomMove(first_move);

			if (first_move)
			{
				first_move = false;
			}
		}
	}

	return true;
}

// Initialize the maze vector with a completely-filled grid with the size the user specified
void cMazeMaker::m_initializeMaze(void)
{
	for (int a = 0; a < this->m_maze_size[1]; a++)
	{
		for (int b = 0; b < this->m_maze_size[0]; b++)
		{
			bool is_border;

			if (a == 0 || a == this->m_maze_size[1] - 1 ||
				b == 0 || b == this->m_maze_size[0] - 1)
			{
				is_border = true;
			}
			else
			{
				is_border = false;
			}

			std::vector< bool > new_cell = { true, is_border };

			if ((unsigned int)a + 1 > this->maze.size())
			{
				std::vector< std::vector< bool > > new_row = { new_cell };

				this->maze.push_back(new_row);
			}
			else
			{
				this->maze[a].push_back(new_cell);
			}
		}
	}
}



unsigned int x = -5;		// integer


void cMazeMaker::PrintMaze(void)
{
	for (unsigned int a = 0; a < this->maze.size(); a++)
	{
		for (unsigned int b = 0; b < this->maze[a].size(); b++)
		{
			if (this->maze[a][b][0])
			{
				std::cout << "X";
				//				std::cout << "X";
				//				std::cout << '\u2500';
				//				std::cout << (char)0xC5;
				//				std::cout << (char)0xDA; //   (218)
				//				std::cout << (char)0xBF; //   (191)
				//				std::cout << (char)0xC0; //   (192)
				//				std::cout << (char)0xD9; //   (217)
			}
			else
			{
				std::cout << " ";
			}
		}

		std::cout << std::endl;
	}
	return;
}

void cMazeMaker::CopyToCharArray(void)
{
	for (unsigned int a = 0; a < this->maze.size(); a++)
	{
		for (unsigned int b = 0; b < this->maze[a].size(); b++)
		{
			if (this->maze[a][b][0])
			{
				TheMaze[a][b] = 'X';
			}
			else
			{
				TheMaze[a][b] = ' ';
			}
		}
	}
	return;
}

void cMazeMaker::PrintMazeToFile(std::string filename)
{
	std::ofstream mazeFile(filename.c_str());

	for (unsigned int a = 0; a < this->maze.size(); a++)
	{
		for (unsigned int b = 0; b < this->maze[a].size(); b++)
		{
			if (this->maze[a][b][0])
			{
				mazeFile << "X";
			}
			else
			{
				mazeFile << " ";
			}
		}

		mazeFile << std::endl;
	}

	mazeFile.close();


	return;
}

// gems midterm
void cMazeMaker::ReadMazeFromFile(std::string fileName )
{
	std::ifstream mazeFile(fileName.c_str());

	std::string text; 
	mazeFile >> text;
	std::cout << text << std::endl;

	mazeFile >> text;
	std::cout << text << std::endl;

	mazeFile >> text;
	std::cout << text << std::endl;


	mazeFile >> text;

	unsigned int numOfRows = std::stoi(text);
	std::cout << text << std::endl;

	mazeFile >> text;
	std::cout << text << std::endl;

	mazeFile >> text;
	std::cout << text << std::endl;

	mazeFile >> text;
	std::cout << text << std::endl;
	unsigned int numOfColumns = std::stoi(text);

	// first map
	while (text != "DUNGEON_BEGIN")
	{
		mazeFile >> text;
	}

	mazeFile >> text;
	while (text != "DUNGEON_END")
	{
		//std::cout << text << std::endl;
		mazeFile >> text;
	}

	//second map
	while (text != "DUNGEON_BEGIN")
	{
		mazeFile >> text;
	}

	mazeFile >> text;
	
	while (text != "DUNGEON_END")
	{


		rowsAndColumns.push_back(text);
		//std::cout << text << std::endl;
		mazeFile >> text;
	}

	float MeshOffSet = 1.0f;

	for (int i = 0; i < rowsAndColumns.size(); i++)
	{
		std::cout << rowsAndColumns[i] << std::endl;

		for (int j = 0; j < rowsAndColumns[i].size(); j++)
		{
			if (rowsAndColumns[i][j] == 'X')
			{
				glm::vec3 meshPosition = glm::vec3(
					(rowsAndColumns.size() - i) * MeshOffSet,
					0.0f, 
					j * MeshOffSet);

				cMesh* wall = new cMesh();
				wall->meshName = "shapes/Quad_xyz_n_rgba.ply";
				wall->drawPosition = meshPosition;
				wall->friendlyName = "wall";
				wall->bUseDebugColours = true;
				wall->wholeObjectDebugColourRGBA = cColor::COLOR_RED;

				g_currentScene->AddMesh(wall);

			}

			if (rowsAndColumns[i][j] == '.')
			{
				glm::vec3 meshPosition = glm::vec3(
					(rowsAndColumns.size() - i) * MeshOffSet,
					0.0f,
					j * MeshOffSet);

				cMesh* wall = new cMesh();
				wall->meshName = "shapes/Quad_xyz_n_rgba.ply";
				wall->drawPosition = meshPosition;
				wall->friendlyName = "wall";
				wall->bUseDebugColours = true;
				wall->wholeObjectDebugColourRGBA = cColor::COLOR_WHITE;

				g_currentScene->AddMesh(wall);

				cDungeonRoom* dungeonRoom = new cDungeonRoom();
				dungeonRoom->id += (unsigned int)(wall->drawPosition.x * 1'000'000.0f);
				dungeonRoom->id += (unsigned int)wall->drawPosition.z;
				dungeonRoom->position = wall->drawPosition;
				//dungeonRoom.tsvChar = 'X';
				dungeonRooms.push_back(dungeonRoom);
			}
		}
	}

	// fill surrounding rooms
	for (unsigned int i = 0; i < dungeonRooms.size(); i++)
	{
		for (unsigned int j = 0; j < dungeonRooms.size(); j++)
		{
			float distance = glm::distance(dungeonRooms[j]->position, dungeonRooms[i]->position);
			if ((distance < 1.5f) && (distance > 0.2f))
			{
				dungeonRooms[i]->surroundingRooms.push_back(dungeonRooms[j]);
			}
		}
	}



	return;
	//cCube* emptySpace = new cCube("cube");
//float xOffSet = 1.0f; 
//float yOffSet = 1.0f;
}

//void cMazeMaker::ReadMazeFromFile(std::string fileName)
//{
//	std::ifstream mazeFile(fileName.c_str());
//
//	for (unsigned int a = 0; a < 49; a++)
//	{
//		std::string text;
//		mazeFile >> text;
//
//		for (unsigned int b = 0; b < 49; b++)
//		{
//			if (text[b] == 'X')
//			{
//				maze[a][b][0] = true;
//			}
//			else
//			{
//				maze[a][b][0] = false;
//			}
//		}
//	}
//
//	mazeFile.close();
//
//	return;
//}


// Set a random point (start or end)
void cMazeMaker::m_randomPoint(bool part)
{
	int axis;
	int side;

	if (!part)
	{
		axis = rand() % 2;
		side = rand() % 2;

		this->m_start_axis = axis;
		this->m_start_side = side;
	}
	else
	{
		bool done = false;

		while (!done)
		{
			axis = rand() % 2;
			side = rand() % 2;

			if (axis != this->m_start_axis ||
				side != this->m_start_side)
			{
				done = true;
			}
		}
	}

	std::vector< int > location = { 0, 0 };

	if (!side)
	{
		location[!axis] = 0;
	}
	else
	{
		location[!axis] = this->m_maze_size[!axis] - 1;
	}

	location[axis] = 2 * (rand() % ((this->m_maze_size[axis] + 1) / 2 - 2)) + 1;

	if (!part)
	{
		this->m_dfs_path.push_back(location);
	}

	this->maze[location[1]][location[0]][0] = false;
	this->maze[location[1]][location[0]][1] = true;

	return;
}

cDungeonRoom* cDungeonRoom::GetRoomAbove()
{
	for (unsigned int i = 0; i < this->surroundingRooms.size(); i++)
	{
		// distance in x should be 1
		float distanceInX = surroundingRooms[i]->position.x - position.x;
		float distanceInZ = surroundingRooms[i]->position.z - position.z;

		if ((distanceInX == 1.0f) && (distanceInZ == 0.0f))
		{
			if (surroundingRooms[i]->position.z == position.z)
			{
				return surroundingRooms[i];
			}
		}
	}
	return nullptr;
}

cDungeonRoom* cDungeonRoom::GetRoomBelow()
{
	for (unsigned int i = 0; i < this->surroundingRooms.size(); i++)
	{
		// distance in x should be 1
		float distanceInX = surroundingRooms[i]->position.x - position.x;
		float distanceInZ = surroundingRooms[i]->position.z - position.z;


		if ((distanceInX == -1.0f) && (distanceInZ == 0.0f))
		{
			if (surroundingRooms[i]->position.z == position.z)
			{
				return surroundingRooms[i];
			}
		}
	}
	return nullptr;
}

cDungeonRoom* cDungeonRoom::GetRoomLeft()
{
	for (unsigned int i = 0; i < this->surroundingRooms.size(); i++)
	{
		// distance in x should be 1
		float distanceInX = surroundingRooms[i]->position.x - position.x;
		float distanceInZ = surroundingRooms[i]->position.z - position.z;

		if ((distanceInX == 0.0f) && (distanceInZ == 1.0f))
		{
			if (surroundingRooms[i]->position.x == position.x)
			{
				return surroundingRooms[i];
			}
		}
	}
	return nullptr;
}

cDungeonRoom* cDungeonRoom::GetRoomRight()
{
	for (unsigned int i = 0; i < this->surroundingRooms.size(); i++)
	{
		// distance in x should be 1

		float distanceInX = surroundingRooms[i]->position.x - position.x;
		float distanceInZ = surroundingRooms[i]->position.z - position.z;

		if ((distanceInX == 0.0f) && (distanceInZ == -1.0f))
		{
			if (surroundingRooms[i]->position.x == position.x)
			{
				return surroundingRooms[i];
			}
		}
	}
	return nullptr;
}
