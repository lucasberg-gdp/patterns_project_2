//#include "FBXLoader.h"
//#include <iostream>
//
//#include "LogUtils.h"
//
//#include "cAnimation.h"
//
//FBXLoader::FBXLoader()
//{
//	m_fbxManager = FbxManager::Create();
//	m_fbxIOSettings = FbxIOSettings::Create(m_fbxManager, IOSROOT);
//	m_fbxManager->SetIOSettings(m_fbxIOSettings);
//
//	 characterAnimation = new Bones::CharacterAnimation();
//
//}
//
//FBXLoader::~FBXLoader()
//{
//	m_fbxManager->Destroy();
//	//m_fbxIOSettings->Destroy();
//}
//
//void FBXLoader::LoadFBXScene(const char* filePath)
//{
//	//FbxImporter* importer = FbxImporter::Create(m_fbxManager, "");
//	//
//	//if (!importer->Initialize(filePath, -1, m_fbxManager->GetIOSettings()))
//	//{
//	//	return;
//	//}
//
//	//FbxScene* scene = FbxScene::Create(m_fbxManager, "MyScene");
//	//importer->Import(scene);
//
//	//// Read everything
//	//FbxNode* root = scene->GetRootNode();
//
//	//if (root != nullptr)
//	//{
//	//	LoadMeshData(root);
//	//	LoadBonesAndAnimations(root);
//	//}
//
//	//// When you are done, cleanup
//
//	//importer->Destroy();
//}
//
//void FBXLoader::LoadMeshData(FbxNode* node, sModelDrawInfo& drawInfo)
//{
//	// Recurse this
//	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();
//
//	// Mesh information
//	if (nodeAttribute != nullptr)
//	{
//		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
//		{
//			FbxMesh* mesh = static_cast<FbxMesh*>(nodeAttribute);
//
//			const char* meshName = mesh->GetName();
//
//			fbxsdk::FbxVector4* vertices = mesh->GetControlPoints();
//			int vertexCount = mesh->GetControlPointsCount();
//
//			drawInfo.numberOfVertices = vertexCount;
//			drawInfo.pVertices = new sVertex[drawInfo.numberOfVertices];
//
//			// Load your mesh in
//			for (unsigned int i = 0; i < vertexCount; i++)
//			{
//				fbxsdk::FbxVector4 vertex = vertices[i];
//
//				drawInfo.pVertices[i].x = (float)vertex.mData[0];
//				drawInfo.pVertices[i].y = (float)vertex.mData[0];
//				drawInfo.pVertices[i].z = (float)vertex.mData[0];
//				drawInfo.pVertices[i].w = (float)vertex.mData[0];
//			}
//
//
//
// //        if (mesh->HasBones())
// //        {
// //            BoneWeightInfo& boneInfo = boneWeights[index];
//
// //            drawInfo.pVertices[vertArrayIndex].bones1 = (float)boneInfo.id[0];
// //            drawInfo.pVertices[vertArrayIndex].bones2 = (float)boneInfo.id[1];
// //            drawInfo.pVertices[vertArrayIndex].bones3 = (float)boneInfo.id[2];
// //            drawInfo.pVertices[vertArrayIndex].bones4 = (float)boneInfo.id[3];
//
// //            drawInfo.pVertices[vertArrayIndex].boneWeight1 = boneInfo.weight[0];
// //            drawInfo.pVertices[vertArrayIndex].boneWeight2 = boneInfo.weight[1];
// //            drawInfo.pVertices[vertArrayIndex].boneWeight3 = boneInfo.weight[2];
// //            drawInfo.pVertices[vertArrayIndex].boneWeight4 = boneInfo.weight[3];
//
// //            float weight = boneInfo.weight[0] + boneInfo.weight[1] + boneInfo.weight[2] + boneInfo.weight[3];
// //            if (weight != 1.f)
// //            {
// //                int breakhere = 0;
// //            }
// //        }
//
// //        drawInfo.pIndices[vertArrayIndex] = vertArrayIndex;
//
// //        vertArrayIndex++;
// //    }
// //}
//
//			// Normals
//			if (mesh->GetElementNormalCount() > 0)
//			{
//				FbxGeometryElementNormal* normalElement = mesh->GetElementNormal();
//
//				if (normalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
//				{
//					for (unsigned int i = 0; i < vertexCount; i++)
//					{
//						fbxsdk::FbxVector4 normal = normalElement->GetDirectArray().GetAt(i);
//
//						drawInfo.pVertices[i].nx = (float)normal.mData[0];
//						drawInfo.pVertices[i].ny = (float)normal.mData[1];
//						drawInfo.pVertices[i].nz = (float)normal.mData[2];
//						drawInfo.pVertices[i].nw = 0.0f;
//					}
//				}
//			}
//
//			// UV
//			if (mesh->GetElementUVCount() > 0)
//			{
//				FbxGeometryElementUV* textureElement = mesh->GetElementUV();
//
//				for (unsigned int i = 0; i < vertexCount; i++)
//				{
//					fbxsdk::FbxVector2 texture = textureElement->GetDirectArray().GetAt(i);
//
//					drawInfo.pVertices[i].u = texture.mData[0];
//					drawInfo.pVertices[i].v = texture.mData[1];
//				}
//
//			}
//
//			// Material
//			if (mesh->GetElementMaterialCount() > 0)
//			{
//				fbxsdk::FbxGeometryElementMaterial* meshMaterial =  mesh->GetElementMaterial();
//
//				std::string materialName = meshMaterial->GetName();
//
//				std::cout << "Material name: " << materialName;
//
//				//for (unsigned int i = 0; i < mesh->GetElementMaterialCount(); i++)
//				//{
//
//				//}
//
//			}
//
//			int numDeformers = mesh->GetDeformerCount(FbxDeformer::eSkin);
//
//			for (int i = 0; i < numDeformers; ++i)
//			{
//				fbxsdk::FbxSkin* skinDeformer = static_cast<FbxSkin*>(mesh->GetDeformer(i, FbxDeformer::eSkin));
//				int numClusters = skinDeformer->GetClusterCount();
//
//				for (int j = 0; j < numClusters; ++j)
//				{
//					fbxsdk::FbxCluster* cluster = skinDeformer->GetCluster(j);
//					fbxsdk::FbxNode* boneNode = cluster->GetLink();
//
//					if (boneNode)
//					{
//						int boneIndex = -1;
//						// Get the index of the bone node in its parent's child list
//						FbxNode* parentNode = boneNode->GetParent();
//
//						if (parentNode) 
//						{
//							for (int k = 0; k < parentNode->GetChildCount(); ++k) 
//							{
//								if (parentNode->GetChild(k) == boneNode) 
//								{
//									boneIndex = k;
//									break;
//								}
//							}
//						}
//						if (boneIndex >= 0) 
//						{
//							if (cluster->GetControlPointWeights() == nullptr)
//							{
//								continue;
//							}
//
//							// Retrieve bone weights for each vertex
//							int numControlPoints = mesh->GetControlPointsCount();
//							for (int k = 0; k < numControlPoints; ++k) 
//							{
//								double weight = cluster->GetControlPointWeights()[k];
//								if (weight > 0.0) 
//								{
//									// Store or process bone weight (vertex index, bone index, weight)
//									std::cout << "Vertex " << k << " is influenced by bone " << boneIndex << " with weight " << weight << std::endl;
//
//
//
//								}
//							}
//						}
//					}
//				}
//			}
//
//		}
//
//
//
//
//		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
//		{
//			// TODO: Read it
//
//
//		}
//	}
//
//
//
//
//	unsigned int childrenCount = node->GetChildCount();
//
//	for (unsigned int i = 0; i < childrenCount; i++)
//	{
//		LoadMeshData(node->GetChild(i), drawInfo);
//	}
//}
//
//void FBXLoader::LoadBonesAndAnimations(fbxsdk::FbxNode* node, int depth = 0)
//{
//	fbxsdk::FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();
//
//	if (nodeAttribute != nullptr)
//	{
//		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
//		{
//			fbxsdk::FbxSkeleton* skeleton = static_cast<FbxSkeleton*>(nodeAttribute);
//			
//			const char* boneName = node->GetName();
//			const char* parentName = (node->GetParent() != nullptr) ? node->GetParent()->GetName() : "None";
//
//			//printf("%*s%s\n", depth, "", boneName);
//			fbxsdk::FbxAMatrix transform = node->EvaluateGlobalTransform();
//
//			fbxsdk::FbxAnimStack* animStack = node->GetScene()->GetCurrentAnimationStack();
//
//			if (animStack != nullptr)
//			{
//				Bones::NodeAnim* nodeAnim = new Bones::NodeAnim(std::string("Armature_" + std::string(node->GetName())));
//
//				FbxString lAnimationName = animStack->GetName();
//				FbxTakeInfo* lTakeInfo = node->GetScene()->GetTakeInfo(lAnimationName);
//				FbxTime start = lTakeInfo->mLocalTimeSpan.GetStart();
//				FbxTime end = lTakeInfo->mLocalTimeSpan.GetStop();
//
//				//std::cout << "Start: " << start.Get() << " End: " << end.Get() << std::endl;
//
//				for (FbxLongLong i = start.GetFrameCount(FbxTime::eFrames24); i <= end.GetFrameCount(FbxTime::eFrames24); i++)
//				{
//					FbxTime currentTime;
//					currentTime.SetFrame(i, FbxTime::eFrames24);
//
//					for (int j = 0; j < node->GetChildCount(); j++)
//					{
//						FbxNode* childNode = node->GetChild(j);
//						if (childNode->GetNodeAttribute() && childNode->GetNodeAttribute()->GetAttributeType() && childNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
//						{
//							// Get the global position of the bone at this frame
//							fbxsdk::FbxAMatrix globalPosition = childNode->EvaluateGlobalTransform(currentTime);
//							glm::mat4 nodeTransform;
//							FBXToGLM(globalPosition, nodeTransform);
//							//LogUtils::Mat4Log(nodeTransform);
//
//							fbxsdk::FbxVector4 position = globalPosition.GetT();
//							//std::cout << "Position: (" << position.mData[0] << ", " << position.mData[1] << ", " << position.mData[2] << ")" << std::endl;
//							
//							nodeAnim->m_PositionKeyFrames.emplace_back(
//								cAnimation::PositionKeyFrame(glm::vec3((float)position.mData[0], (float)position.mData[1], (float)position.mData[2]), currentTime.GetSecondDouble()));
//
//							fbxsdk::FbxVector4 scale = globalPosition.GetS();
//							//std::cout << "Scale: (" << scale.mData[0] << ", " << scale.mData[1] << ", " << scale.mData[2] << ")" << std::endl;
//
//							nodeAnim->m_ScaleKeyFrames.emplace_back(
//								cAnimation::ScaleKeyFrame(glm::vec3((float)scale.mData[0], (float)scale.mData[1], (float)scale.mData[2]), currentTime.GetSecondDouble()));
//
//							fbxsdk::FbxQuaternion quat = globalPosition.GetQ();
//							//std::cout << "Rotation: (" << quat.mData[0] << ", " << quat.mData[1] << ", " << quat.mData[2] << ")" << std::endl;
//
//							nodeAnim->m_RotationKeyFrames.emplace_back(
//								cAnimation::RotationKeyFrame(glm::quat((float)quat.mData[0], (float)quat.mData[1], (float)quat.mData[2], (float)quat.mData[3]), currentTime.GetSecondDouble()));
//						}
//					}
//				}
//
//				characterAnimation->Channels.emplace_back(nodeAnim);
//			}
//		}
//	}
//
//	for (unsigned int i = 0; i < node->GetChildCount(); i++)
//	{
//		LoadBonesAndAnimations(node->GetChild(i), depth + 1);
//	}
//}
//
//void FBXLoader::LoadBones(FbxNode* node, int depth)
//{
//
//}
//
//void FBXLoader::LoadAnimations(FbxNode* node, int depth)
//{
//
//}
//
//void FBXLoader::FBXToGLM(const fbxsdk::FbxAMatrix& a, glm::mat4& g)
//{
//	g[0][0] = a.Get(0,0); g[0][1] = a.Get(0,1); g[0][2] = a.Get(0,2); g[0][3] = a.Get(0,3);
//	g[1][0] = a.Get(1,0); g[1][1] = a.Get(1,1); g[1][2] = a.Get(1,2); g[1][3] = a.Get(1,3);
//	g[2][0] = a.Get(2,0); g[2][1] = a.Get(2,1); g[2][2] = a.Get(2,2); g[2][3] = a.Get(2,3);
//	g[3][0] = a.Get(3,0); g[3][1] = a.Get(3,1); g[3][2] = a.Get(3,2); g[3][3] = a.Get(3,3);
//}
//
//Bones::Node* FBXLoader::CreateAnimNode(FbxNode* node)
//{
//	Bones::Node* newNode = new Bones::Node(node->GetName());
//	FBXToGLM(node->EvaluateLocalTransform(), newNode->Transformation);
//	return newNode;
//}
//
//Bones::Node* FBXLoader::GenerateBoneHierarchy(FbxNode* fbxNode, sModelDrawInfo& drawInfo, const int depth)
//{
//	Bones::Node* node = CreateAnimNode(fbxNode);
//	//PRINT_SPACES(depth); printf("%s\n", assimpNode->mName.C_Str());
//	FbxAMatrix& transformation = fbxNode->EvaluateLocalTransform();
//
//	FbxVector4 position = transformation.GetRow(0);
//	FbxVector4 rotation = transformation.GetRow(1);
//	FbxVector4 scale = transformation.GetRow(2);
//
//	//fbxsdk::FbxVec
//	//aiVector3D position;
//	//aiQuaternion rotation;
//	//aiVector3D scaling;
//	//transformation.Decompose(scaling, rotation, position);
//	//PRINT_SPACES(depth + 1); printf("Position: {%.3f, %.3f, %.3f}\n", position.x, position.y, position.z);
//	//PRINT_SPACES(depth + 1); printf("Scaling: {%.3f, %.3f, %.3f}\n", scaling.x, scaling.y, scaling.z);
//	//PRINT_SPACES(depth + 1); printf("Quaternion: {%.3f, %.3f, %.3f, %.3f}\n", rotation.x, rotation.y, rotation.z, rotation.w);
//	//printf("\n");
//
//	glm::mat4 glmMatrix;
//	FBXToGLM(transformation, glmMatrix);
//
//	LogUtils::Mat4Log(glmMatrix);
//
//	//glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.f), glm::vec3(position.x, position.y, position.z));
//	//glm::mat4 RotationMatrix = glm::mat4_cast(glm::quat(rotation.w, rotation.x, rotation.y, rotation.z));
//	//glm::mat4 ScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaling.x, scaling.y, scaling.z));
//
//	//glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;
//	drawInfo.NodeNameToIdMap.insert(std::pair<std::string, int>(fbxNode->GetName(), (int)(drawInfo.NodeHeirarchyTransformations.size())));
//
//	//drawInfo.NodeNameToId.push_back(NodeNameToId(assimpNode->mName.C_Str(), (int)(drawInfo.NodeHeirarchyTransformations.size())));
//
//	drawInfo.NodeHeirarchyTransformations.emplace_back(glmMatrix);
//
//	for (unsigned int i = 0; i < fbxNode->GetChildCount(); ++i)
//	{
//		node->Children.emplace_back(GenerateBoneHierarchy(fbxNode->GetChild(i), drawInfo, depth + 1));
//	}
//
//	return node;
//}
//
//bool FBXLoader::LoadFBXScene(const char* filePath, sModelDrawInfo& drawInfo, std::string& errorInfo)
//{
//	FbxImporter* importer = FbxImporter::Create(m_fbxManager, "");
//
//	if (!importer->Initialize(filePath, -1, m_fbxManager->GetIOSettings()))
//	{
//		errorInfo += "Initialize failed";
//		return false;
//	}
//
//	FbxScene* scene = FbxScene::Create(m_fbxManager, "MyScene");
//	importer->Import(scene);
//
//	// Read everything
//	FbxNode* root = scene->GetRootNode();
//
//	if (root != nullptr)
//	{
//		LoadMeshData(root, drawInfo);
//		LoadBonesAndAnimations(root);
//	}
//
//
//
//
//	// When you are done, cleanup
//	importer->Destroy();
//
//	return true;
//}
//
//void PrintPositionKeyframes(fbxsdk::FbxAnimCurve* curve) 
//{
//	if (!curve)
//	{
//		return;
//	}
//
//	int numKeys = curve->KeyGetCount();
//	for (int i = 0; i < numKeys; ++i) 
//	{
//		fbxsdk::FbxTime keyTime = curve->KeyGetTime(i);
//		float keyValue = curve->Evaluate(keyTime);
//		printf("Time: %f, Position: (%f)\n", keyTime.GetSecondDouble(), keyValue);
//	}
//}
//
//
//bool FBXLoader::LoadAnimation(const char* fileName, const char* filePath, sModelDrawInfo& drawInfo, std::string& errorInfo)
//{
//	std::cout << "Loading animations in: " << fileName << std::endl;
//
//	FbxImporter* importer = FbxImporter::Create(m_fbxManager, "");
//
//	if (!importer->Initialize(filePath, -1, m_fbxManager->GetIOSettings()))
//	{
//		errorInfo += "Initialize failed";
//		return false;
//	}
//
//	FbxScene* scene = FbxScene::Create(m_fbxManager, "MyScene");
//	importer->Import(scene);
//
//	// Read everything
//	FbxNode* root = scene->GetRootNode();
//
//	characterAnimation = new Bones::CharacterAnimation();
//
//	characterAnimation->Name = fileName;
//	characterAnimation->RootNode = CreateAnimNode(root);
//	//characterAnimation->RootNode = root;
//
//	if (root != nullptr)
//	{
//		LoadBonesAndAnimations(root);
//	}
//
//	characterAnimations.push_back(characterAnimation);
//
//	// When you are done, cleanup
//	importer->Destroy();
//
//
//	return true;
//
//	//fbxsdk::FbxImporter* importer = FbxImporter::Create(m_fbxManager, "");
//
//	//if (!importer->Initialize(filePath, -1, m_fbxManager->GetIOSettings()))
//	//{
//	//	errorInfo += "Initialize failed";
//	//	return false;
//	//}
//
//	//fbxsdk::FbxScene* scene = FbxScene::Create(m_fbxManager, "MyScene");
//	//importer->Import(scene);
//
//	//// Read everything
//	//FbxNode* node = scene->GetRootNode();
//
//	//fbxsdk::FbxAnimStack* animStack = scene->GetCurrentAnimationStack();
//
//	//Bones::CharacterAnimation* modelAnimation = new Bones::CharacterAnimation(fileName);
//
//	//modelAnimation->Duration = animStack->GetLocalTimeSpan().GetDuration().GetSecondDouble();
//
//
//
//
//	//if (animStack != nullptr)
//	//{
//	//	fbxsdk::FbxAnimEvaluator* evaluator = scene->GetAnimationEvaluator();
//
//	//	const fbxsdk::FbxTimeSpan& timeSpan = animStack->GetLocalTimeSpan();
//
//	//	//animStack->Get
//
//	//	for (int layerIndex = 0; layerIndex < animStack->GetMemberCount(); layerIndex++) 
//	//	{
//	//		//fbxsdk::FbxAnimLayer* animLayer = animStack->GetMember<fbxsdk::FbxAnimLayer>(layerIndex);
//
//
//
//	//		//FbxProperty translationProperty = node->FindProperty(fbxsdk::FbxNode::::sTranslation);
//
//	//		//// Print position keyframes
//	//		//printf("Position Keyframes:\n");
//	//		//PrintPositionKeyframes(translationCurve);
//	//	}
//
//	//	std::cout << "Start time: " << timeSpan.GetStart().GetSecondDouble() << std::endl;
//	//	std::cout << "End time: " << timeSpan.GetStop().GetSecondDouble() << std::endl;
//
//	//		 
//
//	//	for (fbxsdk::FbxTime time = timeSpan.GetStart(); time <= timeSpan.GetStop(); time += fbxsdk::FbxTime(1.0 / 60.0))
//	//	{
//	//		FbxAMatrix matrix = evaluator->GetNodeGlobalTransform(node, time);
//
//	//		//std::cout << matrix.Get(0,0) << std::endl;
//	//		/*time.*/
//	//	}
//
//
//	//	//characterAnimation->Name = animation->mName.C_Str();
//	//	//characterAnimation->Duration = animation->mDuration;
//	//	//characterAnimation->TicksPerSecond = animation->mTicksPerSecond;
//
//	//	//for (unsigned int i = 0; i < animation->mNumChannels; ++i)
//	//	//{
//	//	//	aiNodeAnim* assimpNodeAnim = animation->mChannels[i];
//	//	//	Bones::NodeAnim* nodeAnim = new Bones::NodeAnim(assimpNodeAnim->mNodeName.C_Str());
//	//	//	for (unsigned int i = 0; i < assimpNodeAnim->mNumPositionKeys; ++i)
//	//	//	{
//	//	//		aiVectorKey& p = assimpNodeAnim->mPositionKeys[i];
//	//	//		nodeAnim->m_PositionKeyFrames.emplace_back(cAnimation::PositionKeyFrame(glm::vec3(p.mValue.x, p.mValue.y, p.mValue.z), p.mTime));
//	//	//	}
//	//	//	for (unsigned int i = 0; i < assimpNodeAnim->mNumScalingKeys; ++i)
//	//	//	{
//	//	//		aiVectorKey& s = assimpNodeAnim->mScalingKeys[i];
//	//	//		nodeAnim->m_ScaleKeyFrames.emplace_back(cAnimation::ScaleKeyFrame(glm::vec3(s.mValue.x, s.mValue.y, s.mValue.z), s.mTime));
//	//	//	}
//	//	//	for (unsigned int i = 0; i < assimpNodeAnim->mNumRotationKeys; ++i)
//	//	//	{
//	//	//		aiQuatKey& q = assimpNodeAnim->mRotationKeys[i];
//	//	//		nodeAnim->m_RotationKeyFrames.emplace_back(cAnimation::RotationKeyFrame(glm::quat(q.mValue.w, q.mValue.x, q.mValue.y, q.mValue.z), q.mTime));
//	//	//	}
//	//	//	characterAnimation->Channels.emplace_back(nodeAnim);
//	//	//}
//
//	//}
//
//
//
//
//	////if (root != nullptr)
//	////{
//	////	animation_stack = fbx_scene.GetSrcObject(0, fbx.FbxCriteria.ObjectType(fbx.FbxAnimStack.ClassId))
//	////	animation_layer = animation_stack.GetMember(0)
//	////	anim_curve_node = fbx_scene.FindSrcObject(animation_layer)
//
//	////	LoadMeshData(root, drawInfo);
//	////	LoadBonesAndAnimations(root);
//	////}
//
//	//// When you are done, cleanup
//	//importer->Destroy();
//
//	//return true;
//}
