#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <iterator>
#include <sstream>

#include "cMesh.h"
#include "cLightManager.h"
#include "cPhysics.h"
#include "cScene.h"

extern cScene* g_currentScene;

bool SaveVectorSceneToFile(std::string saveFileName, std::string lightFileName, std::string physicsFileName)
{
	std::vector<cMesh*> meshesToDraw = g_currentScene->GetAllMeshes();

	std::ofstream theSameFile(saveFileName.c_str());

	theSameFile << meshesToDraw.size() << std::endl;

	for (unsigned int index = 0; index != meshesToDraw.size(); index++)
	{
		glm::quat orientation = meshesToDraw[index]->GetOrientation();

		int numberOfTextures = 0;
		int numberOfPossibleTextures = sizeof(meshesToDraw[index]->textureName) / sizeof(meshesToDraw[index]->textureName[0]);;
		for (int i = 0; i < numberOfPossibleTextures; i++)
		{
			if (!(meshesToDraw[index]->textureName[i].empty()))
			{
				++numberOfTextures;
			}
		}

		theSameFile << meshesToDraw[index]->meshName << " "
			<< meshesToDraw[index]->drawPosition.x << " "
			<< meshesToDraw[index]->drawPosition.y << " "
			<< meshesToDraw[index]->drawPosition.z << " "
			<< meshesToDraw[index]->friendlyName << " "
			<< meshesToDraw[index]->objectType << " "
			<< meshesToDraw[index]->bUseDebugColours << " "
			<< orientation.w << " "
			<< orientation.x << " "
			<< orientation.y << " "
			<< orientation.z << " "
			<< meshesToDraw[index]->drawScale.x << " "
			<< meshesToDraw[index]->drawScale.y << " "
			<< meshesToDraw[index]->drawScale.z << " "
			<< meshesToDraw[index]->wholeObjectDebugColourRGBA.r << " "
			<< meshesToDraw[index]->wholeObjectDebugColourRGBA.g << " "
			<< meshesToDraw[index]->wholeObjectDebugColourRGBA.b << " "
			<< meshesToDraw[index]->wholeObjectDebugColourRGBA.a << " ";

		if (numberOfTextures > 0)
		{
			theSameFile << "true" << " "
			 << numberOfTextures << " ";

			for (int i = 0; i < numberOfTextures; i++)
			{
				theSameFile << meshesToDraw[index]->textureName[i] << " "
							<< meshesToDraw[index]->textureRatios[i] << " ";
			}
		}
		else
		{
			theSameFile << "false";
		}

		theSameFile << std::endl;
	}

	std::cout << "Scene saved in " << saveFileName << std::endl;

	// save light info
	//std::ofstream lightsFile(lightFileName);

	//lightsFile << ::g_pTheLights->NUMBER_OF_LIGHTS_IM_USING << std::endl;

	//for (unsigned int index = 0; index != ::g_pTheLights->NUMBER_OF_LIGHTS_IM_USING; index++)
	//{
	//	std::string friendlyName = ::g_pTheLights->theLights[index].lightFriendlyName;

	//	if (friendlyName.empty())
	//	{
	//		friendlyName = "notnamed";
	//	}

	//	lightsFile << friendlyName << " "
	//		<< ::g_pTheLights->theLights[index].position.x << " "
	//		<< ::g_pTheLights->theLights[index].position.y << " "
	//		<< ::g_pTheLights->theLights[index].position.z << " "
	//		<< ::g_pTheLights->theLights[index].diffuse.x << " "
	//		<< ::g_pTheLights->theLights[index].diffuse.y << " "
	//		<< ::g_pTheLights->theLights[index].diffuse.z << " "
	//		<< ::g_pTheLights->theLights[index].diffuse.a << " "
	//		<< ::g_pTheLights->theLights[index].atten.x << " "
	//		<< ::g_pTheLights->theLights[index].atten.y << " "
	//		<< ::g_pTheLights->theLights[index].atten.z << " "
	//		<< ::g_pTheLights->theLights[index].param1.x << " "
	//		<< ::g_pTheLights->theLights[index].param1.y << " "
	//		<< ::g_pTheLights->theLights[index].param1.z << " "
	//		<< ::g_pTheLights->theLights[index].direction.x << " "
	//		<< ::g_pTheLights->theLights[index].direction.y << " "
	//		<< ::g_pTheLights->theLights[index].direction.z << " "
	//		<< ::g_pTheLights->theLights[index].param2.x << " "
	//		<< std::endl;
	//}

	//std::cout << "Scene lights saved in " << lightFileName << std::endl;

	//// save physics info
	//std::ofstream physicsFile(physicsFileName);

	//unsigned int numOfPhysicsShapes = (unsigned int)g_currentScene->m_scenePhysics->m_vec_pPhysicalProps.size();

	//physicsFile << numOfPhysicsShapes << std::endl;

	//for (unsigned int index = 0; index != numOfPhysicsShapes; index++)
	//{
	//	sPhysicsProperties* physicsObject = g_currentScene->m_scenePhysics->m_vec_pPhysicalProps[index];
	//	sPhysicsProperties::eShape shape = physicsObject->shapeType;
	//	std::string physicsShape = physicsObject->getShapeTypeAsString();


	//	switch (shape)
	//	{
	//		case sPhysicsProperties::eShape::UNKNOWN_OR_UNDEFINED:
	//			break;
	//		case sPhysicsProperties::eShape::SPHERE:
	//			physicsFile << physicsShape << " "
	//				<< physicsObject->pTheAssociatedMesh->GetFriendlyName() << " "
	//				<< ((sPhysicsProperties::sSphere*)(physicsObject->pShape))->radius << " "
	//				<< std::endl;
	//			break;
	//		case sPhysicsProperties::eShape::PLANE:
	//			physicsFile << physicsShape << " "
	//				<< physicsObject->pTheAssociatedMesh->GetFriendlyName() 
	//				<< std::endl;
	//			break;
	//		case sPhysicsProperties::eShape::TRIANGLE :
	//			physicsFile << physicsShape << " "
	//				<< physicsObject->pTheAssociatedMesh->GetFriendlyName() 
	//				<< std::endl;
	//			break;
	//		case sPhysicsProperties::eShape::AABB :
	//			physicsFile << physicsShape << " "
	//				<< physicsObject->pTheAssociatedMesh->GetFriendlyName() 
	//				<< std::endl;
	//			break;
	//		case sPhysicsProperties::eShape::CAPSULE :
	//			physicsFile << physicsShape << " "
	//				<< physicsObject->pTheAssociatedMesh->GetFriendlyName()
	//				<< std::endl;
	//			break;
	//		case sPhysicsProperties::eShape::MESH_OF_TRIANGLES_INDIRECT:
	//			physicsFile << physicsShape << " "
	//				<< physicsObject->pTheAssociatedMesh->GetFriendlyName() << " "
	//				<< ((sPhysicsProperties::sMeshOfTriangles_Indirect*)(physicsObject->pShape))->meshName << " "
	//				<< std::endl;
	//			break;
	//		case sPhysicsProperties::eShape::MESH_OF_TRIANGLES_LOCAL_VERTICES:
	//			physicsFile << physicsShape << " "
	//				<< physicsObject->pTheAssociatedMesh->GetFriendlyName()
	//				<< std::endl;
	//			break;
	//	}
	//}

	//std::cout << "Scene physics saved in " << physicsFileName << std::endl;

	return true;
}

bool LoadVectorSceneFromFile(std::string loadFileName, std::string lightFileName, std::string physicsFileName)
{
	//// lights info
	//std::ifstream lightsFile(lightFileName);

	//std::string lightsLine;
	//unsigned int lightIndex = 0;
	//while (getline(lightsFile, lightsLine))
	//{
	//	std::istringstream lightsLineStream(lightsLine);
	//	lightsLineStream >> ::g_pTheLights->theLights[lightIndex].lightFriendlyName
	//		>> ::g_pTheLights->theLights[lightIndex].position.x
	//		>> ::g_pTheLights->theLights[lightIndex].position.y
	//		>> ::g_pTheLights->theLights[lightIndex].position.z
	//		>> ::g_pTheLights->theLights[lightIndex].diffuse.x
	//		>> ::g_pTheLights->theLights[lightIndex].diffuse.y
	//		>> ::g_pTheLights->theLights[lightIndex].diffuse.z
	//		>> ::g_pTheLights->theLights[lightIndex].diffuse.a
	//		>> ::g_pTheLights->theLights[lightIndex].atten.x
	//		>> ::g_pTheLights->theLights[lightIndex].atten.y
	//		>> ::g_pTheLights->theLights[lightIndex].atten.z
	//		>> ::g_pTheLights->theLights[lightIndex].param1.x
	//		>> ::g_pTheLights->theLights[lightIndex].param1.y
	//		>> ::g_pTheLights->theLights[lightIndex].param1.z
	//		>> ::g_pTheLights->theLights[lightIndex].direction.x
	//		>> ::g_pTheLights->theLights[lightIndex].direction.y
	//		>> ::g_pTheLights->theLights[lightIndex].direction.z
	//		>> ::g_pTheLights->theLights[lightIndex].param2.x;

	//	++lightIndex;
	//}

	//lightsFile.close();
	//std::cout << "Scene lights loaded from " << lightFileName << std::endl;

	//physics info
	//std::ifstream physicsFile(physicsFileName);

	//std::string physicsLine;
	//while (getline(physicsFile, physicsLine))
	//{
	//	std::istringstream lightsLineStream(physicsFile);

	//}

	//physicsFile.close();
	//std::cout << "Scene physics loaded from " << physicsFileName << std::endl;

	return true;
}