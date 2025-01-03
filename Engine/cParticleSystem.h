#pragma once

#include "OpenGLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

#include "cMesh.h"

class cParticleSystem
{
public:
	cParticleSystem();
	~cParticleSystem();

	struct sEmitterInfo
	{
		enum SourceType {
			Point,
			Circular,
			Square
		};

		unsigned int maxNumParticles = 0;

		glm::vec3 emitterPosition = glm::vec3(0.0f);
		glm::vec3 positionOffsetMin = glm::vec3(0.0f);
		glm::vec3 positionOffsetMax = glm::vec3(0.0f);

		//TODO: think about a better way to do this. Particles per second
		unsigned int minNumParticlesPerFrame = 0;
		unsigned int maxNumParticlesPerFrame = 0;
		float minLifeTime = 0.0f;
		float maxLifeTime = 0.0f;
		glm::vec3 initVelocityMin = glm::vec3(0.0f);
		glm::vec3 initVelocityMax = glm::vec3(0.0f);

		glm::vec3 orientationChangeMinRadians = glm::vec3(0.0f);
		glm::vec3 orientationChangeMaxRadians = glm::vec3(0.0f);

		glm::vec3 orientationMinDegrees = glm::vec3(0.0f);
		glm::vec3 orientationMaxDegrees = glm::vec3(0.0f);

		bool isUsingUniformScale = false;
		float uniformScaleMin = 1.0f;
		float uniformScaleChangeMin = 0.0f;
		float uniformScaleChangeMax = 0.0f;

		//glm::vec3 scaleChangeMin = glm::vec3(0.0f);
		//glm::vec3 scaleChangeMax = glm::vec3(0.0f);

		glm::vec3 constantForce = glm::vec3(0.0f);

		SourceType sourceType = SourceType::Point;

		// half length for the square source
		float radius = 0.0f;
		bool isOnlyOnSurface = false;

		bool isSelected = false;

		glm::vec3 GetSourcePosition();
	};

	void Init(sEmitterInfo& emitterInfo);

	void Explode(unsigned int numParticles, float minVelocity, float maxVelocity);

	void Enable(void);
	void Disable(void);

	void MakeVisible(void);
	void MekeInvisible(void);

	void Update(double deltaTime);
	void UpdatePosition(glm::vec3 newPosition);
	void UpdateConstantForce(glm::vec3 newForce, bool bUpdateParticlesToo = false);

	glm::vec3 GetPosition(void);

	struct sParticleRender
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::quat orientation = glm::quat(glm::vec3(0.0f));
		glm::vec3 scaleXYZ = glm::vec3(1.0f);

		bool isSelected = false;
	};
	void GetParticleList(std::vector< sParticleRender> &particles);

	cMesh* CreateModelFromParticles();

	void UpdateModelFromParticles();

	void UpdateParticlesFromShader(double deltaTime);

	std::vector< sParticleRender> m_particlesRender;


private:
	class cParticle
	{
	public:
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 velocity = glm::vec3(0.0f);
		glm::vec3 acceleration = glm::vec3(0.0f);

		float lifetime = 0.0f;

		glm::quat orientation = glm::quat(glm::vec3(0.0f));
		glm::quat rotSpeed = glm::quat(glm::vec3(0.0f));

		glm::vec3 scaleXYZ = glm::vec3(0.0f);
	};

	bool m_isEnabled = false;
	bool m_isVisible = false;

	std::vector<cParticle> m_particles;
	sEmitterInfo m_EmitterState;

	double m_secondElapsedCounter = 0.0;

	void m_SecondElapsedEvent(void);

	const double MAX_DELTATIME = 1.0 / 60.0;

public:
	sParticleRender GetClosestParticle(sParticleRender particle);

};

