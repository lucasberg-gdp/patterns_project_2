#pragma once

#include "../OpenGLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <vector>
#include <map>

#include "../Bones.h"


// The vertex structure 
//	that's ON THE GPU (eventually) 
// So dictated from THE SHADER
struct sVertex
{
	float x, y, z, w;		// vPos
	float r, g, b, a;		// vCol
	float nx, ny, nz, nw;	// vNormal
	float u, v;				// vTextureCoords		<--- NEW!!
	float bones1, bones2, bones3, bones4;
	float boneWeight1, boneWeight2, boneWeight3, boneWeight4;
};

struct sTriangle
{
	glm::vec3 vecA, vecB, vecC;
};

struct NodeNameToId
{
	NodeNameToId(const char* name, int id):
		Id(id),
		Name(name)
	{

	}

	int Id;
	std::string Name;
};

struct ModelTexture
{
	ModelTexture(std::string name, float ratio)
	{
		Name = name;
		Ratio = ratio;
	}

	std::string Name;
	float Ratio;
};

struct sModelDrawInfo
{
	sModelDrawInfo();
	~sModelDrawInfo();

	std::string modelName;
	std::string meshName;

	unsigned int VAO_ID;

	unsigned int VertexBufferID;
	unsigned int VertexBuffer_Start_Index;
	unsigned int numberOfVertices;

	unsigned int IndexBufferID;
	unsigned int IndexBuffer_Start_Index;
	unsigned int numberOfIndices;
	unsigned int numberOfTriangles;

	std::vector<Bones::BoneInfo> BoneInfoVec;
	GLuint Vbo;
	Bones::Node* RootNode;
	std::vector<Bones::CharacterAnimation*> CharacterAnimations;
	glm::mat4 GlobalInverseTransformation = glm::mat4(0.0f);
	std::map<std::string, int> BoneNameToIdMap;

	std::vector<glm::mat4> NodeHeirarchyTransformations;
	std::map<std::string, int> NodeNameToIdMap;

	std::vector< ModelTexture > ModelTextures;

	//std::vector<NodeNameToId> NodeNameToId;

	// The "local" (i.e. "CPU side" temporary array)
	sVertex* pVertices;	//  = 0;
	sTriangle* pTriangles;

	// The index buffer (CPU side)
	unsigned int* pIndices;	// Triangles

	glm::vec3 maxExtents_XYZ;	// bounding box maximums
	glm::vec3 minExtents_XYZ;	// bounding box minimums
	glm::vec3 deltaExtents_XYZ;	// bounding box dimensions
	float maxExtent;

	void calcExtents(void);

	void FreeMemory(void);

	std::vector<glm::vec3> GetLowestPointsInX();
	std::vector<glm::vec3> GetLowestPointsInY();
	std::vector<glm::vec3> GetLowestPointsInZ();

	float GetYByXAndZ(float xValue, float yValue);

	void ShowVertices();
	void DrawVertices();

	glm::vec3 IsAboveModel(glm::vec3 point, bool& isAbove);

	glm::vec3 GetRandomPointOnSurface();

	// 
	unsigned int getUniqueID(void);
private:
	unsigned int m_UniqueID;
	static const unsigned int FIRST_UNIQUE_ID = 1;
	static unsigned int m_nextUniqueID;
};

