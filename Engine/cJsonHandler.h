/**
 * Project:      ReadAndWriteConfiguration
 * File:         cJsonConfigurationReaderWriter.h
 * Author:       Lucas Berg
 * Date Created: November 2nd, 2023
 * Description:  Declares methods to handle reading and writing of json files
 */
#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#pragma warning(push)
#pragma warning(disable: 26495) // 26495 is the warning code for not initializing data members
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#pragma warning(pop)

struct sTextureInfo
{
	std::string Name;
	float Ratio = 0.0f;
};

struct sModel
{
	std::string MeshName;
	glm::vec3 Position = glm::vec3(0.0f);
	std::string FriendlyName;
	std::string ObjectType;
	bool IsUsingDebugColors = false;
	bool IsUsingVertexColors = false;
	bool IsUsingBones = false;
	glm::quat Orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 DrawScale = glm::vec3(1.0f);;
	glm::vec4 DebugColourRGBA = glm::vec4(0.0f);
	float Alpha = 1.0f;
	std::vector<sTextureInfo> ObjectTextures;
};

struct sCameraInfo
{
	std::string CameraType;
	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Target = glm::vec3(0.0f);
	glm::quat Orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 Forward = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);;
	float NearClip = 0.0f;
	float FarClip = 0.0f;
};

struct sPhysicsInfo 
{
	std::string physicsShape;
};

struct sLightsInfo
{
	std::string FriendlyName;
	glm::vec4 Position = glm::vec4(0.0f);
	glm::vec4 Diffuse = glm::vec4(0.0f);
	glm::vec4 Specular = glm::vec4(0.0f);
	glm::vec4 Atten = glm::vec4(0.0f);
	glm::vec4 Direction = glm::vec4(0.0f);
	glm::vec4 Param1 = glm::vec4(0.0f);
	glm::vec4 Param2 = glm::vec4(0.0f);
};

struct sSceneInfo
{
	std::string SceneName;
	std::vector<sCameraInfo> CameraInfo;
	std::vector<sModel> ModelInfo;
	std::vector<sPhysicsInfo> PhysicsInfo;
	std::vector<sLightsInfo> LightsInfo;
};

class cJsonHandler
{
	bool DoesFileExist(std::string filePath);

	bool SetValue(const rapidjson::Value& jsonValue, bool& boolOut);
	bool SetValue(const rapidjson::Value& jsonValue, int& stringOut);
	bool SetValue(const rapidjson::Value& jsonValue, float& floatOut);
	bool SetValue(const rapidjson::Value& jsonValue, std::string& stringOut);
	bool SetValue(const rapidjson::Value& jsonValue, std::vector<std::string>& vectorOut);
	bool SetValue(const rapidjson::Value& jsonValue, glm::vec3& vectorOut);
	bool SetValue(const rapidjson::Value& jsonValue, glm::vec4& vectorOut);
	bool SetValue(const rapidjson::Value& jsonValue, glm::quat& vectorOut);
	bool SetValue(const rapidjson::Value& jsonValue, std::vector <sModel>& sceneModelsOut);
	bool SetValue(const rapidjson::Value& jsonValue, std::vector <sCameraInfo>& sceneCamerasOut);
	bool SetValue(const rapidjson::Value& jsonValue, std::vector <sTextureInfo>& textureInfoOut);

	bool SerializeModel(rapidjson::Value& jsonValue, sModel& sModelOut, rapidjson::Document::AllocatorType& allocator);
	bool SerializeCamera(rapidjson::Value& jsonValue, sCameraInfo& sModelOut, rapidjson::Document::AllocatorType& allocator);
	bool SerializeLight(rapidjson::Value& jsonValue, sLightsInfo& sModelOut, rapidjson::Document::AllocatorType& allocator);

	bool ParseCameraInfo(const rapidjson::Value& configValue, std::vector<sCameraInfo>& configOut);
	bool ParseModelsWithTextureInfo(const rapidjson::Value& preferenceValue, std::vector < std::string>& modelsInfoOut);
	bool ParsePhysicsInfo(const rapidjson::Value& languageValue, sPhysicsInfo& languageOut);


	bool ParseTextureInfo(const rapidjson::Value& preferenceValue, std::vector < std::string>& texturesOut);
	bool ParseSceneModels(const rapidjson::Value& preferenceValue, std::vector < sModel>& sceneModelsOut);
	bool ParseSceneLights(const rapidjson::Value& preferenceValue, std::vector < sLightsInfo>& sceneLightsOut);

	bool ParseScene(const rapidjson::Value& preferenceValue, sSceneInfo& sceneLightsOut);

public:

	std::string sceneName = "PatternsProject2";
	std::string cameraInfoFileName = "docs/CameraInfo.json";
	std::string modelsWithTextureFileName = "docs/PlyModelsToLoadWithTexture.json";
	std::string modelsFileName = "docs/PlyModelsToLoad.json";
	std::string fbxModelsFileName = "docs/AssimpModels.json";
	std::string TexturesFileName = "docs/Textures.json";
	std::string SceneFileName = "docs/scenes/" + sceneName + ".json";
	//std::string SceneLightsName = "docs/SceneLights.json";

	~cJsonHandler();

	bool LoadJsonInfo(std::vector<sCameraInfo>& cameraInfo, 
		std::vector<std::string>& modelInfoWithTexture,
		std::vector<std::string>& plyModelsInfo,
		std::vector<std::string>& fbxModelsInfo,
		std::vector<std::string>& texturesInfo,
		std::vector<sModel>& sceneModelsInfo,
		std::vector<sLightsInfo>& sceneLightsInfo);

	bool LoadJsonInfo(sSceneInfo& sceneInfo,
		std::vector<std::string>& modelInfoWithTexture,
		std::vector<std::string>& plyModelsInfo,
		std::vector<std::string>& fbxModelsInfo,
		std::vector<std::string>& texturesInfo
	);

	bool ReadFBXModelsConfig(const std::string& filePath, std::vector <std::string>& fbxModelsOut);
	bool ReadModelsWithTextureConfig(const std::string& filePath, std::vector <std::string>& modelOut);
	bool ReadTexturesConfig(const std::string& filePath, std::vector <std::string>& modelOut);

	bool ReadModelsConfig(const std::string& filePath, std::vector <sModel>& modelsOut);
	bool ReadCameraConfig(const std::string& filePath, std::vector<sCameraInfo>& cameraInfoOut);
	bool ReadSceneLightsConfig(const std::string& filePath, std::vector <sLightsInfo>& lightsOut);
	bool ReadSceneConfig(const std::string& filePath, sSceneInfo& sceneOut);

	bool WriteCameraConfig(const std::string& filePath, std::vector<sCameraInfo>& cameraInfoIn);
	bool WriteModelsWithTextureConfig(const std::string& filePath, std::vector <std::string>& modelsIn);
	bool WriteTexturesConfig(const std::string& filePath, std::vector <std::string>& texturesIn);
	bool WriteSceneConfig(const std::string& filePath, std::vector <sModel>& sceneModelsIn);
	bool WriteLightsConfig(const std::string& filePath, std::vector <sLightsInfo>& sceneModelsIn);

	bool CreateScene(std::string sceneName);

	bool SaveScene(sCameraInfo& cameraInfoIn,
		std::vector <sModel>& sceneModelsIn,
		std::vector <sLightsInfo>& lightsInt);

	bool SaveScene(sSceneInfo scene);

	// TODO: New way to load scene
	//bool LoadScene(sSceneInfo& scene);

	bool WriteConfig(const std::string& filePath,
		const sCameraInfo& configOut,
		const sModel& modelsOut,
		const sPhysicsInfo& languagesOut);

	bool DoesSceneExist(std::string fileName);

	bool SaveJsonToFile(std::string filePath, std::string jsonString);
};