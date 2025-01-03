#include "sModelDrawInfo.h"

#include <iostream>

#include "../Engine.h"
#include "../cColor.h"
#include "../MathUtils.h"

extern Engine engine;

sModelDrawInfo::sModelDrawInfo()
{
	this->VAO_ID = 0;

	this->VertexBufferID = 0;
	this->VertexBuffer_Start_Index = 0;
	this->numberOfVertices = 0;

	this->IndexBufferID = 0;
	this->IndexBuffer_Start_Index = 0;
	this->numberOfIndices = 0;
	this->numberOfTriangles = 0;

	// The "local" (i.e. "CPU side" temporary array)
	this->pVertices = 0;	// or NULL
	this->pIndices = 0;		// or NULL

	// You could store the max and min values of the 
	//  vertices here (determined when you load them):
	this->maxExtents_XYZ = glm::vec3(0.0f);
	this->minExtents_XYZ = glm::vec3(0.0f);
	this->deltaExtents_XYZ = glm::vec3(0.0f);

	//	scale = 5.0/maxExtent;		-> 5x5x5
	this->maxExtent = 0.0f;


	this->m_UniqueID = sModelDrawInfo::m_nextUniqueID;
	sModelDrawInfo::m_nextUniqueID++;

	return;
}

sModelDrawInfo::~sModelDrawInfo()
{

}

//static 
unsigned int sModelDrawInfo::m_nextUniqueID = sModelDrawInfo::FIRST_UNIQUE_ID;


unsigned int sModelDrawInfo::getUniqueID(void)
{
	return this->m_UniqueID;
}

void sModelDrawInfo::calcExtents(void)
{
	if (this->pVertices == NULL)
	{
		// No vertices to check
		return;
	}

	// Set the 1st vertex as max and min
	sVertex* pCurrentVert = &(this->pVertices[0]);

	this->minExtents_XYZ.x = pCurrentVert->x;
	this->minExtents_XYZ.y = pCurrentVert->y;
	this->minExtents_XYZ.z = pCurrentVert->z;

	this->maxExtents_XYZ.x = pCurrentVert->x;
	this->maxExtents_XYZ.y = pCurrentVert->y;
	this->maxExtents_XYZ.z = pCurrentVert->z;

	for (unsigned int vertIndex = 0; vertIndex != this->numberOfVertices; vertIndex++)
	{
		sVertex* pCurrentVert = &(this->pVertices[vertIndex]);

		if (pCurrentVert->x < this->minExtents_XYZ.x) { this->minExtents_XYZ.x = pCurrentVert->x; }
		if (pCurrentVert->y < this->minExtents_XYZ.y) { this->minExtents_XYZ.y = pCurrentVert->y; }
		if (pCurrentVert->z < this->minExtents_XYZ.z) { this->minExtents_XYZ.z = pCurrentVert->z; }

		if (pCurrentVert->x > this->maxExtents_XYZ.x) { this->maxExtents_XYZ.x = pCurrentVert->x; }
		if (pCurrentVert->y > this->maxExtents_XYZ.y) { this->maxExtents_XYZ.y = pCurrentVert->y; }
		if (pCurrentVert->z > this->maxExtents_XYZ.z) { this->maxExtents_XYZ.z = pCurrentVert->z; }

	}//for ( unsigned int vertIndex = 0...

	// Calculate deltas...
	this->deltaExtents_XYZ.x = this->maxExtents_XYZ.x - this->minExtents_XYZ.x;
	this->deltaExtents_XYZ.y = this->maxExtents_XYZ.y - this->minExtents_XYZ.y;
	this->deltaExtents_XYZ.z = this->maxExtents_XYZ.z - this->minExtents_XYZ.z;

	// Largest extent...
	this->maxExtent = this->deltaExtents_XYZ.x;
	if (this->maxExtent < this->deltaExtents_XYZ.y) { this->maxExtent = this->deltaExtents_XYZ.y; }
	if (this->maxExtent < this->deltaExtents_XYZ.z) { this->maxExtent = this->deltaExtents_XYZ.z; }

	return;
}

void sModelDrawInfo::FreeMemory(void)
{
	if (this->pVertices)
	{
		delete[] this->pVertices;
	}

	if (this->pIndices)
	{
		delete[] this->pIndices;
	}
}

std::vector<glm::vec3> sModelDrawInfo::GetLowestPointsInX()
{
	std::vector<glm::vec3> verticesWithLowestValue;

	sVertex* firstVertex = &(this->pVertices[0]);
	float lowestValue = firstVertex->y;

	for (unsigned int vertIndex = 0; vertIndex != this->numberOfVertices; vertIndex++)
	{
		sVertex* currentVert = &(this->pVertices[vertIndex]);

		if (lowestValue > currentVert->y)
		{
			lowestValue = currentVert->y;
		}
	}

	for (unsigned int vertIndex = 0; vertIndex != this->numberOfVertices; vertIndex++)
	{
		sVertex* currentVert = &(this->pVertices[vertIndex]);

		if (lowestValue == currentVert->y)
		{
			verticesWithLowestValue.push_back(glm::vec3(currentVert->x, currentVert->y, currentVert->z));
		}
	}

	return verticesWithLowestValue;
}

std::vector<glm::vec3> sModelDrawInfo::GetLowestPointsInY()
{
	return std::vector<glm::vec3>();
}

std::vector<glm::vec3> sModelDrawInfo::GetLowestPointsInZ()
{
	return std::vector<glm::vec3>();
}



float DistanceToPlane(const glm::vec3& point, const glm::vec3& planeNormal, const glm::vec3& planePoint)
{
	return glm::dot(planeNormal, point - planePoint);
}

bool IsPointAboveTriangle(const glm::vec3& point, const glm::vec3& vertexA, const glm::vec3& vertexB, const glm::vec3& vertexC)
{
	glm::vec3 normal = glm::normalize(glm::cross(vertexB - vertexA, vertexC - vertexA));

	float distance = DistanceToPlane(point, normal, vertexA);

	if (distance < 0.0f)
	{
		return false;
	}
	
	return true;
}

float sModelDrawInfo::GetYByXAndZ(float xValue, float zValue)
{
	for (unsigned int i = 0; i < 2; i++)
	{
		engine.g_DrawDebugSphere(
			glm::vec3(pTriangles[i].vecA.x, pTriangles[i].vecA.y, pTriangles[i].vecA.z),
			10.0f, cColor::COLOR_BLUE, 0.0f);

		engine.g_DrawDebugSphere(
			glm::vec3(pTriangles[i].vecB.x, pTriangles[i].vecB.y, pTriangles[i].vecB.z),
			10.0f, cColor::COLOR_GREEN, 0.0f);

		engine.g_DrawDebugSphere(
			glm::vec3(pTriangles[i].vecC.x, pTriangles[i].vecC.y, pTriangles[i].vecC.z),
			10.0f, cColor::COLOR_RED, 0.0f);
	}


	//std::vector < glm::vec3> closestPositions;

	//ShowVertices();
	//for (unsigned int i = 0; i < this->numberOfVertices; i++)
	//{
	//	engine.g_DrawDebugSphere(
	//		glm::vec3(pVertices[i].x, pVertices[i].y, pVertices[i].z),
	//		10.0f, cColor::COLOR_BLUE, 0.0f);

	//	float xDistance = glm::distance(xValue, pVertices[i].x);
	//	float zDistance = glm::distance(zValue, pVertices[i].z);

	//	if ((xDistance < 500.0f) && (zDistance < 500.0f))
	//	{
	//		closestPositions.push_back(glm::vec3(pVertices[i].x, pVertices[i].y, pVertices[i].z));

	//		engine.g_DrawDebugSphere(
	//			glm::vec3(xValue, pVertices[i].y, zValue),
	//			100.0f, cColor::COLOR_WHITE, 0.0f);

	//		return pVertices[i].y;
	//	}
	//}

	//if (closestPositions.size() > 0)
	//{
	//	glm::vec3 highestCloseVertice = closestPositions[0];

	//	for (glm::vec3 closePosition : closestPositions)
	//	{
	//		if (highestCloseVertice.y > closePosition.y)
	//		{
	//			highestCloseVertice = closePosition;
	//		}
	//	}

	//	return highestCloseVertice.y;
	//}


	std::cout << "Didn't find the ground in x: " << xValue << " and z: " << zValue << std::endl;

	return 0.0f;
}

void sModelDrawInfo::ShowVertices()
{
	for (unsigned int i = 0; i < this->numberOfVertices; i++)
	{
		std::cout << "Pos:(" << pVertices[i].x << "," << pVertices[i].y << "," << pVertices[i].z << ")" << std::endl;
	}
}

void sModelDrawInfo::DrawVertices()
{
	for (unsigned int i = 0; i < this->numberOfVertices; i++)
	{
		engine.g_DrawDebugSphere(glm::vec3(pVertices[i].x, pVertices[i].y, pVertices[i].z),
			1.0f, cColor::COLOR_GREEN, 0.0f);
	}
}



//glm::vec3 removeScaleAndRotation(const glm::vec3& position, const glm::mat4& transformationMatrix) {
//	// Calculate the inverse of the transformation matrix
//	glm::mat4 inverseTransformationMatrix = glm::inverse(transformationMatrix);
//
//	// Apply the inverse transformation matrix to the position vector
//	glm::vec4 newPosition = inverseTransformationMatrix * glm::vec4(position, 1.0f);
//
//	return glm::vec3(newPosition);
//}

glm::vec3 calculateNormal(const glm::vec3& A, const glm::vec3& B, const glm::vec3& C) 
{
	// Calculate two edges of the triangle
	glm::vec3 edge1 = B - A;
	glm::vec3 edge2 = C - A;

	// Calculate the normal by taking the cross product of the edges
	glm::vec3 normal = glm::cross(edge1, edge2);

	// Make sure the normal is normalized (unit length)
	return glm::normalize(normal);
}

glm::vec3 sModelDrawInfo::IsAboveModel(glm::vec3 point, bool& isAbove)
{
	sTriangle closestTriangle = pTriangles[0];
	float closestDistance = glm::distance(
		MathUtils::GetCenterOfTriangle(closestTriangle.vecA, closestTriangle.vecB, closestTriangle.vecC),
		point);

	for (unsigned int i = 0; i < numberOfTriangles; i++)
	{
		float distance = glm::distance(
			MathUtils::GetCenterOfTriangle(pTriangles[i].vecA, pTriangles[i].vecB, pTriangles[i].vecC),
			point);

		if (distance < closestDistance)
		{
			closestTriangle = pTriangles[i];
			closestDistance = distance;
		}
	}

	if (!IsPointAboveTriangle(point, closestTriangle.vecA, closestTriangle.vecB, closestTriangle.vecC))
	{
		glm::vec3 pointBelow = point;

		bool foundSurface = false;

		while (!foundSurface)
		{
			pointBelow.y += 0.1f;

			foundSurface = IsPointAboveTriangle(pointBelow, closestTriangle.vecA, closestTriangle.vecB, closestTriangle.vecC);
		}

		isAbove = false;
		return pointBelow;
	}

	return glm::vec3(0.0f);

}

glm::vec3 sModelDrawInfo::GetRandomPointOnSurface()
{
	const unsigned randomTriangleIndex = MathUtils::GetRandomInt(0, numberOfTriangles);

	sTriangle randomTriangle = pTriangles[randomTriangleIndex];

	glm::vec3 randomPosition = MathUtils::GetRandomInTriangle(
		randomTriangle.vecA,
		randomTriangle.vecB,
		randomTriangle.vecC);

	return randomPosition;
}
