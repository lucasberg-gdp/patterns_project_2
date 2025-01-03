#pragma once

#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
class cMeshNav
{
public:
	cMeshNav();

	bool CalculateNavMeshFromPly(std::string plyFileName);

	struct sNode
	{
		glm::vec3 position;
		struct sNodeCostPair
		{
			sNodeCostPair(){}
			sNode* pNode = NULL;
			float cost = 0.0f;

			sNodeCostPair(sNode* pNode_, float cost_)
			{
				pNode = pNode_;
				cost = cost_;
			}
		};
		std::vector< sNodeCostPair > vec_pAdjacentNodes;
	};

	std::vector < sNode* > vec_pNodes;

	float CalcCost(sNode* pStart, sNode* pEnd);

	sNode* pFindClosestNode(glm::vec2 XZ);

	bool CalculateNavMeshFromFile(std::string fileName);
};

