#pragma once
#include "cGameObject.h"

class Character : public cGameObject
{
    std::vector<glm::mat4> m_BoneTransformations;
};