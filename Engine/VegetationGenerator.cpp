#include "VegetationGenerator.h"
#include "cScene.h"
#include "MathUtils.h"

extern cScene* g_currentScene;

void VegetationGenerator::CreateForest(ForestInfo forestInfo)
{
	forestInfo.SurfaceMesh->isFoliage = true;

	int randomNumberOfTrees = MathUtils::GetRandomInt(forestInfo.MinNumOfTrees, forestInfo.MaxNumOfTrees);

	//for (unsigned int i = 0; i < randomNumberOfTrees; i++)
	//{
	//	cMesh* randomTree = MathUtils::GetRandomMeshFromVector(forestInfo.Trees);

	//	cMesh* treeCopy = new cMesh(randomTree);

	//	treeCopy->drawPosition = forestInfo.SurfaceMesh->GetRandomPointOnSurface();

	//	g_currentScene->AddMesh(randomTree);
	//}
}
