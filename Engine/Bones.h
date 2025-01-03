#pragma once
#include <string>
#include <vector>
#include <glm/mat4x4.hpp> // glm::mat4
#include "cAnimation.h"

class Bones
{
public: 

	// Connection Node for hierarchy
	struct Node
	{
		Node(const std::string& name) : Name(name) { }
		std::string Name;
		glm::mat4 Transformation = glm::mat4(0.0f);
		std::vector<Node*> Children;
	};

	struct NodeAnim
	{
		NodeAnim(const std::string& name) : Name(name) { }
		std::string Name;
		std::vector<cAnimation::PositionKeyFrame> m_PositionKeyFrames;
		std::vector<cAnimation::ScaleKeyFrame> m_ScaleKeyFrames;
		std::vector<cAnimation::RotationKeyFrame> m_RotationKeyFrames;
	};

	struct CharacterAnimation
	{
		CharacterAnimation()
		{
			TicksPerSecond = 60.0;
			Duration = 60.0;
			RootNode = nullptr;
		}

		CharacterAnimation(std::string name)
		{
			Name = name;
			TicksPerSecond = 60.0;
			Duration = 60.0;
			RootNode = nullptr;
		}

		std::string Name;
		double TicksPerSecond;
		double Duration;
		Node* RootNode;
		std::vector<NodeAnim*> Channels;
	};

	struct BoneInfo
	{
		glm::mat4 BoneOffset;				// Offset from the parent bone/node
		glm::mat4 FinalTransformation;		// Calculated transformation used for rendering
		glm::mat4 GlobalTransformation;		// used for the bone hierarchy transformation calculations when animating
	};
};

