#include "cJsonHandler.h"

#include <glm/vec3.hpp>

//cJsonConfigurationReaderWriter::cJsonConfigurationReaderWriter(std::string cameraFile, 
//	std::string texturesFile, std::string plyModelsWithTextureFile, std::string sceneModelsFile)
//{
//	m_cameraFile = cameraFile;
//	m_texturesFile = texturesFile;
//	m_plyModelsWithTextureFile = plyModelsWithTextureFile;
//	m_sceneModelsFile = sceneModelsFile;
//}

cJsonHandler::~cJsonHandler()
{

}

bool cJsonHandler::LoadJsonInfo(
	std::vector<sCameraInfo>& cameraInfo, 
	std::vector<std::string>& modelInfoWithTexture, 
	std::vector<std::string>& plyModelsInfo, 
	std::vector<std::string>& fbxModelsInfo,
	std::vector<std::string>& texturesInfo, 
	std::vector<sModel>& sceneModelsInfo, 
	std::vector<sLightsInfo>& sceneLightsInfo
)
{
	if (!(ReadCameraConfig(cameraInfoFileName, cameraInfo)))
	{
		std::cout << "config file " << cameraInfoFileName << " failed" << std::endl;
		return false;
	}

	if (!(ReadModelsWithTextureConfig(modelsWithTextureFileName, modelInfoWithTexture)))
	{
		std::cout << "config file " << modelsWithTextureFileName << " failed" << std::endl;
		return false;
	}

	if (!(ReadModelsWithTextureConfig(modelsFileName, plyModelsInfo)))
	{
		std::cout << "config file " << modelsFileName << " failed" << std::endl;
		return false;
	}

	if (!(ReadModelsWithTextureConfig(fbxModelsFileName, fbxModelsInfo)))
	{
		std::cout << "config file " << fbxModelsFileName << " failed" << std::endl;
		return false;
	}

	if (!(ReadTexturesConfig(TexturesFileName, texturesInfo)))
	{
		std::cout << "config file " << TexturesFileName << " failed" << std::endl;
		return false;
	}

	if (!(ReadModelsConfig(SceneFileName, sceneModelsInfo)))
	{
		std::cout << "config file " << SceneFileName << " failed" << std::endl;
		return false;
	}

	//if (!(ReadSceneLightsConfig(SceneLightsName, sceneLightsInfo)))
	//{
	//	std::cout << "config file " << SceneLightsName << " failed" << std::endl;
	//	return false;
	//}

	return true;
}

bool cJsonHandler::LoadJsonInfo(sSceneInfo& sceneInfo,
	std::vector<std::string>& modelInfoWithTexture,
	std::vector<std::string>& plyModelsInfo,
	std::vector<std::string>& fbxModelsInfo,
	std::vector<std::string>& texturesInfo)
{
	std::string errorString = "";

	if (!(ReadSceneConfig(SceneFileName, sceneInfo)))
	{
		errorString += "config file " + SceneFileName + " failed\n";
	}

	if (!(ReadModelsWithTextureConfig(modelsWithTextureFileName, modelInfoWithTexture)))
	{
		errorString += "config file " + modelsWithTextureFileName + " failed\n";
	}

	if (!(ReadModelsWithTextureConfig(modelsFileName, plyModelsInfo)))
	{
		errorString += "config file " + modelsFileName + " failed\n";
	}

	if (!(ReadModelsWithTextureConfig(fbxModelsFileName, fbxModelsInfo)))
	{
		errorString += "config file " + fbxModelsFileName + " failed\n";
	}

	if (!(ReadTexturesConfig(TexturesFileName, texturesInfo)))
	{
		errorString += "config file " + TexturesFileName + " failed\n";
	}

	if (errorString != "")
	{
		std::cout << errorString;
		return false;
	}

	return true;
}

bool cJsonHandler::ReadCameraConfig(const std::string& filePath,std::vector<sCameraInfo>& cameraInfoOut)
{
	using namespace rapidjson;

	// camera info
	FILE* cameraFile = 0;
	fopen_s(&cameraFile, filePath.c_str(), "rb");

	Document cameraInfo;
	char readBuffer1[3000] = {};
	FileReadStream is(cameraFile, readBuffer1, sizeof(readBuffer1));
	cameraInfo.ParseStream(is);

	ParseCameraInfo(cameraInfo, cameraInfoOut);

	if (cameraFile != 0)
	{
		fclose(cameraFile);
	}

	return true;
}

bool cJsonHandler::ReadFBXModelsConfig(const std::string& filePath, std::vector<std::string>& fbxModelsOut)
{
	using namespace rapidjson;

	// model info
	FILE* modelsFile = 0;
	fopen_s(&modelsFile, filePath.c_str(), "rb");

	Document fbxModelsInfo;
	char readBuffer1[3000] = {}; // buffer size
	FileReadStream is(modelsFile, readBuffer1, sizeof(readBuffer1));
	fbxModelsInfo.ParseStream(is);

	ParseModelsWithTextureInfo(fbxModelsInfo, fbxModelsOut);

	if (modelsFile != 0)
	{
		fclose(modelsFile);
	}

	return true;
}

bool cJsonHandler::ReadModelsWithTextureConfig(const std::string& filePath, std::vector <std::string> & modelOut)
{
	using namespace rapidjson;

	// model info
	FILE* modelsFile = 0;
	fopen_s(&modelsFile, filePath.c_str(), "rb");

	Document cameraInfo;
	char readBuffer1[3000] = {}; // buffer size
	FileReadStream is(modelsFile, readBuffer1, sizeof(readBuffer1));
	cameraInfo.ParseStream(is);

	ParseModelsWithTextureInfo(cameraInfo, modelOut);

	if (modelsFile != 0)
	{
		fclose(modelsFile);
	}

	return true;
}

bool cJsonHandler::ReadTexturesConfig(const std::string& filePath, std::vector<std::string>& modelOut)
{
	using namespace rapidjson;

	// model info
	FILE* texturesFile = 0;
	fopen_s(&texturesFile, filePath.c_str(), "rb");

	Document cameraInfo;
	char readBuffer1[3000] = {}; // buffer size
	FileReadStream is(texturesFile, readBuffer1, sizeof(readBuffer1));
	cameraInfo.ParseStream(is);

	ParseTextureInfo(cameraInfo, modelOut);

	if (texturesFile != 0)
	{
		fclose(texturesFile);
	}

	return true;
}

bool cJsonHandler::ReadModelsConfig(const std::string& filePath, std::vector<sModel>& modelsOut)
{
	using namespace rapidjson;

	// model info
	FILE* sceneFile = 0;
	fopen_s(&sceneFile, filePath.c_str(), "rb");

	Document sceneInfo;
	char readBuffer1[3000] = {}; // buffer size
	FileReadStream is(sceneFile, readBuffer1, sizeof(readBuffer1));
	sceneInfo.ParseStream(is);

	if (!ParseSceneModels(sceneInfo, modelsOut))
	{
		std::cout << "Scene models failed to load" << std::endl;
	}

	if (sceneFile != 0)
	{
		fclose(sceneFile);
	}

	return true;
}

bool cJsonHandler::ReadSceneLightsConfig(const std::string& filePath, std::vector<sLightsInfo>& lightsOut)
{
	using namespace rapidjson;

	// model info
	FILE* sceneFile = 0;
	fopen_s(&sceneFile, filePath.c_str(), "rb");

	Document sceneInfo;
	char readBuffer1[3000] = {}; // buffer size
	FileReadStream is(sceneFile, readBuffer1, sizeof(readBuffer1));
	sceneInfo.ParseStream(is);

	ParseSceneLights(sceneInfo, lightsOut);

	if (sceneFile != 0)
	{
		fclose(sceneFile);
	}

	return true;
}

bool cJsonHandler::ReadSceneConfig(const std::string& filePath, sSceneInfo& sceneOut)
{
	using namespace rapidjson;

	// model info
	FILE* sceneFile = 0;
	fopen_s(&sceneFile, filePath.c_str(), "rb");

	Document sceneInfo;
	char readBuffer1[3000] = {}; // buffer size
	FileReadStream is(sceneFile, readBuffer1, sizeof(readBuffer1));
	sceneInfo.ParseStream(is);


	if (!ParseScene(sceneInfo, sceneOut))
	{
		std::cout << "Scene failed to load" << std::endl;
	}

	if (sceneFile != 0)
	{
		fclose(sceneFile);
	}

	return true;
}

bool cJsonHandler::WriteCameraConfig(const std::string& filePath, std::vector<sCameraInfo>& cameraInfoIn)
{
	using namespace rapidjson;

	// config
	FILE* cameraConfigFile = 0;
	fopen_s(&cameraConfigFile, filePath.c_str(), "rb");

	Document cameraConfig(kArrayType);
	cameraConfig.SetArray();
	Document::AllocatorType& allocator = cameraConfig.GetAllocator();

	for (unsigned int i = 0; i < cameraInfoIn.size(); i++)
	{
		Value cameraInfo(kObjectType);
		cameraInfo.SetObject();

		Value position(kArrayType);
		position.SetArray();
		position.PushBack(cameraInfoIn[i].Position.x, allocator);
		position.PushBack(cameraInfoIn[i].Position.y, allocator);
		position.PushBack(cameraInfoIn[i].Position.z, allocator);

		Value target(kArrayType);
		target.SetArray();
		target.PushBack(cameraInfoIn[i].Target.x, allocator);
		target.PushBack(cameraInfoIn[i].Target.y, allocator);
		target.PushBack(cameraInfoIn[i].Target.z, allocator);

		Value orientation(kArrayType);
		orientation.SetArray();
		orientation.PushBack(cameraInfoIn[i].Orientation.w, allocator);
		orientation.PushBack(cameraInfoIn[i].Orientation.x, allocator);
		orientation.PushBack(cameraInfoIn[i].Orientation.y, allocator);
		orientation.PushBack(cameraInfoIn[i].Orientation.z, allocator);

		Value forward(kArrayType);
		forward.SetArray();
		forward.PushBack(cameraInfoIn[i].Forward.x, allocator);
		forward.PushBack(cameraInfoIn[i].Forward.y, allocator);
		forward.PushBack(cameraInfoIn[i].Forward.z, allocator);

		Value up(kArrayType);
		up.SetArray();
		up.PushBack(cameraInfoIn[i].Up.x, allocator);
		up.PushBack(cameraInfoIn[i].Up.y, allocator);
		up.PushBack(cameraInfoIn[i].Up.z, allocator);

		cameraInfo.AddMember("CameraType", Value(cameraInfoIn[i].CameraType.c_str(), allocator), allocator);
		cameraInfo.AddMember("Position", Value(position, allocator), allocator);
		cameraInfo.AddMember("Target", Value(target, allocator), allocator);
		cameraInfo.AddMember("Orientation", Value(orientation, allocator), allocator);
		cameraInfo.AddMember("Forward", Value(forward, allocator), allocator);
		cameraInfo.AddMember("Up", Value(up, allocator), allocator);
		cameraInfo.AddMember("NearClip", cameraInfoIn[i].NearClip, allocator);
		cameraInfo.AddMember("FarClip", cameraInfoIn[i].FarClip, allocator);

		cameraConfig.PushBack(cameraInfo, allocator); 
	}


	char writeBuffer1[3000] = {};
	FileWriteStream os1(cameraConfigFile, writeBuffer1, sizeof(writeBuffer1));

	StringBuffer writeBuffer2;
	Writer<StringBuffer> writer1(writeBuffer2);
	cameraConfig.Accept(writer1);

	std::string jsonString = writeBuffer2.GetString();

	if (cameraConfigFile != NULL)
	{
		fclose(cameraConfigFile);
	}

	std::ofstream outputFile(filePath.c_str());
	if (outputFile.is_open()) 
	{
		outputFile << jsonString;
		outputFile.close();
	}
	else 
	{
		std::cerr << "Failed to open the output file " << filePath.c_str() << " for writing." << std::endl;
	}

	return true;
}

bool cJsonHandler::WriteModelsWithTextureConfig(const std::string& filePath, std::vector<std::string>& modelsIn)
{
	return false;
}

bool cJsonHandler::WriteTexturesConfig(const std::string& filePath, std::vector<std::string>& texturesIn)
{
	return false;
}

bool cJsonHandler::WriteSceneConfig(const std::string& filePath, std::vector<sModel>& sceneModelsIn)
{
	using namespace rapidjson;

	// config
	FILE* sceneModelsFile = 0;
	fopen_s(&sceneModelsFile, filePath.c_str(), "rb");

	Document modelsConfig(kArrayType);
	modelsConfig.SetArray();

	Document::AllocatorType& allocator = modelsConfig.GetAllocator();

	for (unsigned int i = 0; i < sceneModelsIn.size(); i++)
	{
		Value model(kObjectType);
		model.SetObject();

		Value position(kArrayType);
		position.SetArray();
		position.PushBack(sceneModelsIn[i].Position.x, allocator);
		position.PushBack(sceneModelsIn[i].Position.y, allocator);
		position.PushBack(sceneModelsIn[i].Position.z, allocator);

		Value orientation(kArrayType);
		orientation.SetArray();
		orientation.PushBack(sceneModelsIn[i].Orientation.w, allocator);
		orientation.PushBack(sceneModelsIn[i].Orientation.x, allocator);
		orientation.PushBack(sceneModelsIn[i].Orientation.y, allocator);
		orientation.PushBack(sceneModelsIn[i].Orientation.z, allocator);

		Value scale(kArrayType);
		scale.SetArray();
		scale.PushBack(sceneModelsIn[i].DrawScale.x, allocator);
		scale.PushBack(sceneModelsIn[i].DrawScale.y, allocator);
		scale.PushBack(sceneModelsIn[i].DrawScale.z, allocator);

		Value colorRGBA(kArrayType);
		colorRGBA.SetArray();
		colorRGBA.PushBack(sceneModelsIn[i].DebugColourRGBA.x, allocator);
		colorRGBA.PushBack(sceneModelsIn[i].DebugColourRGBA.y, allocator);
		colorRGBA.PushBack(sceneModelsIn[i].DebugColourRGBA.z, allocator);
		colorRGBA.PushBack(sceneModelsIn[i].DebugColourRGBA.w, allocator);

		Value textures(kArrayType);
		textures.SetArray();
		for (unsigned int j = 0; j < sceneModelsIn[i].ObjectTextures.size(); j++)
		{
			Value texture(kObjectType);
			texture.SetObject();

			texture.AddMember("Name", Value(sceneModelsIn[i].ObjectTextures[j].Name.c_str(), allocator), allocator);
			texture.AddMember("Ratio", sceneModelsIn[i].ObjectTextures[j].Ratio, allocator);

			textures.PushBack(texture, modelsConfig.GetAllocator());
		}


		model.AddMember("MeshName", Value(sceneModelsIn[i].MeshName.c_str(), allocator), allocator);
		model.AddMember("Position", Value(position, allocator), allocator);
		model.AddMember("FriendlyName", Value(sceneModelsIn[i].FriendlyName.c_str(), allocator), allocator);
		model.AddMember("ObjectType", Value(sceneModelsIn[i].ObjectType.c_str(), allocator), allocator);
		model.AddMember("IsUsingDebugColors", sceneModelsIn[i].IsUsingDebugColors, allocator);
		model.AddMember("IsUsingVertexColors", sceneModelsIn[i].IsUsingVertexColors, allocator);
		model.AddMember("IsUsingBones", sceneModelsIn[i].IsUsingBones, allocator);
		model.AddMember("Orientation", Value(orientation, allocator), allocator);
		model.AddMember("DrawScale", Value(scale, allocator), allocator);
		model.AddMember("DebugColourRGBA", Value(colorRGBA,  allocator), allocator);
		model.AddMember("Alpha", sceneModelsIn[i].Alpha, allocator);
		model.AddMember("ObjectTextures", Value(textures, allocator), allocator);


		modelsConfig.PushBack(model, modelsConfig.GetAllocator());
	}

	char writeBuffer1[7000] = {};
	FileWriteStream os1(sceneModelsFile, writeBuffer1, sizeof(writeBuffer1));

	StringBuffer writeBuffer2;
	Writer<StringBuffer> writer1(writeBuffer2);
	modelsConfig.Accept(writer1);

	std::string jsonString = writeBuffer2.GetString();

	if (sceneModelsFile != NULL)
	{
		fclose(sceneModelsFile);
	}

	std::ofstream outputFile(filePath.c_str());
	if (outputFile.is_open())
	{
		outputFile << jsonString;
		outputFile.close();
	}
	else
	{
		std::cerr << "Failed to open the output file " << filePath.c_str() << " for writing." << std::endl;
	}

	return true;
}

bool cJsonHandler::WriteLightsConfig(const std::string& filePath, std::vector<sLightsInfo>& sceneLightsIn)
{
	using namespace rapidjson;

	FILE* sceneLightsFile = 0;
	fopen_s(&sceneLightsFile, filePath.c_str(), "rb");

	Document lightsConfig(kArrayType);
	lightsConfig.SetArray();

	Document::AllocatorType& allocator = lightsConfig.GetAllocator();

	for (unsigned int i = 0; i < sceneLightsIn.size(); i++)
	{
		Value light(kObjectType);
		light.SetObject();

		Value position(kArrayType);
		position.SetArray();
		position.PushBack(sceneLightsIn[i].Position.x, allocator);
		position.PushBack(sceneLightsIn[i].Position.y, allocator);
		position.PushBack(sceneLightsIn[i].Position.z, allocator);
		position.PushBack(sceneLightsIn[i].Position.w, allocator);

		Value diffuse(kArrayType);
		diffuse.SetArray();
		diffuse.PushBack(sceneLightsIn[i].Diffuse.x, allocator);
		diffuse.PushBack(sceneLightsIn[i].Diffuse.y, allocator);
		diffuse.PushBack(sceneLightsIn[i].Diffuse.z, allocator);
		diffuse.PushBack(sceneLightsIn[i].Diffuse.w, allocator);

		Value specular(kArrayType);
		specular.SetArray();
		specular.PushBack(sceneLightsIn[i].Specular.x, allocator);
		specular.PushBack(sceneLightsIn[i].Specular.y, allocator);
		specular.PushBack(sceneLightsIn[i].Specular.z, allocator);
		specular.PushBack(sceneLightsIn[i].Specular.w, allocator);

		Value atten(kArrayType);
		atten.SetArray();
		atten.PushBack(sceneLightsIn[i].Atten.x, allocator);
		atten.PushBack(sceneLightsIn[i].Atten.y, allocator);
		atten.PushBack(sceneLightsIn[i].Atten.z, allocator);
		atten.PushBack(sceneLightsIn[i].Atten.w, allocator);

		Value direction(kArrayType);
		direction.SetArray();
		direction.PushBack(sceneLightsIn[i].Direction.x, allocator);
		direction.PushBack(sceneLightsIn[i].Direction.y, allocator);
		direction.PushBack(sceneLightsIn[i].Direction.z, allocator);
		direction.PushBack(sceneLightsIn[i].Direction.w, allocator);

		Value param1(kArrayType);
		param1.SetArray();
		param1.PushBack(sceneLightsIn[i].Param1.x, allocator);
		param1.PushBack(sceneLightsIn[i].Param1.y, allocator);
		param1.PushBack(sceneLightsIn[i].Param1.z, allocator);
		param1.PushBack(sceneLightsIn[i].Param1.w, allocator);

		Value param2(kArrayType);
		param2.SetArray();
		param2.PushBack(sceneLightsIn[i].Param2.x, allocator);
		param2.PushBack(sceneLightsIn[i].Param2.y, allocator);
		param2.PushBack(sceneLightsIn[i].Param2.z, allocator);
		param2.PushBack(sceneLightsIn[i].Param2.w, allocator);


		light.AddMember("Position", Value(position, allocator), allocator);
		light.AddMember("Diffuse", Value(diffuse, allocator), allocator);
		light.AddMember("Specular", Value(specular, allocator), allocator);
		light.AddMember("Atten", Value(atten, allocator), allocator);
		light.AddMember("Direction", Value(direction, allocator), allocator);
		light.AddMember("Param1", Value(param1, allocator), allocator);
		light.AddMember("Param2", Value(param2, allocator), allocator);
		light.AddMember("FriendlyName", Value(sceneLightsIn[i].FriendlyName.c_str(), allocator), allocator);
		
		lightsConfig.PushBack(light, lightsConfig.GetAllocator());
	}

	char writeBuffer1[5000] = {};
	FileWriteStream os1(sceneLightsFile, writeBuffer1, sizeof(writeBuffer1));

	StringBuffer writeBuffer2;
	Writer<StringBuffer> writer1(writeBuffer2);
	lightsConfig.Accept(writer1);

	std::string jsonString = writeBuffer2.GetString();

	if (sceneLightsFile != NULL)
	{
		fclose(sceneLightsFile);
	}

	if (SaveJsonToFile(filePath, jsonString))
	{
		return true;
	}

	return false;
}

bool cJsonHandler::CreateScene(std::string sceneName)
{	
	using namespace rapidjson;

	std::string sceneFileName = "docs/scenes/" + sceneName + ".json";

	StringBuffer writeBuffer2;
	Writer<StringBuffer> writer1(writeBuffer2);

	Document sceneConfig(kObjectType);
	sceneConfig.SetObject();

	Document::AllocatorType& allocator = sceneConfig.GetAllocator();

	//New Scene
	sModel mainCamera;
	mainCamera.MeshName = "shapes/Sphere_1_unit_Radius_xyz_n_rgba_uv.ply";
	mainCamera.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	mainCamera.FriendlyName = "maincamera";
	mainCamera.ObjectType = "camera";
	mainCamera.IsUsingDebugColors = true;
	mainCamera.IsUsingVertexColors = false;
	mainCamera.IsUsingBones = false;
	mainCamera.Orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	mainCamera.DrawScale = glm::vec3(1.0f, 1.0f, 1.0f);
	mainCamera.DebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	mainCamera.Alpha = 1.0f;

	std::vector<sModel> newSceneModels;
	newSceneModels.push_back(mainCamera);

	WriteSceneConfig(sceneFileName.c_str(), newSceneModels);

	//sceneConfig.AddMember("SceneName", Value(sceneName.c_str(), allocator), allocator);

	//sceneConfig.Accept(writer1);

	//std::string jsonString = writeBuffer2.GetString();

	//std::ofstream outputFile(sceneFileName.c_str());
	//if (outputFile.is_open())
	//{
	//	outputFile << jsonString;
	//	outputFile.close();
	//	return true;
	//}

	return false;
}

bool cJsonHandler::SaveScene(sCameraInfo& cameraInfoIn, std::vector<sModel>& modelsIn, std::vector<sLightsInfo>& lightsIn)
{
	bool result = true;
	//if (!WriteCameraConfig("docs/CameraInfo.json", cameraInfoIn))
	//{
	//	std::cout << "Camera info failed to read" << std::endl;
	//	result = false;
	//}

	//if (!WriteLightsConfig("docs/SceneLights.json", lightsIn))
	//{
	//	std::cout << "Camera info failed to read" << std::endl;
	//	result = false;
	//}

	return result;
}

// TODO: Complete this
bool cJsonHandler::SaveScene(sSceneInfo scene)
{
	using namespace rapidjson;

	scene.SceneName = sceneName;

	Document document(kObjectType);
	document.SetObject();

	Document::AllocatorType& allocator = document.GetAllocator();

	// Serialize Meshes
	Value models(kArrayType);
	models.SetArray();

	for (sModel model : scene.ModelInfo)
	{
		Value modelJson(kObjectType);
		modelJson.SetObject();

		SerializeModel(modelJson, model, allocator);

		models.PushBack(modelJson, allocator);
	}

	document.AddMember("meshes", models, allocator);
	
	// Serialize Cameras
	Value cameras(kArrayType);
	cameras.SetArray();

	for (sCameraInfo camera : scene.CameraInfo)
	{
		Value cameraJson(kObjectType);
		cameraJson.SetObject();

		SerializeCamera(cameraJson, camera, allocator);

		cameras.PushBack(cameraJson, allocator);
	}

	document.AddMember("cameras", cameras, allocator);

	// Serialize Lights
	Value lights(kArrayType);
	lights.SetArray();

	for (sLightsInfo light : scene.LightsInfo)
	{
		Value lightJson(kObjectType);
		lightJson.SetObject();

		SerializeLight(lightJson, light, allocator);

		lights.PushBack(lightJson, allocator);
	}

	document.AddMember("lights", lights, allocator);

	FILE* sceneModelsFile = 0;
	fopen_s(&sceneModelsFile, SceneFileName.c_str(), "rb");

	char writeBuffer1[7000] = {};
	FileWriteStream os1(sceneModelsFile, writeBuffer1, sizeof(writeBuffer1));

	StringBuffer writeBuffer2;
	Writer<StringBuffer> writer1(writeBuffer2);
	document.Accept(writer1);

	std::string jsonString = writeBuffer2.GetString();

	if (sceneModelsFile != NULL)
	{
		fclose(sceneModelsFile);
	}

	SaveJsonToFile(SceneFileName, jsonString);

	return true;
}

bool cJsonHandler::WriteConfig(const std::string& filePath, const sCameraInfo& configOut, const sModel& prefsOut, const sPhysicsInfo& languagesOut)
{
    return false;
}

bool cJsonHandler::DoesSceneExist(std::string fileName)
{
	std::string filePath = "docs/scenes/" + fileName + ".json";
	return DoesFileExist(filePath);
}

bool cJsonHandler::SaveJsonToFile(std::string filePath, std::string jsonString)
{
	std::ofstream outputFile(filePath.c_str());
	if (outputFile.is_open())
	{
		outputFile << jsonString;
		outputFile.close();
	}
	else
	{
		std::cerr << "Failed to open the output file " << filePath.c_str() << " for writing." << std::endl;
		return false;
	}

	return true;
}

bool cJsonHandler::DoesFileExist(std::string filePath)
{
	using namespace rapidjson;

	FILE* file = 0;
	errno_t err = fopen_s(&file, filePath.c_str(), "rb");

	if (err == 0) 
	{
		fclose(file);
		return true;
	}

	return true;
}

bool cJsonHandler::SetValue(const rapidjson::Value& jsonValue, bool& boolOut)
{
	if (!jsonValue.IsBool())
	{
		return false;
	}

	boolOut = jsonValue.GetBool();
	return true;
}

bool cJsonHandler::SetValue(const rapidjson::Value& jsonValue, int& stringOut)
{
	if (!jsonValue.IsInt())
	{
		return false;
	}

	stringOut = jsonValue.GetInt();
	return true;
}

bool cJsonHandler::SetValue(const rapidjson::Value& jsonValue, float& floatOut)
{
	if (!jsonValue.IsFloat())
	{
		return false;
	}

	floatOut = jsonValue.GetFloat();
	return true;
}

bool cJsonHandler::SetValue(const rapidjson::Value& jsonValue, std::string& stringOut)
{
	if (!jsonValue.IsString())
	{
		return false;
	}

	stringOut = jsonValue.GetString();
	return true;
}

bool cJsonHandler::SetValue(const rapidjson::Value& jsonValue, std::vector<std::string>& vectorOut)
{
	if (!jsonValue.IsArray())
	{
		return false;
	}

	for (unsigned int i = 0; i < jsonValue.Size(); i++)
	{
		vectorOut.push_back(jsonValue[i].GetString());
	}

	return true;
}

bool cJsonHandler::SetValue(const rapidjson::Value& jsonValue, glm::vec3& vectorOut)
{
	if (!jsonValue.IsArray())
	{
		return false;
	}

	float pos[3] = {};

	for (rapidjson::SizeType i = 0; i < jsonValue.Size(); ++i) 
	{
		const rapidjson::Value& person = jsonValue[i];
		float position = person.GetFloat();
		pos[i] = position;
	}

	vectorOut.x = pos[0];
	vectorOut.y = pos[1]; 
	vectorOut.z = pos[2];

	return true;
}

bool cJsonHandler::SetValue(const rapidjson::Value& jsonValue, glm::vec4& vectorOut)
{
	if (!jsonValue.IsArray())
	{
		return false;
	}

	float pos[4] = {};

	for (rapidjson::SizeType i = 0; i < jsonValue.Size(); ++i)
	{
		const rapidjson::Value& person = jsonValue[i];
		float position = person.GetFloat();
		pos[i] = position;
	}

	vectorOut.x = pos[0];
	vectorOut.y = pos[1];
	vectorOut.z = pos[2];
	vectorOut.w = pos[3];

	return true;
}

bool cJsonHandler::SetValue(const rapidjson::Value& jsonValue, glm::quat& vectorOut)
{
	if (!jsonValue.IsArray())
	{
		return false;
	}

	float pos[4] = {};

	for (rapidjson::SizeType i = 0; i < jsonValue.Size(); ++i)
	{
		const rapidjson::Value& person = jsonValue[i];
		float position = person.GetFloat();
		pos[i] = position;
	}

	vectorOut.w = pos[0];
	vectorOut.x = pos[1];
	vectorOut.y = pos[2];
	vectorOut.z = pos[3];

	return true;
}

bool cJsonHandler::SetValue(const rapidjson::Value& jsonValue, std::vector <sModel>& sceneModelsOut)
{
	if (!jsonValue.IsArray())
	{
		return false;
	}

	return true;
}

bool cJsonHandler::SetValue(const rapidjson::Value& jsonValue, std::vector <sTextureInfo>& textureInfoOut)
{
	if (!jsonValue.IsArray())
	{
		return false;
	}

	for (unsigned int i = 0; i < jsonValue.Size(); i++)
	{
		sTextureInfo texture;

		if (!SetValue(jsonValue[i]["Name"], texture.Name))
		{
			return false;
		}

		if (!SetValue(jsonValue[i]["Ratio"], texture.Ratio))
		{
			return false;
		}

		textureInfoOut.push_back(texture);
	}


	return true;
}

bool cJsonHandler::SerializeModel(rapidjson::Value& jsonValue, sModel& sModelOut, rapidjson::Document::AllocatorType& allocator)
{
	using namespace rapidjson;

	Value model(kObjectType);
	model.SetObject();

	Value position(kArrayType);
	position.SetArray();
	position.PushBack(sModelOut.Position.x, allocator);
	position.PushBack(sModelOut.Position.y, allocator);
	position.PushBack(sModelOut.Position.z, allocator);

	Value orientation(kArrayType);
	orientation.SetArray();
	orientation.PushBack(sModelOut.Orientation.w, allocator);
	orientation.PushBack(sModelOut.Orientation.x, allocator);
	orientation.PushBack(sModelOut.Orientation.y, allocator);
	orientation.PushBack(sModelOut.Orientation.z, allocator);

	Value scale(kArrayType);
	scale.SetArray();
	scale.PushBack(sModelOut.DrawScale.x, allocator);
	scale.PushBack(sModelOut.DrawScale.y, allocator);
	scale.PushBack(sModelOut.DrawScale.z, allocator);

	Value colorRGBA(kArrayType);
	colorRGBA.SetArray();
	colorRGBA.PushBack(sModelOut.DebugColourRGBA.x, allocator);
	colorRGBA.PushBack(sModelOut.DebugColourRGBA.y, allocator);
	colorRGBA.PushBack(sModelOut.DebugColourRGBA.z, allocator);
	colorRGBA.PushBack(sModelOut.DebugColourRGBA.w, allocator);

	Value textures(kArrayType);
	textures.SetArray();
	for (unsigned int j = 0; j < sModelOut.ObjectTextures.size(); j++)
	{
		Value texture(kObjectType);
		texture.SetObject();

		texture.AddMember("Name", Value(sModelOut.ObjectTextures[j].Name.c_str(), allocator), allocator);
		texture.AddMember("Ratio", sModelOut.ObjectTextures[j].Ratio, allocator);

		textures.PushBack(texture, allocator);
	}

	model.AddMember("MeshName", Value(sModelOut.MeshName.c_str(), allocator), allocator);
	model.AddMember("Position", Value(position, allocator), allocator);
	model.AddMember("FriendlyName", Value(sModelOut.FriendlyName.c_str(), allocator), allocator);
	model.AddMember("ObjectType", Value(sModelOut.ObjectType.c_str(), allocator), allocator);
	model.AddMember("IsUsingDebugColors", sModelOut.IsUsingDebugColors, allocator);
	model.AddMember("IsUsingVertexColors", sModelOut.IsUsingVertexColors, allocator);
	model.AddMember("IsUsingBones", sModelOut.IsUsingBones, allocator);
	model.AddMember("Orientation", Value(orientation, allocator), allocator);
	model.AddMember("DrawScale", Value(scale, allocator), allocator);
	model.AddMember("DebugColourRGBA", Value(colorRGBA, allocator), allocator);
	model.AddMember("Alpha", sModelOut.Alpha, allocator);
	model.AddMember("ObjectTextures", Value(textures, allocator), allocator);

	jsonValue = model;
	return true;
}

bool cJsonHandler::SerializeCamera(rapidjson::Value& jsonValue, sCameraInfo& sCameraOut, rapidjson::Document::AllocatorType& allocator)
{
	using namespace rapidjson;

	Value camera(kObjectType);
	camera.SetObject();

	Value position(kArrayType);
	position.SetArray();
	position.PushBack(sCameraOut.Position.x, allocator);
	position.PushBack(sCameraOut.Position.y, allocator);
	position.PushBack(sCameraOut.Position.z, allocator);

	Value target(kArrayType);
	target.SetArray();
	target.PushBack(sCameraOut.Target.x, allocator);
	target.PushBack(sCameraOut.Target.y, allocator);
	target.PushBack(sCameraOut.Target.z, allocator);

	Value orientation(kArrayType);
	orientation.SetArray();
	orientation.PushBack(sCameraOut.Orientation.w, allocator);
	orientation.PushBack(sCameraOut.Orientation.x, allocator);
	orientation.PushBack(sCameraOut.Orientation.y, allocator);
	orientation.PushBack(sCameraOut.Orientation.z, allocator);

	Value forward(kArrayType);
	forward.SetArray();
	forward.PushBack(sCameraOut.Forward.x, allocator);
	forward.PushBack(sCameraOut.Forward.y, allocator);
	forward.PushBack(sCameraOut.Forward.z, allocator);

	Value up(kArrayType);
	up.SetArray();
	up.PushBack(sCameraOut.Up.x, allocator);
	up.PushBack(sCameraOut.Up.y, allocator);
	up.PushBack(sCameraOut.Up.z, allocator);

	camera.AddMember("CameraType", Value(sCameraOut.CameraType.c_str(), allocator), allocator);
	camera.AddMember("Position", Value(position, allocator), allocator);
	camera.AddMember("Target", Value(target, allocator), allocator);
	camera.AddMember("Orientation", Value(orientation, allocator), allocator);
	camera.AddMember("Forward", Value(forward, allocator), allocator);
	camera.AddMember("Up", Value(up, allocator), allocator);
	camera.AddMember("NearClip", sCameraOut.NearClip, allocator);
	camera.AddMember("FarClip", sCameraOut.FarClip, allocator);

	jsonValue = camera;
	return true;
}

bool cJsonHandler::SerializeLight(rapidjson::Value& jsonValue, sLightsInfo& sLightOut, rapidjson::Document::AllocatorType& allocator)
{
	using namespace rapidjson;

	Value light(kObjectType);
	light.SetObject();

	Value position(kArrayType);
	position.SetArray();
	position.PushBack(sLightOut.Position.x, allocator);
	position.PushBack(sLightOut.Position.y, allocator);
	position.PushBack(sLightOut.Position.z, allocator);
	position.PushBack(sLightOut.Position.w, allocator);

	Value diffuse(kArrayType);
	diffuse.SetArray();
	diffuse.PushBack(sLightOut.Diffuse.x, allocator);
	diffuse.PushBack(sLightOut.Diffuse.y, allocator);
	diffuse.PushBack(sLightOut.Diffuse.z, allocator);
	diffuse.PushBack(sLightOut.Diffuse.w, allocator);

	Value specular(kArrayType);
	specular.SetArray();
	specular.PushBack(sLightOut.Specular.x, allocator);
	specular.PushBack(sLightOut.Specular.y, allocator);
	specular.PushBack(sLightOut.Specular.z, allocator);
	specular.PushBack(sLightOut.Specular.w, allocator);

	Value atten(kArrayType);
	atten.SetArray();
	atten.PushBack(sLightOut.Atten.x, allocator);
	atten.PushBack(sLightOut.Atten.y, allocator);
	atten.PushBack(sLightOut.Atten.z, allocator);
	atten.PushBack(sLightOut.Atten.w, allocator);

	Value direction(kArrayType);
	direction.SetArray();
	direction.PushBack(sLightOut.Direction.x, allocator);
	direction.PushBack(sLightOut.Direction.y, allocator);
	direction.PushBack(sLightOut.Direction.z, allocator);
	direction.PushBack(sLightOut.Direction.w, allocator);

	Value param1(kArrayType);
	param1.SetArray();
	param1.PushBack(sLightOut.Param1.x, allocator);
	param1.PushBack(sLightOut.Param1.y, allocator);
	param1.PushBack(sLightOut.Param1.z, allocator);
	param1.PushBack(sLightOut.Param1.w, allocator);

	Value param2(kArrayType);
	param2.SetArray();
	param2.PushBack(sLightOut.Param2.x, allocator);
	param2.PushBack(sLightOut.Param2.y, allocator);
	param2.PushBack(sLightOut.Param2.z, allocator);
	param2.PushBack(sLightOut.Param2.w, allocator);


	light.AddMember("Position", Value(position, allocator), allocator);
	light.AddMember("Diffuse", Value(diffuse, allocator), allocator);
	light.AddMember("Specular", Value(specular, allocator), allocator);
	light.AddMember("Atten", Value(atten, allocator), allocator);
	light.AddMember("Direction", Value(direction, allocator), allocator);
	light.AddMember("Param1", Value(param1, allocator), allocator);
	light.AddMember("Param2", Value(param2, allocator), allocator);
	light.AddMember("FriendlyName", Value(sLightOut.FriendlyName.c_str(), allocator), allocator);

	jsonValue = light;
	return true;
}

bool cJsonHandler::ParseCameraInfo(const rapidjson::Value& cameraInfoValue, std::vector<sCameraInfo>& cameraInfoOut)
{
	using namespace rapidjson;

	if (!cameraInfoValue.IsArray())
	{
		return false;
	}

	for (unsigned int i = 0; i < cameraInfoValue.Size(); i++)
	{
		sCameraInfo cameraInfo;

		if (!SetValue(cameraInfoValue[i]["CameraType"], cameraInfo.CameraType))
		{
			return false;
		}

		if (!SetValue(cameraInfoValue[i]["Position"], cameraInfo.Position))
		{
			return false;
		}

		if (!SetValue(cameraInfoValue[i]["Target"], cameraInfo.Target))
		{
			return false;
		}

		if (!SetValue(cameraInfoValue[i]["Orientation"], cameraInfo.Orientation))
		{
			return false;
		}

		if (!SetValue(cameraInfoValue[i]["Forward"], cameraInfo.Forward))
		{
			return false;
		}

		if (!SetValue(cameraInfoValue[i]["Up"], cameraInfo.Up))
		{
			return false;
		}

		if (!SetValue(cameraInfoValue[i]["NearClip"], cameraInfo.NearClip))
		{
			return false;
		}

		if (!SetValue(cameraInfoValue[i]["FarClip"], cameraInfo.FarClip))
		{
			return false;
		}

		cameraInfoOut.push_back(cameraInfo);
	}

	return true;
}

bool cJsonHandler::ParseModelsWithTextureInfo(const rapidjson::Value& preferenceValue, std::vector<std::string>& modelsInfoOut)
{
	if (!preferenceValue.IsArray())
	{
		return false;
	}

	for (unsigned int i = 0; i < preferenceValue.Size(); i++)
	{
		modelsInfoOut.push_back(preferenceValue[i].GetString());
	}

	return true;
}

bool cJsonHandler::ParsePhysicsInfo(const rapidjson::Value& languageValue, sPhysicsInfo& languageOut)
{
    return false;
}

bool cJsonHandler::ParseTextureInfo(const rapidjson::Value& preferenceValue, std::vector<std::string>& texturesOut)
{
	if (!preferenceValue.IsArray())
	{
		return false;
	}

	for (unsigned int i = 0; i < preferenceValue.Size(); i++)
	{
		texturesOut.push_back(preferenceValue[i].GetString());
	}

	return true;
}

bool cJsonHandler::ParseSceneModels(const rapidjson::Value& preferenceValue, std::vector<sModel>& sceneModelsOut)
{
	if (!preferenceValue.IsArray())
	{
		return false;
	}

	for (unsigned int i = 0; i < preferenceValue.Size(); i++)
	{
		sModel model;


		if (!SetValue(preferenceValue[i]["MeshName"], model.MeshName))
		{
			return false;
		}

		if (!SetValue(preferenceValue[i]["Position"], model.Position))
		{
			return false;
		}

		if (!SetValue(preferenceValue[i]["Orientation"], model.Orientation))
		{
			return false;
		}

		if (!SetValue(preferenceValue[i]["FriendlyName"], model.FriendlyName))
		{
			return false;
		}

		if (!SetValue(preferenceValue[i]["ObjectType"], model.ObjectType))
		{
			return false;
		}

		if (!SetValue(preferenceValue[i]["IsUsingDebugColors"], model.IsUsingDebugColors))
		{
			return false;
		}

		if (!SetValue(preferenceValue[i]["IsUsingVertexColors"], model.IsUsingVertexColors))
		{
			return false;
		}

		if (!SetValue(preferenceValue[i]["IsUsingBones"], model.IsUsingBones))
		{
			return false;
		}

		if (!SetValue(preferenceValue[i]["DrawScale"], model.DrawScale))
		{
			return false;
		}

		if (!SetValue(preferenceValue[i]["DebugColourRGBA"], model.DebugColourRGBA))
		{
			return false;
		}

		if (!SetValue(preferenceValue[i]["Alpha"], model.Alpha))
		{
			return false;
		}

		if (!SetValue(preferenceValue[i]["ObjectTextures"], model.ObjectTextures))
		{
			return false;
		}

		sceneModelsOut.push_back(model);
	}

	return true;
}

bool cJsonHandler::ParseSceneLights(const rapidjson::Value& preferenceValue, std::vector<sLightsInfo>& sceneLightsOut)
{
	if (!preferenceValue.IsArray())
	{
		return false;
	}

	for (unsigned int i = 0; i < preferenceValue.Size(); i++)
	{
		sLightsInfo light;

		if (!SetValue(preferenceValue[i]["FriendlyName"], light.FriendlyName))
		{
			return false;
		}

		if (!SetValue(preferenceValue[i]["Position"], light.Position))
		{
			return false;
		}

		if (!SetValue(preferenceValue[i]["Diffuse"], light.Diffuse))
		{
			return false;
		}

		if (!SetValue(preferenceValue[i]["Specular"], light.Specular))
		{
			return false;
		}

		if (!SetValue(preferenceValue[i]["Atten"], light.Atten))
		{
			return false;
		}

		if (!SetValue(preferenceValue[i]["Direction"], light.Direction))
		{
			return false;
		}

		if (!SetValue(preferenceValue[i]["Param1"], light.Param1))
		{
			return false;
		}

		if (!SetValue(preferenceValue[i]["Param2"], light.Param2))
		{
			return false;
		}

		sceneLightsOut.push_back(light);
	}

	return true;
}

bool cJsonHandler::ParseScene(const rapidjson::Value& preferenceValue, sSceneInfo& sceneOut)
{
	if (!preferenceValue.IsObject())
	{
		return false;
	}

	// Parse meshes

	if (!ParseSceneModels(preferenceValue["meshes"], sceneOut.ModelInfo))
	{
		return false;
	}

	if (!ParseCameraInfo(preferenceValue["cameras"], sceneOut.CameraInfo))
	{
		return false;
	}

	if (!ParseSceneLights(preferenceValue["lights"], sceneOut.LightsInfo))
	{
		return false;
	}

	return true;
}
