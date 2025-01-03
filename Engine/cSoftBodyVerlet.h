#pragma once

#include "OpenGLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <string>

#include "cVAOManager/sModelDrawInfo.h"
#include "cMesh.h"

class cSoftBodyVerlet
{

public:

	//double flagWind = 0.0;
	//bool isWindBlowing = false;
	//float windBlowingInitialTimer = 4.0;
	//float windBlowingTimer = windBlowingInitialTimer;

	void Start(GLuint shaderProgramID);
	void Update(double deataTime, GLuint shaderProgramID);

	glm::mat4 initialTransform = glm::mat4(1.0f);
	cSoftBodyVerlet();
	~cSoftBodyVerlet();

	std::string tag;

	bool CreateSoftBody(sModelDrawInfo modelInfo, glm::mat4 matInitalTransform = glm::mat4(1.0f));

	bool CreateSoftBody(cMesh* mesh, glm::mat4 matInitalTransform = glm::mat4(1.0f));

	// Create random constraints within the object
	// These are invisible
	void CreateRandomBracing(unsigned int numberOfBraces,
		float minDistanceBetweenVertices);

	// physics midterm
	void CreateFlagBracing();
	void DisconnectRandomHole();
	void ResetFlagPosition();

	void ActivateAllParticles();

	// Will update the vertex information to match the particles
	void UpdateVertexPositions(void);

	void UpdateNormals(void);

	glm::vec3 GetCenterPoint(void);

	// This will update the draw info vertex information from the
	// soft body simulation state (at the current moment)
	bool UpdateDrawInfo(void);

	void VerletUpdate(double deltaTime);
	void SatisfyConstraints(void);
	void ApplyCollision(double deltaTime);

	void SelectBottomParticles();
	void SelectHinges();

	float GetHighestPointWithBoundsZ(float zValueMin, float zValueMax);

	struct sParticle
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 oldPosition = glm::vec3(0.0f);

		// Pointer back to the model vertex info
		glm::quat orientation = glm::quat(glm::vec3(0.0f));
		glm::quat oldOrientation = glm::quat(glm::vec3(0.0f));

		sVertex* pModelVertex = NULL;

		bool isActive = true;

		bool isBottomParticle = false;
		bool isLeftBackHinge = false;
		bool isRightBackHinge = false;
		bool isLeftFrontHinge = false;

	};

	void CleanZeros(glm::vec3& value);
	float CalcDistanceBetween(sParticle* particleA, sParticle* particleB);

	// These are the "rods" or "springs" between vertices
	// We get these from the mesh, BUT we might need additional

	struct sConstraint
	{
		sConstraint() {}
		sParticle* pParticleA = NULL;
		sParticle* pParticleB = NULL;

		unsigned int numIterations = 1;

		// How far away they are supposed to be 'at rest'
		float restLength = 0.0f;
		bool bIsActive = true;

		bool bIsBlocked = false;
	};

	// Force, like gravity
	glm::vec3 acceleration = glm::vec3(0.0f);

	std::vector<sParticle* > vec_pParticles;
	std::vector<sConstraint* > vec_pConstraints;

	sModelDrawInfo m_modelVertexInfo;

	const double MAX_DELTATIME = 1.0 / 60.0;

	bool isFlagConnected = false;
};