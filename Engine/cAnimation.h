#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class cAnimation
{

public:
	enum class EasingType {
		NoEasing,
		EaseIn,
		EaseOut,
		EaseInOut,
	};

	struct PositionKeyFrame
	{
		PositionKeyFrame(const glm::vec3& position, double time, EasingType type = EasingType::NoEasing):
			m_position(position),
			m_time(time),
			m_type(type)
		{}

		glm::vec3 m_position;
		double m_time;
		EasingType m_type;
	};

	struct ScaleKeyFrame
	{
		ScaleKeyFrame(const glm::vec3& scale, double time, EasingType type = EasingType::NoEasing) :
			m_scale(scale),
			m_time(time),
			m_type(type)
		{}

		glm::vec3 m_scale;
		double m_time;
		EasingType m_type;
	};

	struct RotationKeyFrame
	{
		RotationKeyFrame(const glm::quat& position, double time, EasingType type = EasingType::NoEasing) :
			m_rotation(position),
			m_time(time),
			m_type(type)
		{}

		glm::quat m_rotation;
		double m_time;
		EasingType m_type;
	};

	enum EventType
	{
		Destroy,
	};

	struct EventKeyFrame 
	{
		EventKeyFrame(double time, EventType type = EventType::Destroy) :
			m_time(time),
			m_eventType(type)
		{}

		EventType m_eventType;
		double m_time;
	};

	cAnimation(glm::vec3 start, glm::vec3 end, EasingType type);
	cAnimation() {}

	cAnimation(const cAnimation& animation);
	std::vector<PositionKeyFrame> m_positionKeyFrames;
	std::vector<ScaleKeyFrame> m_scaleKeyFrames;
	std::vector<RotationKeyFrame> m_rotationKeyFrames;
	std::vector<EventKeyFrame> m_eventKeyFrames;
	double m_time = 0.0;

	float m_percent = 0.0f;
	bool m_isAnimRunning = false;

	//Position GetEaseIn(float t, Position a, Position b)
	//{

	//}

	//Position GetEaseOut(float t, Position a, Position b)
	//{

	//}

	//Position GetCustomBezier(float t, Position a, Position b)
	//{

	//}


	//void CustomEasing() 
	//{

	//}

	//void DestroyObject();


	struct Animation
	{
		std::vector< PositionKeyFrame > m_positionKeyFrames;
	};

	class AnimationSystem 
	{
	public:

		void Update()
		{
			//EasingType type;

		}
	};
};

