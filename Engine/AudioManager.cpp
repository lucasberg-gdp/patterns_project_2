/**
 * Project:      ConsoleApp
 * File:         AudioManager.cpp
 * Author:       Lucas Berg
 * Date Created: October 15th, 2023
 * Description:  Implements all methods from AudioManager.h
 */
#include "AudioManager.h"
#include <iostream>
#include <glm/vec3.hpp>

void CheckError(FMOD_RESULT result, const char* file, int line)
{
	if (result != FMOD_OK)
	{
		printf("FMOD Error [%d]: '%s' at %d\n", static_cast<int>(result), file, line);
	}
}

void GLMToFMOD(const glm::vec3& in, FMOD_VECTOR& out)
{
	out.x = in.x;
	out.y = in.y;
	out.z = in.z;
}

void FMODToGLM(const FMOD_VECTOR& in, glm::vec3& out)
{
	out.x = in.x;
	out.y = in.y;
	out.z = in.z;
}

AudioManager::AudioManager() :
	m_ChorusPassDSP(nullptr),
	m_DelayPassDSP(nullptr),
	m_DistortionDSP(nullptr),
	m_Geometry(nullptr),
	m_HighPassDSP(nullptr),
	m_LowPassDSP(nullptr),
	m_Result(FMOD_RESULT::FMOD_OK),
	m_ReverbDSP(nullptr)
{

}

AudioManager::~AudioManager()
{

}

void AudioManager::Initialize()
{
	if (m_Initialized)
		return;

	FMOD_RESULT result;
	result = FMOD::System_Create(&m_System);
	if (result != FMOD_OK)
	{
		printf("Failed to create the FMOD System!\n");
		return;
	}

	result = m_System->init(1, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK)
	{
		printf("Failed to initialize the system!\n");
		// Cleanup
		result = m_System->close();
		if (result != FMOD_OK)
		{
			printf("Failed to close system!\n");
		}
		return;
	}

	m_Initialized = true;

}

void AudioManager::Destroy()
{
	if (!m_Initialized)
		return;

	FMOD_RESULT result;

	result = m_Sound->release();
	FMODCheckError(result);

	result = m_System->close();
	FMODCheckError(result);

	result = m_System->release();
	FMODCheckError(result);

	m_Initialized = false;
}

float AudioManager::GetVolume()
{
	float currentVolume = 0.0f;

	FMOD_RESULT result;
	result = m_Channel->getVolume(&currentVolume);
	FMODCheckError(result);
	return currentVolume;
}

bool AudioManager::GetIsLooping()
{
	int numOfLoops;
	FMOD_RESULT result;
	result = m_Channel->getLoopCount(&numOfLoops);
	FMODCheckError(result);
	return (numOfLoops == -1);
}

void AudioManager::LoadSound(const char* file)
{
	if (!m_Initialized)
		return;

	FMOD_RESULT result;

	result = m_System->createSound(file, FMOD_LOOP_NORMAL, 0, &m_Sound);
	if (result != FMOD_OK)
	{
		std::cout << "Failed to load the sound file : " << file << std::endl;
	}
}

void AudioManager::PlayAudio()
{
	if (!m_Initialized)
		return;

	FMOD_RESULT result;
	result = m_System->playSound(m_Sound, 0, false, &m_Channel);
	if (result != FMOD_OK)
	{
		printf("Failed to play the sound!\n");
		// Cleanup
		result = m_Sound->release();
		FMODCheckError(result);

		result = m_System->close();
		FMODCheckError(result);

		result = m_System->release();
		FMODCheckError(result);
		return;
	}
}

void AudioManager::PauseSound()
{
	FMOD_RESULT result;
	result = m_Channel->setPaused(true);
	FMODCheckError(result);
}

void AudioManager::ResumeSound()
{
	FMOD_RESULT result;
	result = m_Channel->setPaused(false);
	FMODCheckError(result);
}

void AudioManager::StopSound()
{
	FMOD_RESULT result;
	result = m_Channel->stop();


	FMODCheckError(result);
}

void AudioManager::SetPitch(float newPitch)
{
	FMOD_RESULT result;
	result = m_Channel->setPitch(newPitch);
	FMODCheckError(result);

}

void AudioManager::SetVolume(float newVolume)
{
	FMOD_RESULT result;
	result = m_Channel->setVolume(newVolume);
	FMODCheckError(result);

}

void AudioManager::SetPan(float newPan)
{
	FMOD_RESULT result;
	result = m_Channel->setPan(newPan);
	FMODCheckError(result);

}

void AudioManager::SetMute(bool isMute)
{
	FMOD_RESULT result;
	result = m_Channel->setMute(isMute);
	FMODCheckError(result);

}

void AudioManager::Update()
{
	if (!m_Initialized)
		return;

	FMOD_RESULT result;
	result = m_System->update();
	if (result != FMOD_OK)
	{
		FMODCheckError(result);
		Destroy();
	}
}

void AudioManager::SetLoop(bool isLooping)
{
	if (isLooping)
	{
		FMOD_RESULT result;
		result = m_Channel->setLoopCount(-1);
		FMODCheckError(result);

		return;
	}

	FMOD_RESULT result;
	result = m_Channel->setLoopCount(0);
	FMODCheckError(result);

}

void AudioManager::Mute()
{
	FMOD_RESULT result;
	result = m_Channel->setMute(true);
	FMODCheckError(result);

}

void AudioManager::Unmute()
{
	FMOD_RESULT result;
	result = m_Channel->setMute(false);
	FMODCheckError(result);
}

// This function loads a media file directly into memory
void AudioManager::Load3DAudio(const char* file)
{
	if (!m_Initialized)
		return;

	if (m_AudioMap.find(file) != m_AudioMap.end())
	{
		printf("AudioManager::LoadAudio() Audio already loaded!\n");
		return;
	}

	m_AudioMap.insert(std::pair<const char*, Audio*>(file, new Audio()));

	FMOD_RESULT result;

	// This call loads our audio file entirely into memory
	result = m_System->createSound(file, FMOD_3D | FMOD_LOOP_NORMAL, nullptr, &m_AudioMap[file]->Audio);
	FMODCheckError(result);
	if (result != FMOD_OK)
	{
		printf("AudioManager::LoadAudio() Failed to load the Audio file: %s\n", file);
		return;
	}

	m_AudioMap[file]->Audio->set3DMinMaxDistance(0.5f, 1000.f);

	printf("AudioManager::LoadAudio(%s): Loaded successful!\n", file);
}

void AudioManager::SetListenerAttributes(const glm::vec3& position,
	const glm::vec3& velocity, const glm::vec3& forward, const glm::vec3& up)
{
	FMOD_VECTOR fmodPosition;
	FMOD_VECTOR fmodVelocity;
	FMOD_VECTOR fmodForward;
	FMOD_VECTOR fmodUp;

	GLMToFMOD(position, fmodPosition);
	GLMToFMOD(velocity, fmodVelocity);
	GLMToFMOD(forward, fmodForward);
	GLMToFMOD(up, fmodUp);

	FMOD_RESULT result = m_System->set3DListenerAttributes(
		0, &fmodPosition, &fmodVelocity, &fmodForward, &fmodUp
	);

	float direct, reverb;
	FMOD_VECTOR origin;
	GLMToFMOD(glm::vec3(0.f), origin);
	m_System->getGeometryOcclusion(&origin, &fmodPosition, &direct, &reverb);
	//printf("ListenerPosition: %.2f, %.2f, %.2f | direct: %.4f, reverb: %.4f\n", position.x, position.y, position.z, direct, reverb);
	FMODCheckError(result);
}

int AudioManager::AddPolygon(float direct, float reverb, bool doublesided,
	const std::vector<glm::vec3>& vertices, const glm::vec3& position)
{
	FMOD_RESULT result;
	int index;

	// Add the polygon
	unsigned int numVertices = (unsigned int)vertices.size();
	FMOD_VECTOR* fmodVertices = (FMOD_VECTOR*)malloc(sizeof(FMOD_VECTOR) * numVertices);
	for (unsigned int i = 0; i < numVertices; i++)
	{
		if (fmodVertices)
		{
			GLMToFMOD(vertices[i], fmodVertices[i]);
		}
		//printf("%.2f, %.2f, %.2f\n", fmodVertices[i].x, fmodVertices[i].y, fmodVertices[i].z);
	}

	result = m_Geometry->addPolygon(direct, reverb, doublesided, numVertices, fmodVertices, &index);
	FMODCheckError(result);

	// Set the position
	FMOD_VECTOR fmodPosition;
	GLMToFMOD(position, fmodPosition);
	result = m_Geometry->setPosition(&fmodPosition);
	FMODCheckError(result);

	glm::vec3 scale(1.f);
	FMOD_VECTOR fmodScale;
	GLMToFMOD(scale, fmodScale);
	result = m_Geometry->setScale(&fmodScale);
	FMODCheckError(result);

	FMOD_VECTOR v;
	glm::vec3 g;
	for (unsigned int i = 0; i < numVertices; i++)
	{
		m_Geometry->getPolygonVertex(index, i, &v);
		FMODToGLM(v, g);
		printf("%.2f, %.2f, %.2f = %.2f, %.2f, %.2f\n", g.x, g.y, g.z,
			vertices[i].x, vertices[i].y, vertices[i].z);
	}

	m_Geometry->setActive(true);

	//int count;
	//m_Geometry->setPolygonAttributes(index, 0.9f, 0.9f, true);

	return index;
}

void AudioManager::GetListenerAttributes(glm::vec3& position,
	glm::vec3& velocity, glm::vec3& forward, glm::vec3& up)
{
	FMOD_VECTOR fmodPosition;
	FMOD_VECTOR fmodVelocity;
	FMOD_VECTOR fmodForward;
	FMOD_VECTOR fmodUp;

	FMOD_RESULT result = m_System->get3DListenerAttributes(
		0, &fmodPosition, &fmodVelocity, &fmodForward, &fmodUp
	);
	FMODCheckError(result);

	FMODToGLM(fmodPosition, position);
	FMODToGLM(fmodVelocity, velocity);
	FMODToGLM(fmodForward, forward);
	FMODToGLM(fmodUp, up);
}