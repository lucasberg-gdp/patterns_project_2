/**
 * Project:      ConsoleApp
 * File:         cMediaPlayer.h
 * Author:       Lucas Berg
 * Date Created: October 15th, 2023
 * Description:  Header for cMediaPlayer class that make the link between user and audio manager that interacts with FMOD
 */
#pragma once

//#include "cAudioManager.h"
#include <vector>
#include <string>

#include "AudioManager.h"

class cMediaPlayer
{
private:


	bool m_IsPlaying = false;
	bool m_IsMuted = false;
	bool m_IsLooping = false;
	bool m_IsPaused = false;
	float m_ChangePace = 0.1f;
	float m_CurrentVolume = 1.0f;
	float m_CurrentPitch = 1.0f;
	float m_CurrentPan = 0.0f;
	std::string m_CurrentFile;
	std::vector<std::string> m_FileNames = {};
	AudioManager* m_AudioManager;
public:
	float m_MinimumPitch;
	float m_MaximumPitch;
	float m_MinimumPan;
	float m_MaximumPan;
	float m_MinimumVolume;
	float m_MaximumVolume;


	// Constructor
	cMediaPlayer();

	// Destructor (Gets called on delete)
	~cMediaPlayer();

	void Initialize();
	void LoadFiles();
	void Destroy();
	void Update();

	// Getters
	bool GetIsMuted() const;
	bool GetIsLooping() const;
	float GetVolume() const;
	float GetPitch() const;
	float GetPan() const;
	AudioManager* GetAudioManager();
	std::vector<std::string> GetFileNames();

	// Media player
	void PlayAudio(const char* soundfile);
	void PauseAudio();
	void ResumeAudio();
	void StopAudio();

	// Pitch
	void SetPitch(float pitch);
	void RaisePitchTenPercent();
	void LowerPitchTenPercent();
	void EditPitchMenu();

	// Pan
	void SetPan(float pan);
	void RaisePanTenPercent();
	void LowerPanTenPercent();
	void EditPanMenu();

	// Volume
	void SetVolume(float volume);
	void RaiseVolumeTenPercent();
	void LowerVolumeTenPercent();
	void MuteSound();
	void UnmuteSound();
	void EditVolumeMenu();

	// Looping
	void SetLooping(bool isLooping);

	// Methods
	void AddFileName(std::string fileName);
	void ShowcMediaPlayerSettings();
	void ShowcMediaPlayerControls(std::string soundFile);
};