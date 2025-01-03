#include "cParticleSystem.h"
#include "MathUtils.h"

#include "cVAOManager/cVAOManager.h"
#include "cScene.h"
#include "Engine.h"
#include "LogUtils.h"
#include "cGLComputeEuler.h"
#include "Basic Shader Manager/cShaderManager.h"

extern cShaderManager* shaderManager;
extern cVAOManager* g_pMeshManager;
extern cScene* g_currentScene;
extern Engine engine;
extern GLuint shaderProgramID;
extern unsigned int computeShaderID;

extern cShaderManager::cShader computeShader;

cParticleSystem::cParticleSystem()
{
}

cParticleSystem::~cParticleSystem()
{
}

void cParticleSystem::Init(sEmitterInfo& initialState)
{
	m_EmitterState = initialState;

	// Allocate all the particles now, not when they are needed.
	// ("pool" of particles)
	m_particles.clear();
	m_particles.reserve(initialState.maxNumParticles);

	for (unsigned int count = 0; count != initialState.maxNumParticles; count++)
	{
		cParticle newParticle;
		//newParticle.position = Get

		m_particles.push_back(newParticle);
	}


	return;
}

void cParticleSystem::Explode(unsigned int numParticles, float minVelocity, float maxVelocity)
{
	unsigned int numParticlesCreatedSoFar = 0;
	for (cParticle& particle : m_particles)
	{
		if (numParticlesCreatedSoFar >= numParticles)
		{
			break;
		}

		if (particle.lifetime <= 0.0)
		{
			particle.lifetime = MathUtils::
				GetRandomFloat(m_EmitterState.minLifeTime, m_EmitterState.maxLifeTime);
			;
			particle.position = m_EmitterState.emitterPosition;
			//particle.position += MathUtils::
			//	GetRandomVec3(m_EmitterState.positionOffsetMin, m_EmitterState.positionOffsetMax);

			glm::vec3 randomDirection = MathUtils::GetRandomVec3(glm::vec3(-1.0f), glm::vec3(1.0f));

			particle.velocity = randomDirection * MathUtils::GetRandomFloat(minVelocity, maxVelocity);

			//particle.velocity.x = MathUtils::GetRandomFloat(minVelocity, maxVelocity);
			//particle.velocity.y = MathUtils::GetRandomFloat(minVelocity, maxVelocity);
			//particle.velocity.z = MathUtils::GetRandomFloat(minVelocity, maxVelocity);

			//particle.velocity = MathUtils::
			//	GetRandomVec3(minVelocity, maxVelocity);

			particle.acceleration = m_EmitterState.constantForce;

			numParticlesCreatedSoFar++;
		}
	}
}

void cParticleSystem::Update(double deltaTime)
{
	if (!m_isEnabled)
	{
		return;
	}

	if (deltaTime > MAX_DELTATIME)
	{
		deltaTime = MAX_DELTATIME;
	}

	m_secondElapsedCounter += deltaTime;
	if (m_secondElapsedCounter > 1.0)
	{
		m_SecondElapsedEvent();
		m_secondElapsedCounter -= 1.0;
	}

	unsigned int numParticlesToCreate = MathUtils::
		GetRandomInt(m_EmitterState.minNumParticlesPerFrame,
					 m_EmitterState.maxNumParticlesPerFrame);

	unsigned int numParticlesCreatedSoFar = 0;
	for (cParticle &particle : m_particles)
	{
		if (numParticlesCreatedSoFar >= numParticlesToCreate)
		{
			break;
		}

		if (particle.lifetime <= 0.0)
		{
			particle.lifetime = MathUtils::
				GetRandomFloat(m_EmitterState.minLifeTime, m_EmitterState.maxLifeTime);

			particle.position = m_EmitterState.GetSourcePosition();
			//particle.position = m_EmitterState.emitterPosition;

			particle.position += MathUtils::
				GetRandomVec3(m_EmitterState.positionOffsetMin, m_EmitterState.positionOffsetMax);

			particle.velocity = MathUtils::
				GetRandomVec3(m_EmitterState.initVelocityMin, m_EmitterState.initVelocityMax);

			particle.rotSpeed = MathUtils::
				GetRandomVec3(m_EmitterState.orientationMinDegrees, m_EmitterState.orientationMaxDegrees);


			float scale = MathUtils::
				GetRandomFloat(m_EmitterState.uniformScaleChangeMin, m_EmitterState.uniformScaleChangeMax);

			particle.scaleXYZ = glm::vec3(scale);

			particle.acceleration = m_EmitterState.constantForce;

			numParticlesCreatedSoFar++;
		}
	}

	//UpdateParticlesFromShader(deltaTime);

	for (cParticle& particle : m_particles)
	{
		if (particle.lifetime > 0.0)
		{
			particle.velocity += particle.acceleration * (float)deltaTime;
			particle.position += particle.velocity * (float)deltaTime;
			particle.orientation *= particle.rotSpeed;

			particle.lifetime -= (float)deltaTime;

			float scaleChange = MathUtils::
				GetRandomFloat(m_EmitterState.uniformScaleChangeMin, m_EmitterState.uniformScaleChangeMax);

			particle.scaleXYZ.x += scaleChange;
			particle.scaleXYZ.y += scaleChange;
			particle.scaleXYZ.z += scaleChange;
		}
	}
}

void cParticleSystem::UpdatePosition(glm::vec3 newPosition)
{
	m_EmitterState.emitterPosition = newPosition;
}

void cParticleSystem::UpdateConstantForce(glm::vec3 newForce, bool bUpdateParticlesToo)
{
	m_EmitterState.constantForce = newForce;

	if (bUpdateParticlesToo)
	{
		for (cParticle& particle : m_particles)
		{
			particle.acceleration = m_EmitterState.constantForce;
		}
	}
}

glm::vec3 cParticleSystem::GetPosition(void)
{
	return m_EmitterState.emitterPosition;
}

void cParticleSystem::GetParticleList(std::vector<sParticleRender>& particles)
{
	m_particlesRender.clear();
	particles.clear();

	if (!m_isVisible)
	{
		return;
	}

	for (cParticle& particle : m_particles)
	{
		if (particle.lifetime > 0.0)
		{
			sParticleRender drawnParticle;
			drawnParticle.position = particle.position;
			drawnParticle.orientation = particle.orientation;
			drawnParticle.scaleXYZ = particle.scaleXYZ;

			particles.push_back(drawnParticle);
			m_particlesRender.push_back(drawnParticle);
		}
	}
}

cMesh* cParticleSystem::CreateModelFromParticles()
{
	std::vector<sParticleRender> particles;
	GetParticleList(particles);

	std::string fireModel;

	sModelDrawInfo drawInfo;

	if (m_particlesRender.size() < 3)
	{
		return nullptr;
	}

	unsigned int numberOfVertices = (unsigned int)m_particlesRender.size();

	while (numberOfVertices % 3 != 0)
	{
		--numberOfVertices;
	}

	drawInfo.numberOfVertices = numberOfVertices;
	drawInfo.pVertices = new sVertex[drawInfo.numberOfVertices];

	drawInfo.numberOfIndices = numberOfVertices;
	drawInfo.pIndices = new unsigned int[drawInfo.numberOfIndices];

	for (unsigned int i = 0; i < numberOfVertices; i++)
	{
		sVertex tempVertex;

		tempVertex.x = m_particlesRender[i].position.x;
		tempVertex.y = m_particlesRender[i].position.y;
		tempVertex.z = m_particlesRender[i].position.z;
		tempVertex.w = 1.0f;

		tempVertex.nx = 0.0f;
		tempVertex.ny = 0.0f;
		tempVertex.nz = 0.0f;
		tempVertex.nw = 0.0f;
			
		tempVertex.r = 1.0f;
		tempVertex.g = 0.0f;
		tempVertex.b = 0.0f;
		tempVertex.a = 1.0f;
	
		tempVertex.u = 0.0f;
		tempVertex.v = 0.0f;

		drawInfo.pVertices[i] = tempVertex;

		drawInfo.pIndices[i] = i;
	}

	//std::cout << "All vertices" << std::endl;

	//for (unsigned int i = 0; i < numberOfVertices; i++)
	//{
	//	glm::vec3 vertexPos = glm::vec3(drawInfo.pVertices[i].x, drawInfo.pVertices[i].y, drawInfo.pVertices[i].z);
	//	LogUtils::Vector3Log(vertexPos);
	//}

	//std::cout << "All indices" << std::endl;

	//for (unsigned int i = 0; i < numberOfVertices; i++)
	//{
	//	std::cout << "Indice:" << i << " = " << drawInfo.pIndices[i] << std::endl;
	//}


	drawInfo.meshName = "fire";
	drawInfo.VAO_ID = shaderProgramID;

	sModelDrawInfo drawInfoFromManager;

	if (g_pMeshManager->FindDrawInfoByModelName("fire", drawInfoFromManager))
	{
		g_pMeshManager->UpdateVAOBuffers("fire", drawInfo, shaderProgramID);

		cMesh* mesh = new cMesh();
		mesh->meshName = "fire";

		return mesh;
	}

	// Ask OpenGL for a new buffer ID...
	glGenVertexArrays(1, &(drawInfo.VAO_ID));
	// "Bind" this buffer:
	// - aka "make this the 'current' VAO buffer
	glBindVertexArray(drawInfo.VAO_ID);

	// Now ANY state that is related to vertex or index buffer
	//	and vertex attribute layout, is stored in the 'state' 
	//	of the VAO... 

	// NOTE: OpenGL error checks have been omitted for brevity
	glGenBuffers(1, &(drawInfo.VertexBufferID));

	//	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, drawInfo.VertexBufferID);
	// sVert vertices[3]

	// This is updated for bIsDynamicBuffer so:
	// * if true, then it's GL_DYNAMIC_DRAW
	// * if false, then it's GL_STATIC_DRAW
	// 
	// Honestly, it's not that Big Of A Deal in that you can still update 
	//  a buffer if it's set to STATIC, but in theory this will take longer.
	// Does it really take longer? Who knows?
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(sVertex) * drawInfo.numberOfVertices,	// ::g_NumberOfVertsToDraw,	// sizeof(vertices), 
		(GLvoid*)drawInfo.pVertices,							// pVertices,			//vertices, 
		(GL_STATIC_DRAW));


	// Copy the index buffer into the video card, too
	// Create an index buffer.
	glGenBuffers(1, &(drawInfo.IndexBufferID));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.IndexBufferID);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER,			// Type: Index element array
		sizeof(unsigned int) * drawInfo.numberOfIndices,
		(GLvoid*)drawInfo.pIndices,
		GL_STATIC_DRAW);

	// Set the vertex attributes.

	GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPos");	// program
	GLint vcol_location = glGetAttribLocation(shaderProgramID, "vCol");	// program;
	GLint vNormal_location = glGetAttribLocation(shaderProgramID, "vNormal");	// program;
	// AND the texture coordinate
	GLint vTextureCoords_location = glGetAttribLocation(shaderProgramID, "vTextureCoords");    // in vec2 vTextureCoords;		// NOTE this is a vec2 not vec4;		// NOTE this is a vec2 not vec4

	GLint vBones_Id = glGetAttribLocation(shaderProgramID, "vBoneIds");    // in vec2 vTextureCoords;		// NOTE this is a vec2 not vec4;		// NOTE this is a vec2 not vec4

	GLint vBones_Weights = glGetAttribLocation(shaderProgramID, "vBoneWeights");

	// Set the vertex attributes for this shader
	glEnableVertexAttribArray(vpos_location);	    // vPos
	glVertexAttribPointer(vpos_location, 4,		// vPos
		GL_FLOAT, GL_FALSE,
		sizeof(sVertex),
		(void*)offsetof(sVertex, x));

	glEnableVertexAttribArray(vcol_location);	    // vCol
	glVertexAttribPointer(vcol_location, 4,		// vCol
		GL_FLOAT, GL_FALSE,
		sizeof(sVertex),
		(void*)offsetof(sVertex, r));

	glEnableVertexAttribArray(vNormal_location);	// vNormal
	glVertexAttribPointer(vNormal_location, 4,		// vNormal
		GL_FLOAT, GL_FALSE,
		sizeof(sVertex),
		(void*)offsetof(sVertex, nx));

	glEnableVertexAttribArray(vTextureCoords_location);	    // vTextureCoords;
	glVertexAttribPointer(vTextureCoords_location, 2,		// in vec2 vTextureCoords;
		GL_FLOAT, GL_FALSE,
		sizeof(sVertex),
		(void*)offsetof(sVertex, u));

	glEnableVertexAttribArray(vBones_Id);	    // vTextureCoords;
	glVertexAttribPointer(vBones_Id, 4,		// in vec2 vTextureCoords;
		GL_FLOAT, GL_FALSE,
		sizeof(sVertex),
		(void*)offsetof(sVertex, bones1));

	glEnableVertexAttribArray(vBones_Weights);	    // vTextureCoords;
	glVertexAttribPointer(vBones_Weights, 4,		// in vec2 vTextureCoords;
		GL_FLOAT, GL_FALSE,
		sizeof(sVertex),
		(void*)offsetof(sVertex, boneWeight1));

	// Now that all the parts are set up, set the VAO to zero
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(vpos_location);
	glDisableVertexAttribArray(vcol_location);
	glDisableVertexAttribArray(vNormal_location);
	glDisableVertexAttribArray(vTextureCoords_location);        // <-- New
	glDisableVertexAttribArray(vBones_Id);
	glDisableVertexAttribArray(vBones_Weights);

	// Store the draw information into the map
	
	g_pMeshManager->m_map_ModelName_to_VAOID[drawInfo.meshName] = drawInfo;

	cMesh* finalMesh = new cMesh();

	finalMesh->meshName = "fire";

	return finalMesh;
}

void cParticleSystem::UpdateModelFromParticles()
{

}

void cParticleSystem::m_SecondElapsedEvent(void)
{
	// TODO: Do something?

}

cParticleSystem::sParticleRender cParticleSystem::GetClosestParticle(sParticleRender particle)
{
	std::vector<sParticleRender> particles;
	GetParticleList(particles);

	float closestDistance = 0.0f;

	bool validParticle = false;
	unsigned int index = 0;

	while (!validParticle)
	{
		if (!(particles[index].isSelected))
		{
			validParticle = true;
			closestDistance = glm::distance(particle.position, particles[index].position);
		}
	}

	cParticleSystem::sParticleRender particleSelected;

	int indexSelected = -1;

	for (unsigned int i = 0; i < m_particlesRender.size(); i++)
	{
		if (!(particles[i].isSelected))
		{
			float distance = glm::distance(particles[i].position, particle.position);

			if (closestDistance > distance)
			{
				closestDistance = distance;
				particleSelected = particles[i];
				indexSelected = i;
			}
		}
	}

	if (indexSelected == -1)
	{
		return particle;
	}

	m_particlesRender[indexSelected].isSelected = true;

	return particleSelected;
}

void cParticleSystem::Enable(void)
{
	m_isEnabled = true;
}

void cParticleSystem::Disable(void)
{
	m_isEnabled = false;
}

void cParticleSystem::MakeVisible(void)
{
	m_isVisible = true;
}

void cParticleSystem::MekeInvisible(void)
{
	m_isVisible = false;
}

glm::vec3 cParticleSystem::sEmitterInfo::GetSourcePosition()
{
	switch (sourceType)
	{
	case cParticleSystem::sEmitterInfo::Point:
		return emitterPosition;
	case cParticleSystem::sEmitterInfo::Circular :
		if (isOnlyOnSurface)
		{
			return MathUtils::GetRandomOnLeveledCircle(emitterPosition, radius);
		}
		return MathUtils::GetRandomInLeveledCircle(emitterPosition, radius);
	case cParticleSystem::sEmitterInfo::Square:
		if (isOnlyOnSurface)
		{
			return MathUtils::GetRandomOnSquare(emitterPosition, radius);
		}
		return  MathUtils::GetRandomInSquare(emitterPosition, radius);
	}

	return glm::vec3();
}



void cParticleSystem::UpdateParticlesFromShader(double deltaTime)
{
	//if (this->m_particlesRender.size() == 0)
	//{
	//	return;
	//}

	bool noActiveParticles = true;
	for (cParticleSystem::cParticle particle : m_particles)
	{
		if (particle.lifetime > 0.0f)
		{
			noActiveParticles = false;
		}
	}

	if (noActiveParticles)
	{
		return;
	}

	if (!shaderManager->createProgramFromFile("computeEuler", computeShader))
	{
		std::cout << "Error: Couldn't compile or link:" << computeShader.fileName << std::endl;
		std::cout << shaderManager->getLastError();
	}
	else
	{
		//std::cout << "Compute shader compiled OK" << std::endl;
	}

	GLint floatStringID = glGetUniformLocation(computeShaderID, "deltaTime");
	glUniform1f(floatStringID, (GLfloat)deltaTime);

	//engine.SetUniformFloatInShader("deltaTime", deltaTime);

	cGLComputeEuler physicsObjectsBuffer;

	std::vector<cGLComputeEuler::sPysicsState> vecObjects;
	const unsigned int NUMBER_OF_OBJECTS = 250;

	for (cParticleSystem::cParticle particle : m_particles)
	{
		if (particle.lifetime > 0.0f)
		{
			cGLComputeEuler::sPysicsState newObject;
			newObject.position = glm::vec4(particle.position, 0.0f);
			newObject.velocity = glm::vec4(particle.velocity, 0.0f);
			newObject.acceleration = glm::vec4(particle.acceleration, 0.0f);

			vecObjects.push_back(newObject);
		}
	}

	if (!physicsObjectsBuffer.init(computeShaderID, vecObjects))
	{
		std::cout << "Error: buffer init didn't work" << std::endl;
	}
	::g_ProcessOpenGLError(__FILE__, __LINE__);


	glUseProgram(computeShaderID);

	GLint numberOfActiveUniforms = 0;
	glGetProgramiv(computeShaderID,	// Shader program ID ('name')
		GL_ACTIVE_UNIFORMS,		// Give me how many active uniforms there are
		&numberOfActiveUniforms);


	GLint computeEuler_deltaTime_ID = glGetUniformLocation(computeShaderID, "deltaTime");
	glUniform1f(computeEuler_deltaTime_ID, (GLfloat)deltaTime);


	glDispatchCompute(NUMBER_OF_OBJECTS, 1, 1);
	::g_ProcessOpenGLError(__FILE__, __LINE__);

	// If you want to wait until the compute is done.
	// You don't have to do this, like if you have something else to do, like render, etc.
	// You could also sleep() for a bit?
	// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glMemoryBarrier.xhtml
	// https://learnopengl.com/Guest-Articles/2022/Compute-Shaders/Introduction
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	::g_ProcessOpenGLError(__FILE__, __LINE__);


	std::vector<cGLComputeEuler::sPysicsState> vecObjectsUpdated;
	if (!physicsObjectsBuffer.getData(computeShaderID, vecObjectsUpdated))
	{
		std::cout << "Error buffer getData() didn't work" << std::endl;
	}
	::g_ProcessOpenGLError(__FILE__, __LINE__);

	int objectUpdatedIndex = 0;

	for (unsigned int i = 0; i < vecObjectsUpdated.size(); i++)
	{
		if (m_particles[i].lifetime > 0.0f)
		{
			m_particles[i].position = glm::vec3(
				vecObjectsUpdated[objectUpdatedIndex].position.x, vecObjectsUpdated[objectUpdatedIndex].position.y, vecObjectsUpdated[objectUpdatedIndex].position.z);
			m_particles[i].velocity = glm::vec3(
				vecObjectsUpdated[objectUpdatedIndex].velocity.x, vecObjectsUpdated[objectUpdatedIndex].velocity.y, vecObjectsUpdated[objectUpdatedIndex].velocity.z);

			++objectUpdatedIndex;

			m_particles[i].orientation *= m_particles[i].rotSpeed;

			m_particles[i].lifetime -= (float)deltaTime;

			float scaleChange = MathUtils::
				GetRandomFloat(m_EmitterState.uniformScaleChangeMin, m_EmitterState.uniformScaleChangeMax);

			m_particles[i].scaleXYZ.x += scaleChange;
			m_particles[i].scaleXYZ.y += scaleChange;
			m_particles[i].scaleXYZ.z += scaleChange;
		}

	}

	//		particle.velocity += particle.acceleration * (float)deltaTime;
//		particle.position += particle.velocity * (float)deltaTime;
//		particle.orientation *= particle.rotSpeed;

//		particle.lifetime -= (float)deltaTime;

//		float scaleChange = MathUtils::
//			GetRandomFloat(m_EmitterState.uniformScaleChangeMin, m_EmitterState.uniformScaleChangeMax);

//		particle.scaleXYZ.x += scaleChange;
//		particle.scaleXYZ.y += scaleChange;
//		particle.scaleXYZ.z += scaleChange;

	//LogUtils::Vector3Log("Original pos: ", vecObjects[0].position);
	//LogUtils::Vector3Log("Original vel: ", vecObjects[0].velocity);
	//LogUtils::Vector3Log("New pos: ", vecObjectsUpdated[0].position);
	//LogUtils::Vector3Log("New vel: ", vecObjectsUpdated[0].velocity);
}