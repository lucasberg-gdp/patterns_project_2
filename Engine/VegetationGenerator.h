#pragma once
#include <vector>
#include "cMesh.h"

class VegetationGenerator
{
public:
	struct ForestInfo
	{
		std::string Name;
		int MinNumOfTrees;
		int MaxNumOfTrees;
		
		cMesh* SurfaceMesh;
		std::vector<cMesh*> Trees;
	};

	ForestInfo forestInfo;

	void CreateForest(ForestInfo forestInfo);



};

