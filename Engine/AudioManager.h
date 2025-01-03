/**
 * Project:      ConsoleApp
 * File:         AudioManager.h
 * Author:       Lucas Berg
 * Date Created: October 15th, 2023
 * Description:  Header for AudioManager class that interacts with FMOD directly
 */
#pragma once

#include <fmod.hpp>
#include <glm/fwd.hpp>
#include <vector>
#include <map>

 // TODO:Move this
void CheckError(FMOD_RESULT result, const char* file, int line);

#define FMODCheckError(result) CheckError(result, __FILE__, __LINE__)

struct Audio
{
	const char* name;
	FMOD::Sound* Audio;
};

struct Channel
{
	const char* name;
	FMOD::Channel* fmodChannel;
	float volume;
	float pitch;
	float pan;
	bool playing;
};

struct DriverInfo
{
	int id;
	char name[256];
	FMOD_GUID guid;
	int systemrate;
	FMOD_SPEAKERMODE speakermode;
	int speakermodechannels;
};

class AudioManager
{
private:
	bool m_Initialized = false;

	std::map<const char*, Audio*> m_AudioMap;
	FMOD::System* m_System = nullptr;
	FMOD::Sound* m_Sound = nullptr;
	FMOD::Channel* m_Channel = nullptr;
	FMOD_RESULT m_Result;
public:
	// Constructor
	AudioManager();

	// Destructor (Gets called on delete)
	~AudioManager();

	void Initialize();
	void Destroy();

	// Getters
	float GetVolume();
	bool GetIsLooping();

	void LoadSound(const char* file);
	void PlayAudio();
	void PauseSound();
	void ResumeSound();
	void StopSound();
	void SetPitch(float newPitch);
	void SetVolume(float newVolume);
	void SetPan(float newPan);
	void SetMute(bool isMute);

	void Update();

	void SetLoop(bool isLooping);
	void Mute();
	void Unmute();

	// 3D Stuff
	void Load3DAudio(const char* file);
	void SetListenerAttributes(const glm::vec3& position, const glm::vec3& velocity,
		const glm::vec3& forward, const glm::vec3& up);

	int AddPolygon(float direct, float reverb, bool doublesided,
		const std::vector<glm::vec3>& vertices, const glm::vec3& position);

	// AudioManager.h ADD THIS FUNCTION
	void GetListenerAttributes(glm::vec3& position, glm::vec3& velocity,
		glm::vec3& forward, glm::vec3& up);

	// 3D
	FMOD::Geometry* m_Geometry;

	// Implemented
	FMOD::DSP* m_ReverbDSP;
	FMOD::DSP* m_HighPassDSP;
	FMOD::DSP* m_LowPassDSP;
	FMOD::DSP* m_DistortionDSP;
	FMOD::DSP* m_ChorusPassDSP;

	// Not implemented
	FMOD::DSP* m_DelayPassDSP;

	// Channel Groups
	//FMOD::ChannelGroup* m_MasterChannelGroup;
	//FMOD::ChannelGroup* m_SFXChannelGroup;
	//FMOD::ChannelGroup* m_MusicChannelGroup;
	std::vector<FMOD::ChannelGroup*> m_ChannelGroupList;

};