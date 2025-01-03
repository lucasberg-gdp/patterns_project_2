#ifndef _cMesh_HG_
#define _cMesh_HG_

#include <string>
#include <vector>
#include <map>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "Bones.h"
#include "iPhysicsMeshTransformAccess.h"

class cMesh : public iPhysicsMeshTransformAccess
{
private:
	glm::quat m_qOrientation = glm::quat(1.0f,0.0f,0.0f,0.0f);
	glm::mat4 m_rotation;

	unsigned int m_UniqueID;
	static const unsigned int FIRST_UNIQUE_ID = 1000;
	static unsigned int m_nextUniqueID;
public:
	struct Texture 
	{
		Texture(std::string newName, float newRatio): 
			Name(newName),
			Ratio(newRatio)
		{}
		std::string Name;
		float Ratio;
	};

	cMesh();
	cMesh(cMesh* mesh);
	cMesh(std::string objectType);

	~cMesh();

	// Lower resolution
	bool hasBackupModel = false;
	std::string backupModelName;

	// Texture info
	static const int NUM_TEXTURES = 8;
	std::string textureName[NUM_TEXTURES];
	float textureRatios[NUM_TEXTURES];

	// Here is some framebuffer object textures
	bool textureIsFromFBO = false;
	unsigned int FBOTextureNumber0 = 0;
	unsigned int FBOTextureNumber1 = 0;

	bool isUsingVertexColors = false;

	bool bDrawBothSides = false;

	bool isToSave = true;

	bool isDrawBehind = false;

	bool isDrawTimed = false;
	double drawTime = 0.0;

	// flickering
	bool isFlickering = false;
	float flickeringAmount = 0.0f;

	// foliage
	bool isFoliage = false;

	// fire
	bool isFire = false;

	// rain
	bool isRain = false;

	//GLuint UseBonesUL = false;

	bool isHavingStatic = false;
	float staticAmount = 0.0f;

	std::string meshName;

	std::string friendlyName;
	std::string objectType;

	glm::vec3 drawPosition = glm::vec3(0.0f);
	glm::vec3 drawOrientation = glm::vec3(0.0f);
	glm::vec3 drawScale = glm::vec3(1.0f);

	glm::vec3 initialScale  = glm::vec3(1.0f);

	std::map<std::string, int> BoneNameToIdMap;
	Bones::Node* RootNode;
	glm::mat4 GlobalInverseTransformation;

	std::vector<Bones::BoneInfo> BoneInfoVec;
	bool UseBonesUL = false;

	// returns the height of where it crossed
	float IsAboveMesh(glm::vec3 position, bool& isAboveMesh);

	glm::vec3 GetRandomPointOnSurface();

	//cTransform* transform;

	void setRotationFromEuler(glm::vec3 newEulerAngleXYZ)
	{
		drawOrientation = newEulerAngleXYZ;
		this->m_qOrientation = glm::quat(newEulerAngleXYZ);
	}

	void adjustRoationAngleFromEuler(glm::vec3 EulerAngleXYZ_Adjust)
	{
		// To combine quaternion values, you multiply them together
		// Make a quaternion that represents that CHANGE in angle
		glm::quat qChange = glm::quat(EulerAngleXYZ_Adjust);

		// Multiply them together to get the change
		// Just like with matrix math
		this->m_qOrientation *= qChange;

		// Which is exactly the same as:
		//		this->m_qOrientation = this->m_qOrientation * qChange;
	}

	glm::quat GetOrientation(void) const
	{
		return this->m_qOrientation;
	}

	void setUniformDrawScale(float scale);
	void SetScale(glm::vec3 scale);

	// STARTOF: From: iPhysicsMeshTransformAccess interface
	virtual glm::vec3 getDrawPosition(void);
	virtual glm::vec3 getDrawOrientation(void);
	glm::vec3 GetScale() const;
	glm::vec3 GetInitialScale();
	float GetUniformScale() const;

	std::vector<Texture> GetAllTextures();

	virtual void setDrawPosition(const glm::vec3& newPosition);
	virtual void setDrawOrientation(const glm::vec3& newOrientation);
	virtual void setDrawOrientation(const glm::quat& newOrientation);
	virtual void setTransformInfo(glm::vec3 transformInfo);

	virtual void RotateAroundX(float degrees);
	virtual void RotateAroundY(float degrees);
	virtual void RotateAroundZ(float degrees);

	void LookAt(glm::vec3 target);

	glm::mat4 GetRotation();

	virtual std::string GetFriendlyName();
	// ENDOF: iPhysicsMeshTransformAccess interface

	std::string MeshInfo();

	float GetVertexYByXAndZ(float xValue, float zValue);

	bool bIsVisible;

	bool bUseDebugColours;
	glm::vec4 wholeObjectDebugColourRGBA;

	float transparencyAlpha = 1.0f;

	bool bIsWireframe;
	bool bDoNotLight;
	bool bIsScaleUniform = true;

	std::vector<cMesh*> vec_pChildMeshes;

	//bool isFire = false;
	//bool isMap = false;

	//bool isLeftMap = false;
	//bool isRightMap = false;

	unsigned int GetUniqueID(void);
};

#endif