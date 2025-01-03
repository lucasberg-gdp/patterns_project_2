#pragma once
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include "cAnimation.h"


class AnimationData
{
public:
	std::string name;
	double duration = 0.0;
	double timer = 0.0;

	std::vector<cAnimation::PositionKeyFrame> m_positionKeyFrames;
	std::vector<cAnimation::ScaleKeyFrame> m_scaleKeyFrames;
	std::vector<cAnimation::RotationKeyFrame> m_rotationKeyFrames;
};

struct AnimationGroup
{
	std::string name;
	std::vector<AnimationData* > animations;
};

