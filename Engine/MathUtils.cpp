#include <sstream>
#include <stdlib.h>

#include "MathUtils.h"
#include <glm/gtc/random.hpp>

float MathUtils::GetRandomFloat(float a, float b)
{
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

int MathUtils::GetRandomInt(int min, int max)
{
    return min + std::rand() % (max - min + 1);
}

glm::vec3 MathUtils::GetRandomVec3(const glm::vec3& vector1, const glm::vec3& vector2)
{
    glm::vec3 randomVec = glm::vec3(0.0f);

    randomVec.x = GetRandomFloat(vector1.x, vector2.x);
    randomVec.y = GetRandomFloat(vector1.y, vector2.y);
    randomVec.z = GetRandomFloat(vector1.z, vector2.z);

    return randomVec;
}

std::string MathUtils::GetFloatAsPercentage(float number)
{
	number = number * 10;
	return std::to_string((int)(round(number) * 10)) + "%";
}

std::string MathUtils::RoundFloat(float value, int numberOfDigits)
{
    std::ostringstream fixedDigitsFloat;
    fixedDigitsFloat.precision(2);

    fixedDigitsFloat << std::fixed << value;

    return fixedDigitsFloat.str();
}

glm::vec3 MathUtils::GetRandomOnSurfaceOfSphere(const glm::vec3& center, float radius)
{
    float u = GetRandomFloat(0.0f, 1.0f);
    float v = GetRandomFloat(0.0f, 1.0f);

    double theta = 2.0f * 3.14 * u;
    double phi = glm::acos(2.0f * v - 1.0f);

    double x = radius * glm::sin(phi) * glm::cos(theta);
    double y = radius * glm::sin(phi) * glm::sin(theta);
    double z = radius * glm::cos(phi);

    return glm::vec3(x, y, z) + center;
}

std::string MathUtils::GetRandomFromVector(std::vector<std::string>& strings)
{
    unsigned int randomIndex = GetRandomInt(0, (unsigned int)strings.size() - 1);
    return strings[randomIndex];
}

std::string MathUtils::GetRandomDifferentFromVector(std::string oldString, std::vector<std::string>& strings)
{
    bool isValid = false;

    unsigned int randomIndex = 0;
    while (!isValid)
    {
        randomIndex = GetRandomInt(0, (unsigned int)strings.size() - 1);
        if (strings[randomIndex] != oldString)
        {
            isValid = true;
        }
    }

    return strings[randomIndex];
}

glm::vec3 MathUtils::GetRandomOnLeveledCircle(glm::vec3 center, float radius)
{
    float x = GetRandomFloat(-1.0f, 1.0f);
    float y = GetRandomFloat(-1.0f, 1.0f);

    glm::vec2 circle = glm::vec2(x , y ) / glm::length(glm::vec2(x, y));
    circle *= radius;

    return glm::vec3(circle.x + center.x, center.y, circle.y + center.z);

}

//glm::vec3 MathUtils::GetRandonOnSquare(glm::vec3 center, float halfSide)
//{
//    return glm::vec3();
//}

cMesh* MathUtils::GetRandomMeshFromVector(std::vector<cMesh*>& meshes)
{
    unsigned int randomIndex = GetRandomInt(0, (unsigned int)meshes.size() - 1);
    return meshes[randomIndex];
}

glm::vec3 MathUtils::GetRandomOnSquare(glm::vec3 center, float halfSide)
{
    int randomSide = GetRandomInt(1, 4);

    float xPos;
    float zPos;

    switch (randomSide)
    {
    case 1:
        xPos = GetRandomFloat(center.x - halfSide, center.x + halfSide);
        zPos = center.z + halfSide;
        break;
    case 2:
        xPos = center.x + halfSide;
        zPos = GetRandomFloat(center.z - halfSide, center.z + halfSide);
        break;
    case 3:
        xPos = GetRandomFloat(center.x - halfSide, center.x + halfSide);
        zPos = center.z - halfSide;;
        break;
    case 4:
        xPos = center.x - halfSide;
        zPos = GetRandomFloat(center.z - halfSide, center.z + halfSide);
        break;
    }

    return glm::vec3(xPos, center.y, zPos);

}

glm::vec3 MathUtils::GetRandomInLeveledCircle(glm::vec3 center, float radius)
{
    float angle = GetRandomFloat(0.0f, glm::two_pi<float>());

    float distance = GetRandomFloat(0.0f, radius);

    float x = center.x + distance * std::cos(angle);
    float y = center.y;
    float z = center.z + distance * std::sin(angle);

    return glm::vec3(x, y, z);
}

glm::vec3 MathUtils::GetRandomInSquare(glm::vec3 center, float halfSide)
{
    return glm::vec3();
}

glm::vec3 MathUtils::GetRandomInTriangle(const glm::vec3& vecA, const glm::vec3& vecB, const glm::vec3& vecC)
{
    float r1 = GetRandomFloat(0.0f, 1.0f);
    float r2 = GetRandomFloat(0.0f, 1.0f);

    if (r1 + r2 > 1.0f)
    {
        r1 = 1.0f - r1;
        r2 = 1.0f - r2;
    }

    float r3 = 1.0f - r1 - r2;
    glm::vec3 randomPoint = r1 * vecA + r2 * vecB + r3 * vecC;

    return randomPoint;
}

glm::vec3 MathUtils::GetCenterOfTriangle(const glm::vec3& vecA, const glm::vec3& vecB, const glm::vec3& vecC)
{
    return (vecA + vecB + vecC) / 3.0f;
}

// Given a line segment ab and a point c, compute the closest point on the line ab
// Also return t, where d(t) = a + t * (b - a)
glm::vec3 MathUtils::ClosestPoint(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, float& t, bool clamp)
{
    // Project c onto ab
    t = glm::dot(c - a, b - a) / glm::dot(b - a, b - a);

    // Clamp t to a 0-1 range. If t > 1 or t < 0 then the closest point is outside the line
    if (clamp)
    {
        t = glm::clamp(t, 0.0f, 1.0f);
    }

    // Compute the projected position from the clamped t
    return glm::vec3(a + t * (b - a));
}
