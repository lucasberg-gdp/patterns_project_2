#pragma once

#include <glm/glm.hpp>

#include <stdlib.h>
#include <string>
#include <vector>
#include "cMesh.h"

class MathUtils
{
public:
    static float GetRandomFloat(float a, float b);
	static int GetRandomInt(int a, int b);
	static glm::vec3 GetRandomVec3(const glm::vec3& vector1, const glm::vec3& vector2);

	static glm::vec3 ClosestPoint(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, float& t, bool clamp = true);

	static std::string GetFloatAsPercentage(float number);
	static std::string RoundFloat(float value, int numberOfDigits);

	static glm::vec3 GetRandomOnSurfaceOfSphere(const glm::vec3& position, float radius);

	static std::string GetRandomFromVector(std::vector < std::string>& strings);
	static cMesh* GetRandomMeshFromVector(std::vector < cMesh* >& meshes);
	static std::string GetRandomDifferentFromVector(std::string oldString, std::vector < std::string>& strings);

	// on the surface
	static glm::vec3 GetRandomOnLeveledCircle(glm::vec3 center, float radius);
	static glm::vec3 GetRandomOnSquare(glm::vec3 center, float halfSide);

	// inside + surface
	static glm::vec3 GetRandomInLeveledCircle(glm::vec3 center, float radius);
	static glm::vec3 GetRandomInSquare(glm::vec3 center, float halfSide);
	static glm::vec3 GetRandomInTriangle(const glm::vec3& vecA, const glm::vec3& vecB, const glm::vec3& vecC);

	static glm::vec3 GetCenterOfTriangle(const glm::vec3& vecA, const glm::vec3& vecB, const glm::vec3& vecC);
};

