//#pragma once
//
//#pragma warning(push)
//#pragma warning(disable: 26495) // 26495 is the warning code for not initializing data members
//#include <fbxsdk/fbxsdk.h>
//#pragma warning(pop)
//
//
//#include "cVAOManager/sModelDrawInfo.h"
//#include "Bones.h"
//
//class FBXLoader
//{
//	FbxManager* m_fbxManager = nullptr;
//	FbxIOSettings* m_fbxIOSettings = nullptr;
//
//public:
//	FBXLoader();
//	~FBXLoader();
//
//	void LoadFBXScene(const char* filePath);
//	void LoadMeshData(FbxNode* node, sModelDrawInfo& drawInfo);
//	void LoadBonesAndAnimations(FbxNode* node, int depth);
//
//	void LoadBones(FbxNode* node, int depth);
//	void LoadAnimations(FbxNode* node, int depth);
//
//	Bones::Node* CreateAnimNode(FbxNode* node);
//	Bones::Node* GenerateBoneHierarchy(FbxNode* node, sModelDrawInfo& drawInfo, const int depth = 0);
//	void FBXToGLM(const fbxsdk::FbxAMatrix& a, glm::mat4& g);
//	
//	Bones::CharacterAnimation* characterAnimation = nullptr;
//
//	std::vector< Bones::CharacterAnimation*> characterAnimations;
//
//	void Run()
//	{
//		LoadFBXScene("assets/models/Idle.FBX");
//	}
//
//	bool LoadFBXScene(const char* filePath, sModelDrawInfo& drawInfo, std::string& errorInfo);
//
//	bool LoadAnimation(const char* fileName, const char* filePath, sModelDrawInfo& drawInfo, std::string& errorInfo);
//};
//
