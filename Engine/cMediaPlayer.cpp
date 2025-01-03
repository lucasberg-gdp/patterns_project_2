/**
 * Project:      ConsoleApp
 * File:         cMediaPlayer.cpp
 * Author:       Lucas Berg
 * Date Created: October 15th, 2023
 * Description:  Implements all methods from cMediaPlayer.h
 */
#include "cMediaPlayer.h"

#include "MathUtils.h"
#include <conio.h>
#include <iostream>
#include <fstream>
#include "cCamera.h"
#include "cGameManager.h"
#include "WindowsUtils.h"

extern cMediaPlayer* g_mediaPlayer;
extern iCamera* g_camera;

std::string GetFriendlyName(std::string fileName, std::string extension);

cMediaPlayer::cMediaPlayer() :
	m_MinimumPitch(0.5f),
	m_MaximumPitch(2.0f),
	m_MinimumPan(1.0f),
	m_MaximumPan(1.0f),
	m_MinimumVolume(0.0f),
	m_MaximumVolume(1.0f),
	m_AudioManager(nullptr)
{

}

cMediaPlayer::~cMediaPlayer()
{

}

void cMediaPlayer::Initialize()
{
	m_AudioManager = new AudioManager();
	m_AudioManager->Initialize();
	m_CurrentVolume = 1.0f;
	m_IsLooping = false;

	LoadFiles();
}

void cMediaPlayer::LoadFiles()
{
	std::string numberOfFilesText;

	std::vector<std::string> fileNames;
	WindowsUtils::GetAllFilesFromFolder("audio", fileNames);

	for (std::string fileName : fileNames)
	{
		AddFileName(fileName);
	}


	//std::ifstream MyReadFile("audio/audioFiles.txt");



	//getline(MyReadFile, numberOfFilesText);

	//int numberOfFiles = stoi(numberOfFilesText);

	//for (int i = 0; i < numberOfFiles; i++)
	//{
	//	std::string fileName;
	//	getline(MyReadFile, fileName);
	//	::g_mediaPlayer->AddFileName(fileName);
	//}
}

void cMediaPlayer::Destroy()
{

}

void cMediaPlayer::Update()
{
	// 1. Add this
	m_AudioManager->SetListenerAttributes(
		::g_camera->GetPosition(),
		glm::vec3(0.f),	// ignore velocity for now
		g_camera->GetOrientation()* g_camera->GetForward(), //glm::vec3(0, 0, -1)
		g_camera->GetUp()	// glm::vec3(0,1,0)
	);

	//float randomChanceOfAnimalSound = MathUtils::GetRandomFloat(0.0f, 1.0f);

	//if (randomChanceOfAnimalSound <= 0.01f)
	//{
	//	m_AudioManager->Load3DAudio("audio/lion.wav");
	//	m_AudioManager->PlaySound();
	//}

	//m_AudioManager->Update();	// 2. Add this
}

AudioManager* cMediaPlayer::GetAudioManager()
{
	return m_AudioManager;
}

bool cMediaPlayer::GetIsMuted() const
{
	return m_IsMuted;
}

bool cMediaPlayer::GetIsLooping() const
{
	return m_IsLooping;
}

std::vector<std::string> cMediaPlayer::GetFileNames()
{
	return m_FileNames;
}

float cMediaPlayer::GetVolume() const
{
	return m_CurrentVolume;
}

float cMediaPlayer::GetPitch() const
{
	return m_CurrentPitch;
}

float cMediaPlayer::GetPan() const
{
	return m_CurrentPan;
}

void cMediaPlayer::PlayAudio(const char* soundFile)
{
	m_CurrentFile = std::string(soundFile);
	bool isChangingVolume = false;
	bool isChangingPitch = false;
	bool isChangingPan = false;

	std::vector<std::string> fileNames = GetFileNames();

	for (std::string fileName : fileNames)
	{
		if (fileName == soundFile)
		{
			std::string filePath = "audio/" + std::string(soundFile);
			m_AudioManager->LoadSound(filePath.c_str());
			m_AudioManager->PlayAudio();
		}
	}
}

void cMediaPlayer::PauseAudio()
{
	m_AudioManager->PauseSound();
	m_IsPaused = true;
}

void cMediaPlayer::ResumeAudio()
{
	m_AudioManager->ResumeSound();
	m_IsPaused = false;
}

void cMediaPlayer::StopAudio()
{
	m_AudioManager->StopSound();
}

void cMediaPlayer::SetPitch(float pitch)
{
	if (pitch < m_MinimumPitch || pitch > m_MaximumPitch)
	{
		return;
	}

	m_AudioManager->SetPitch(pitch);
}

void cMediaPlayer::RaisePitchTenPercent()
{
	float currentPitch = m_CurrentPitch;
	if (currentPitch >= m_MaximumPitch)
	{
		std::cout << "pitch: " << MathUtils::GetFloatAsPercentage(currentPitch) << " -- MAXIMUM PITCH" << std::endl;
		return;
	}

	std::cout << "old pitch: " << MathUtils::GetFloatAsPercentage(currentPitch) << " -- ";
	currentPitch += m_ChangePace;
	m_AudioManager->SetPitch(currentPitch);
	std::cout << "new pitch: " << MathUtils::GetFloatAsPercentage(currentPitch) << std::endl;
	m_CurrentPitch = currentPitch;
}

void cMediaPlayer::LowerPitchTenPercent()
{
	float currentPitch = m_CurrentPitch;
	if (currentPitch <= m_MinimumPitch)
	{
		std::cout << "pitch: " << MathUtils::GetFloatAsPercentage(currentPitch) << " -- MINIMUM PITCH" << std::endl;
		return;
	}

	std::cout << "old pitch: " << MathUtils::GetFloatAsPercentage(currentPitch) << " -- ";
	currentPitch -= m_ChangePace;
	m_AudioManager->SetPitch(currentPitch);
	std::cout << "new pitch: " << MathUtils::GetFloatAsPercentage(currentPitch) << std::endl;
	m_CurrentPitch = currentPitch;
}

void cMediaPlayer::EditPitchMenu()
{
	std::cout << "press '+' or '-' to adjust pitch and press 'p' when done changing pitch" << std::endl;

	bool isChangingPitch = true;
	int key = 0;

	while (isChangingPitch)
	{
		if (_kbhit())
		{
			key = _getch();

			if (key == (int)'+')
			{
				RaisePitchTenPercent();
			}

			if (key == (int)'-')
			{
				LowerPitchTenPercent();
			}

			if (key == (int)'p')
			{
				isChangingPitch = false;
				ShowcMediaPlayerControls(m_CurrentFile);
				ShowcMediaPlayerSettings();
			}
		}
	}
}

void cMediaPlayer::SetPan(float pan)
{
	if (pan < m_MinimumPan || pan > m_MaximumPan)
	{
		return;
	}

	m_AudioManager->SetPan(pan);
}

void cMediaPlayer::RaisePanTenPercent()
{
	float currentPan = m_CurrentPan;
	if (currentPan >= m_MaximumPan)
	{
		std::cout << "pan: " << MathUtils::GetFloatAsPercentage(currentPan) << " -- MAXIMUM PAN" << std::endl;
		return;
	}

	std::cout << "old pan: " << MathUtils::GetFloatAsPercentage(currentPan) << " -- ";
	currentPan += m_ChangePace;
	m_AudioManager->SetPan(currentPan);
	std::cout << "new pan: " << MathUtils::GetFloatAsPercentage(currentPan) << std::endl;
	m_CurrentPan = currentPan;
}

void cMediaPlayer::LowerPanTenPercent()
{
	float currentPan = m_CurrentPan;
	if (currentPan <= m_MinimumPan)
	{
		std::cout << "pan: " << MathUtils::GetFloatAsPercentage(currentPan) << " -- MINIMUM PAN" << std::endl;
		return;
	}

	std::cout << "old pan: " << MathUtils::GetFloatAsPercentage(currentPan) << " -- ";
	currentPan -= m_ChangePace;
	m_AudioManager->SetPan(currentPan);
	std::cout << "new pan: " << MathUtils::GetFloatAsPercentage(currentPan) << std::endl;
	m_CurrentPan = currentPan;
}

void cMediaPlayer::EditPanMenu()
{
	std::cout << "press '+' or '-' to adjust pan and press 'n' when done changing pan" << std::endl;

	bool isChangingPan = true;
	int key = 0;

	while (isChangingPan)
	{
		if (_kbhit())
		{
			key = _getch();

			if (key == (int)'+')
			{
				RaisePanTenPercent();
			}

			if (key == (int)'-')
			{
				LowerPanTenPercent();
			}

			if (key == (int)'n')
			{
				isChangingPan = false;
				ShowcMediaPlayerControls(m_CurrentFile);
				ShowcMediaPlayerSettings();
			}
		}
	}
}

void cMediaPlayer::SetVolume(float volume)
{
	if (volume < m_MinimumVolume || volume > m_MaximumVolume)
	{
		return;
	}

	m_AudioManager->SetVolume(volume);
}

void cMediaPlayer::RaiseVolumeTenPercent()
{
	float currentVolume = m_CurrentVolume;
	if (currentVolume >= m_MaximumVolume)
	{
		std::cout << "volume: " << MathUtils::GetFloatAsPercentage(currentVolume) << " -- MAXIMUM VOLUME" << std::endl;
		return;
	}

	std::cout << "old volume: " << MathUtils::GetFloatAsPercentage(currentVolume) << " -- ";
	currentVolume += m_ChangePace;
	m_AudioManager->SetVolume(currentVolume);
	std::cout << "new volume: " << MathUtils::GetFloatAsPercentage(currentVolume) << std::endl;
	m_CurrentVolume = currentVolume;
}

void cMediaPlayer::LowerVolumeTenPercent()
{
	float currentVolume = m_CurrentVolume;
	if (currentVolume <= m_MinimumVolume)
	{
		std::cout << "volume: " << MathUtils::GetFloatAsPercentage(currentVolume) << " -- MINIMUM VOLUME" << std::endl;
		return;
	}

	std::cout << "old volume: " << MathUtils::GetFloatAsPercentage(currentVolume) << " -- ";
	currentVolume -= m_ChangePace;
	m_AudioManager->SetVolume(currentVolume);
	std::cout << "new volume: " << MathUtils::GetFloatAsPercentage(currentVolume) << std::endl;
	m_CurrentVolume = currentVolume;
}

void cMediaPlayer::MuteSound()
{
	m_AudioManager->Mute();
	m_IsMuted = true;
}

void cMediaPlayer::UnmuteSound()
{
	m_AudioManager->Unmute();
	m_IsMuted = false;
}

void cMediaPlayer::EditVolumeMenu()
{
	std::cout << "press '+' or '-' to adjust volume and press 'v' when done changing volume" << std::endl;

	bool isChangingVolume = true;
	int key = 0;

	while (isChangingVolume)
	{
		if (_kbhit())
		{
			key = _getch();

			if (key == (int)'+')
			{
				RaiseVolumeTenPercent();
			}

			if (key == (int)'-')
			{
				LowerVolumeTenPercent();
			}

			if (key == (int)'v')
			{
				isChangingVolume = false;
				ShowcMediaPlayerControls(m_CurrentFile);
				ShowcMediaPlayerSettings();
			}
		}
	}
}

void cMediaPlayer::SetLooping(bool isLooping)
{
	m_IsLooping = isLooping;
	m_AudioManager->SetLoop(isLooping);
}

void cMediaPlayer::AddFileName(std::string fileName)
{
	m_FileNames.push_back(fileName);
}

void cMediaPlayer::ShowcMediaPlayerSettings()
{
	std::cout << "--------------- Settings ----------------" << std::endl;
	std::cout << "looping = " << (m_IsLooping ? "true" : "false") << std::endl;
	std::cout << "muted = " << (m_IsMuted ? "true" : "false") << std::endl;
	std::cout << "volume = " << MathUtils::GetFloatAsPercentage(m_CurrentVolume) << std::endl;
	std::cout << "pitch = " << MathUtils::GetFloatAsPercentage(m_CurrentPitch) << std::endl;
	std::cout << "pan = " << MathUtils::GetFloatAsPercentage(m_CurrentPan) << std::endl;
}

void cMediaPlayer::ShowcMediaPlayerControls(std::string soundFile)
{
	std::string songFriendlyName = GetFriendlyName(std::string(soundFile), ".wav");
	std::cout << "--------------- Controls ----------------" << std::endl;
	std::cout << songFriendlyName << " playing..." << std::endl;
	std::cout << "ESC - back to menu" << std::endl;
	std::cout << "SPACE - pause/resume" << std::endl;
	std::cout << "m - mute/unmute" << std::endl;
	std::cout << "r - replay" << std::endl;
	std::cout << "s - stop" << std::endl;
	std::cout << "l - switch looping" << std::endl;
	std::cout << "v - volume" << std::endl;
	std::cout << "p - pitch" << std::endl;
	std::cout << "n - pan" << std::endl;
}

std::string GetFriendlyName(std::string fileName, std::string extension)
{
	size_t forwardSlashPosition = fileName.find("/");
	size_t friendlyNameStartingPosition = forwardSlashPosition + 1;
	size_t extensionPosition = fileName.find(extension);

	return fileName.substr(friendlyNameStartingPosition, (fileName.length() - friendlyNameStartingPosition - extension.length()));
}