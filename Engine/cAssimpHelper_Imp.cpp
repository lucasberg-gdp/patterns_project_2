//#include "cFileLoader_Imp.h"
//
////#include <iostream>
//
//#include <assimp/Importer.hpp>      // C++ importer interface
//#include <assimp/scene.h>           // Output data structure
//#include <assimp/postprocess.h>     // Post processing flags
//
//// Note: Depending on the library, you have to use the specific config.h
//// file from the lib folder. This is generated by CMake.
//// This config is VS22vc143x64static
//
//#include <sstream>
//
//
//
//cFileLoader_Imp::cFileLoader_Imp()
//{
//	this->numberOfModelLoaded = 0;
//}
//
//cFileLoader_Imp::~cFileLoader_Imp()
//{
//
//}
//
//
//
//
//
//
//AH::cScene* cFileLoader_Imp::pGetScenePointer(std::string filename)
//{
//	std::map< std::string, AH::cScene* >::iterator itScene = this->map_Scenes_by_filename.find(filename);
//
//	if (itScene == this->map_Scenes_by_filename.end())
//	{
//		return NULL;
//	}
//
//	return itScene->second;
//}
//
//bool cFileLoader_Imp::cloneScene(std::string filename, AH::cScene& theScene)
//{
//
//	return false;
//}
//
//unsigned int cFileLoader_Imp::m_loadAssimpPostProcessingFlags(AH::cFileLoader::sPostProcessFlags postProcessOptions)
//{
//	unsigned int assimpPostProcessingFlags = 0;
//
//	if (postProcessOptions.bProcess_CalcTangentSpace)
//	{
//		assimpPostProcessingFlags |= aiProcess_CalcTangentSpace;
//	}
//	if (postProcessOptions.bProcess_CalcTangentSpace)
//	{
//		assimpPostProcessingFlags |= aiProcess_CalcTangentSpace;
//	}
//	if (postProcessOptions.bProcess_JoinIdenticalVertices)
//	{
//		assimpPostProcessingFlags |= aiProcess_JoinIdenticalVertices;
//	}
//	if (postProcessOptions.bProcess_MakeLeftHanded)
//	{
//		assimpPostProcessingFlags |= aiProcess_MakeLeftHanded;
//	}
//	if (postProcessOptions.bProcess_Triangulate)
//	{
//		assimpPostProcessingFlags |= aiProcess_Triangulate;
//	}
//	if (postProcessOptions.bProcess_RemoveComponent)
//	{
//		assimpPostProcessingFlags |= aiProcess_RemoveComponent;
//	}
//	if (postProcessOptions.bProcess_GenNormals)
//	{
//		assimpPostProcessingFlags |= aiProcess_GenNormals;
//	}
//	if (postProcessOptions.bProcess_GenSmoothNormals)
//	{
//		assimpPostProcessingFlags |= aiProcess_GenSmoothNormals;
//	}
//	if (postProcessOptions.bProcess_SplitLargeMeshes)
//	{
//		assimpPostProcessingFlags |= aiProcess_SplitLargeMeshes;
//	}
//	if (postProcessOptions.bProcess_PreTransformVertices)
//	{
//		assimpPostProcessingFlags |= aiProcess_PreTransformVertices;
//	}
//	if (postProcessOptions.bProcess_LimitBoneWeights)
//	{
//		assimpPostProcessingFlags |= aiProcess_LimitBoneWeights;
//	}
//	if (postProcessOptions.bProcess_ValidateDataStructure)
//	{
//		assimpPostProcessingFlags |= aiProcess_ValidateDataStructure;
//	}
//	if (postProcessOptions.bProcess_ImproveCacheLocality)
//	{
//		assimpPostProcessingFlags |= aiProcess_ImproveCacheLocality;
//	}
//	if (postProcessOptions.bProcess_RemoveRedundantMaterials)
//	{
//		assimpPostProcessingFlags |= aiProcess_RemoveRedundantMaterials;
//	}
//	if (postProcessOptions.bProcess_FixInfacingNormals)
//	{
//		assimpPostProcessingFlags |= aiProcess_FixInfacingNormals;
//	}
//	if (postProcessOptions.bProcess_SortByPType)
//	{
//		assimpPostProcessingFlags |= aiProcess_SortByPType;
//	}
//	if (postProcessOptions.bProcess_FindDegenerates)
//	{
//		assimpPostProcessingFlags |= aiProcess_FindDegenerates;
//	}
//	if (postProcessOptions.bProcess_FindInvalidData)
//	{
//		assimpPostProcessingFlags |= aiProcess_FindInvalidData;
//	}
//	if (postProcessOptions.bProcess_GenUVCoords)
//	{
//		assimpPostProcessingFlags |= aiProcess_GenUVCoords;
//	}
//	if (postProcessOptions.bProcess_TransformUVCoords)
//	{
//		assimpPostProcessingFlags |= aiProcess_TransformUVCoords;
//	}
//	if (postProcessOptions.bProcess_FindInstances)
//	{
//		assimpPostProcessingFlags |= aiProcess_FindInstances;
//	}
//	if (postProcessOptions.bProcess_OptimizeMeshes)
//	{
//		assimpPostProcessingFlags |= aiProcess_OptimizeMeshes;
//	}
//	if (postProcessOptions.bProcess_OptimizeGraph)
//	{
//		assimpPostProcessingFlags |= aiProcess_OptimizeGraph;
//	}
//	if (postProcessOptions.bProcess_FlipUVs)
//	{
//		assimpPostProcessingFlags |= aiProcess_FlipUVs;
//	}
//	if (postProcessOptions.bProcess_FlipWindingOrder)
//	{
//		assimpPostProcessingFlags |= aiProcess_FlipWindingOrder;
//	}
//	if (postProcessOptions.bProcess_SplitByBoneCount)
//	{
//		assimpPostProcessingFlags |= aiProcess_SplitByBoneCount;
//	}
//	if (postProcessOptions.bProcess_Debone)
//	{
//		assimpPostProcessingFlags |= aiProcess_Debone;
//	}
//
//	return assimpPostProcessingFlags;
//}
//
//
//std::string cFileLoader_Imp::getLastError(bool bAndClearErrors /*=true*/)
//{
//	std::string error = this->m_LastError;
//
//	if (bAndClearErrors)
//	{
//		this->m_LastError = "";
//	}
//	return error;
//}
//
//void cFileLoader_Imp::m_AppendErrorString(std::string errorString)
//{
//	std::stringstream ssError;
//	if (!this->m_LastError.empty())
//	{
//		ssError << this->m_LastError;
//		ssError << std::endl;
//	}
//	ssError << errorString;
//	return;
//}
//
//
//void cFileLoader_Imp::SetBasePath(std::string basepath_no_end_slash)
//{
//	this->m_basePath_no_end_slash = basepath_no_end_slash;
//	return;
//}