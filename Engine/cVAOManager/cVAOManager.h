#ifndef _cVAOManager_HG_
#define _cVAOManager_HG_

#pragma warning(push)
#pragma warning(disable: 26495) // 26495 is the warning code for not initializing data members
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#pragma warning(pop)

#include <string>
#include <map>

#include "sModelDrawInfo.h"
#include "../cMesh.h"
#include "../cAnimation.h"
#include "../Bones.h"

//#include "../FBXLoader.h"


class cVAOManager
{
private:
	bool m_LoadTheFile_Ply_XYZ_N_RGBA(std::string theFileName, sModelDrawInfo& drawInfo);
	bool m_LoadTheFile_Ply_XYZ_N_RGBA_UV(std::string theFileName, sModelDrawInfo& drawInfo);

	// TODO: replace map for more readable vector
	//std::vector< sModelDrawInfo > m_map_ModelName_to_VAOID;

	std::string m_basePathWithoutSlash;


public:

	struct sVertexPlyFileWithTexture
	{
		float x, y, z;
		float nx, ny, nz;
		float r, g, b, a;
		float u, v;             // <--- NEW!!!
	};

	std::map< std::string /*model name*/,
		sModelDrawInfo /* info needed to draw*/ >
		m_map_ModelName_to_VAOID;

	//FBXLoader* fbxLoader = nullptr;

	struct ModelInfo
	{
		std::string modelFolderName;
		std::vector<std::string> modelFiles;
	};

	struct TextureInfo
	{
		std::string textureFolderName;
		std::vector<std::string> textureFiles;
	};

	cVAOManager(){
		//fbxLoader = new FBXLoader();
	}

	bool LoadModelIntoVAO(std::string fileName,
		sModelDrawInfo& drawInfo,
		unsigned int shaderProgramID,
		bool bIsDynamicBuffer = false);

	bool LoadModelIntoVAOWithoutTexture(std::string fileName,
		sModelDrawInfo& drawInfo,
		unsigned int shaderProgramID,
		bool bIsDynamicBuffer = false);

	// Simpler one that doesn't need the draw info passed
	bool LoadModelIntoVAO(std::string fileName,
		unsigned int shaderProgramID);

	// We don't want to return an int, likely
	bool FindDrawInfoByModelName(std::string filename,
		sModelDrawInfo& drawInfo);

	std::string getLastError(bool bAndClear = true);

	void setBasePath(std::string basePathWithoutSlash);
	std::string getBasePath(void);

	// Here you have to be a little careful about what you pass. 
	// It will use the vertex and element (triangle) buffer IDs 
	//	to update - i.e. OVERWRITE - the buffers with whatever
	//	you've got in the two pointers. 
	// You can get this information by calling FindDrawInfoByModelName()
	//	and using the drawInfo that is returned. Note the sDrawInfo is a
	//	COPY of the structure in the map, but the pointers to the 
	//	vertex and element buffers are the ONE AND ONLY COPIES that
	//	exist on the CPU side. So if you overwrite them, they're gone.
	bool FindDrawInfoPointerByModelName(std::string fileName,
		sModelDrawInfo*& drawInfo,
		unsigned int shaderProgramID);

	bool UpdateVAOBuffers(std::string fileName,
		sModelDrawInfo& updatedDrawInfo,
		unsigned int shaderProgramID);

	// Same as above but ONLY updates the vertex buffer information
	bool UpdateVertexBuffers(std::string fileName,
		sModelDrawInfo& updatedDrawInfo,
		unsigned int shaderProgramID);

	// Assimp
	bool LoadModelWithAssimp(std::string fileName,
		sModelDrawInfo& drawInfo,
		unsigned int shaderProgramID,
		bool bIsDynamicBuffer = false);

	bool LoadAnimationWithAssimp(std::string fileName,
		sModelDrawInfo& drawInfo);

	// FBX
	bool LoadModelWithFBX(std::string fileName,
		sModelDrawInfo& drawInfo,
		unsigned int shaderProgramID,
		bool bIsDynamicBuffer = false);

	//bool LoadAnimationWithFBX(std::string fileName,
	//	sModelDrawInfo& drawInfo,
	//	unsigned int shaderProgramID,
	//	bool bIsDynamicBuffer = false);


	//Bones::CharacterAnimation* GetAnimationByName(std::string name);

	Bones::Node* GenerateBoneHierarchy(aiNode* assimpNode, sModelDrawInfo& drawInfo,  const int depth = 0);

	Bones::Node* CreateAnimNode(aiNode* node);

	//std::vector<Bones::CharacterAnimation*> CharacterAnimations;
	//Bones::Node* RootNode;
	//glm::mat4 GlobalInverseTransformation;

	const int NUM_BONES = 150;
	GLuint BoneMatricesUL[150] = {};

	Bones::CharacterAnimation* characterAnimation = nullptr;

	const char** GetNameOfAllModels();
	unsigned int GetNumberOfModels();

	const char** GetTypesOfModels();
	unsigned int GetNumberOfModelTypes();

	const char** GetNameOfAllModelsByType(const char* type);
	unsigned int GetNumberOfModelsOfType(const char* type);

	//std::vector < std::string > modelTypes;

	std::vector < ModelInfo > modelInfo;
	std::vector < TextureInfo > textureInfo;

	std::vector < std::string> GetModelsByType(std::string type);
};

#endif	// _cVAOManager_HG_
