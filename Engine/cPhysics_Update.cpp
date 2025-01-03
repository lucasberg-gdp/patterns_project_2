#include "cPhysics.h"
#include <iostream>
#include "cGameManager.h"

extern cVAOManager* g_pMeshManager;

glm::vec3 ClosestPtPointTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c);

bool TestSphereTriangle(float sphereRadius, glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2, glm::vec3 sphereCentre);

// HACK:
void g_DrawDebugSphere(glm::vec3 position, float scale, glm::vec4 colourRGBA);


void cPhysics::Update(double deltaTime)
{
//	for ( unsigned int index = 0; index !=  this->m_vec_pPhysicalProps.size(); index++ )
//	{
//		sPhysicsProperties* pObject = this->m_vec_pPhysicalProps[index];
//		
//		// ...and so on...
//
//	}

	// Perform the iteration loop
	for (sPhysicsProperties* pObject : this->m_vec_pPhysicalProps)
	{
		// Infinite mass? 
		if (pObject->inverse_mass >= 0.0f)
		{
			// Explicit forward Euler "integration step"
			//		NewVelocity = LastVel + (Accel * DeltaTime)
			//		NewPosition = LastPos + (Vel * DeltaTime)	

			// Update the "old" position
			pObject->oldPosition = pObject->position;

			// Velocity change is based on the acceleration over this time frame 
			// This part: (Accel * DeltaTime)
			glm::vec3 deltaVelocityThisFrame = pObject->acceleration * (float)deltaTime;

			// Update the velocity based on this delta velocity
			// Then this part: NewVelocity = LastVel + ...
			pObject->velocity += deltaVelocityThisFrame;

			// Max velocity
			float velocityLength = glm::length(pObject->velocity);
			if (velocityLength > pObject->maxVelocity)
			{
				pObject->velocity = glm::normalize(pObject->velocity) * pObject->maxVelocity;
			}


			// Position change is based on the velocity over this time frame
			// This part: (Vel * DeltaTime)	
			glm::vec3 deltaPosition = pObject->velocity * (float)deltaTime;

			// ...then this part: NewPosition = LastPos + ...
			// Upatate the position based on this delta position
	//		pCurrentMesh->pPhysProps->position += deltaPosition;
			pObject->position.x += deltaPosition.x;
			pObject->position.y += deltaPosition.y;
			pObject->position.z += deltaPosition.z;


			// Update orientation
			// Update the "old" orientation
			pObject->m_oldOrientation = pObject->get_qOrientation();

			// Velocity change is based on the acceleration over this time frame 
			// This part: (Accel * DeltaTime)
			glm::vec3 deltaRotSpeedThisFrame = pObject->rotationAcceleration * (float)deltaTime;

			// Update the velocity based on this delta velocity
			// Then this part: NewVelocity = LastVel + ...
			pObject->rotationSpeed += deltaRotSpeedThisFrame;

			glm::vec3 deltaOrientation = pObject->rotationSpeed * (float)deltaTime;

			// ...then this part: NewPosition = LastPos + ...
			// Upatate the position based on this delta position
	//		pCurrentMesh->pPhysProps->position += deltaPosition;
			pObject->eulerOrientation.x += deltaOrientation.x;
			pObject->eulerOrientation.y += deltaOrientation.y;
			pObject->eulerOrientation.z += deltaOrientation.z;

			if (pObject->eulerOrientation.x >= 360.0f)
			{
				pObject->eulerOrientation.x -= 360.0f;
			}
			if (pObject->eulerOrientation.y >= 360.0f)
			{
				pObject->eulerOrientation.y -= 360.0f;
			}
			if (pObject->eulerOrientation.z >= 360.0f)
			{
				pObject->eulerOrientation.z -= 360.0f;
			}


			pObject->setRotationFromEuler(pObject->eulerOrientation);
		}

	}

	// See which object is colliding with which object...
	for (sPhysicsProperties* pObjectA : this->m_vec_pPhysicalProps )
	{
		
		for (sPhysicsProperties* pObjectB : this->m_vec_pPhysicalProps )
		{
			// Are "A" and "B" the same object
			if (pObjectA == pObjectB)
			{
				// Yup, so skip this
				continue;
			}

// We could do the matrix this way...
//			if ((pObjectA->shapeType == sPhysicsProperties::SPHERE) &&
//				(pObjectA->shapeType == sPhysicsProperties::PLANE))
//			{
//
//			}// Sphere - Plane
//
//			if ((pObjectA->shapeType == sPhysicsProperties::SPHERE) &&
//				(pObjectA->shapeType == sPhysicsProperties::SPHERE))
//			{
//
//			}// Sphere - Sphere

			// What's the collision? 
			switch (pObjectA->shapeType)
			{
			case sPhysicsProperties::SPHERE:
				switch (pObjectB->shapeType)
				{
				case sPhysicsProperties::SPHERE:
					// Sphere - Sphere
					if (this->m_Sphere_Sphere_IntersectionTest(pObjectA, pObjectB))
					{
						//std::cout << "Sphere:" << pObjectA->friendlyName 
						//	<< " collided with " << 
						//	"Sphere:" << pObjectB->friendlyName << std::endl;
					}

					break;
				case sPhysicsProperties::PLANE:
					// Shpere - Plane
					break;
				case sPhysicsProperties::TRIANGLE:
					// Sphere - Triangle
					break;
				case sPhysicsProperties::AABB:
					// Sphere - AABB
					break;
				case sPhysicsProperties::CAPSULE:
					// Sphere - Capsule
					break;
				case sPhysicsProperties::MESH_OF_TRIANGLES_INDIRECT:
					// Sphere - Mesh triangle (indirect)
					if ( this->m_Sphere_TriMeshIndirect_IntersectionTest( pObjectA, pObjectB))
					{
						//std::cout << "Hazzah!" << std::endl;
					}
					break;
				case sPhysicsProperties::MESH_OF_TRIANGLES_LOCAL_VERTICES:
					// Shpere - Mesh (local vertices)
					break;
				}//switch (pObjectB->shapeType)
				break;

//			case sPhysicsProperties::PLANE:
//				break;
//			case sPhysicsProperties::TRIANGLE:
//				break;
// ??			case sPhysicsProperties::AABB:
// ??			break;

			case sPhysicsProperties::CAPSULE:
				switch (pObjectB->shapeType)
				{
				case sPhysicsProperties::SPHERE:
					// Capsule - Sphere
					break;
				case sPhysicsProperties::PLANE:
					// Capsule - Plane
					break;
				case sPhysicsProperties::TRIANGLE:
					// Capsule - Triangle
					break;
				case sPhysicsProperties::AABB:
					// Capsule - AABB
					break;
				case sPhysicsProperties::CAPSULE:
					// Capsule - Capsule
					break;
				case sPhysicsProperties::MESH_OF_TRIANGLES_INDIRECT:
					// Capsule - Mesh triangle (indirect)
					break;
				case sPhysicsProperties::MESH_OF_TRIANGLES_LOCAL_VERTICES:
					// Capsule - Mesh (local vertices)
					break;
				}//switch (pObjectB->shapeType)
				break;

//			case sPhysicsProperties::MESH_OF_TRIANGLES_INDIRECT:
//				switch (pObjectB->shapeType)
//				{
//				case sPhysicsProperties::SPHERE:
//					break;
//				case sPhysicsProperties::PLANE:
//					break;
//				case sPhysicsProperties::TRIANGLE:
//					break;
//				case sPhysicsProperties::AABB:
//					break;
//				case sPhysicsProperties::CAPSULE:
//					break;
//				case sPhysicsProperties::MESH_OF_TRIANGLES_INDIRECT:
//					break;
//				case sPhysicsProperties::MESH_OF_TRIANGLES_LOCAL_VERTICES:
//					break;
//				}//switch (pObjectB->shapeType)
//				break;
//
//			case sPhysicsProperties::MESH_OF_TRIANGLES_LOCAL_VERTICES:
//				switch (pObjectB->shapeType)
//				{
//				case sPhysicsProperties::SPHERE:
//					break;
//				case sPhysicsProperties::PLANE:
//					break;
//				case sPhysicsProperties::TRIANGLE:
//					break;
//				case sPhysicsProperties::AABB:
//					break;
//				case sPhysicsProperties::CAPSULE:
//					break;
//				case sPhysicsProperties::MESH_OF_TRIANGLES_INDIRECT:
//					break;
//				case sPhysicsProperties::MESH_OF_TRIANGLES_LOCAL_VERTICES:
//					break;
//				}//switch (pObjectB->shapeType)
//				break;
// 
			//UNKNOWN_OR_UNDEFINED

			}//switch (pObjectA->shapeType)

		}//for (sPhysicsProperties* pObjectB

	}//for (sPhysicsProperties* pObjectA...


	// Upadate the draw locations (and orientations) for all assocuated meshes
	for (sPhysicsProperties* pObject : this->m_vec_pPhysicalProps)
	{
		if (pObject->pTheAssociatedMesh)
		{
			pObject->pTheAssociatedMesh->setDrawPosition(pObject->position);
//			pObject->pTheAssociatedMesh->setDrawOrientation(pObject->orientation);
			pObject->pTheAssociatedMesh->setDrawOrientation(pObject->get_qOrientation());
		}
	}//for (sPhysicsProperties* pObjectA


	// HACK:
	if ( ! this->m_vecCollisionsThisFrame.empty())
	{
		//std::cout << "BREAK ME!" << std::endl;
		
		//std::cout << this->m_vecCollisionsThisFrame.size() << std::endl;
		for (sCollisionEvent col:  this->m_vecCollisionsThisFrame)
		{
			//std::cout
			//	<< col.pObjectA->getShapeTypeAsString()
			//	<< " hit "
			//	<< col.pObjectB->getShapeTypeAsString()
			//	<< std::endl;

			if (col.pObjectA->friendlyName == "camera" && col.pObjectB->friendlyName == "snowground")
			{
				if (col.pObjectA->acceleration.y < 0)
				{
					col.pObjectA->acceleration.y = col.pObjectA->acceleration.y + (9.81f / 5.0f);;
				}

				col.pObjectA->velocity.y = 0.0f;

			}
		}
		//std::cout << "******************************************" << std::endl;
		// HACK:
		this->m_vecCollisionsThisFrame.clear();
	}

	return;
}

void cPhysics::DoPhysicUpdate(double deltaTime)
{

	//// Find the debug sphere
	//cMesh* pDebugSphere = findObjectByFriendlyName("DEBUG_SPHERE");


//	for (unsigned int meshIndex = 0; meshIndex != ::g_vec_pMeshesToDraw.size(); meshIndex++)
	for (unsigned int meshIndex = 0; meshIndex != m_vec_pPhysicalProps.size(); meshIndex++)
	{
		sPhysicsProperties* pCurrentPhysObject = findShapeByVectorID(meshIndex);

		//		if (pCurrentMesh->pPhysProps == NULL )
		//		{
		//			// Skip this.
		//			continue;
		//		}

		// ***********************************************************************
				// Explicit forward Euler "integration step"

		//		NewVelocity = LastVel + (Accel * DeltaTime)
		//		NewPosition = LastPos + (Vel * DeltaTime)	

				// Velocity change is based on the acceleration over this time frame 
		//		// This part: (Accel * DeltaTime)
		glm::vec3 deltaVelocityThisFrame = pCurrentPhysObject->acceleration * (float)deltaTime;

		// Update the velocity based on this delta velocity
		// Then this part: NewVelocity = LastVel + ...
		pCurrentPhysObject->velocity += deltaVelocityThisFrame;


		// Position change is based on the velocity over this time frame
		// This part: (Vel * DeltaTime)	
		glm::vec3 deltaPosition = pCurrentPhysObject->velocity * (float)deltaTime;

		// ...then this part: NewPosition = LastPos + ...
		// Upatate the position based on this delta position
//		pCurrentMesh->pPhysProps->position += deltaPosition;
		pCurrentPhysObject->position.x += deltaPosition.x;
		pCurrentPhysObject->position.y += deltaPosition.y;
		pCurrentPhysObject->position.z += deltaPosition.z;
		// ***********************************************************************


		// ***********************************************************************
			// HACK:Collision detection and response

			// Our ground object is at -10.0f

			// See if our sphere "hits" the ground
			// For spheres, if the ground <= radius of the sphere, it's "hit"
			// Our model has radius of 1.0f


		//    ____        _                         ____  _                  
		//   / ___| _ __ | |__   ___ _ __ ___      |  _ \| | __ _ _ __   ___ 
		//   \___ \| '_ \| '_ \ / _ \ '__/ _ \_____| |_) | |/ _` | '_ \ / _ \
		//    ___) | |_) | | | |  __/ | |  __/_____|  __/| | (_| | | | |  __/
		//   |____/| .__/|_| |_|\___|_|  \___|     |_|   |_|\__,_|_| |_|\___|
		//         |_|                                                       
		//
		// 		const float GROUND_LOCATION_Y = -10.0f;
		//		const float GROUND_LOCATION_Y = 0.0f;
		//
		//		// Is this a sphere? 
		//		if (pCurrentMesh->friendlyName == "Sphere")
		//		{
		//			if ( (pCurrentMesh->pPhysProps->position.y - 1.0f) <= GROUND_LOCATION_Y)
		//			{
		//				// "Invert" the velocity
		//				// Velocity goes "up" +ve.
		//				float newVel = fabs(pCurrentMesh->pPhysProps->velocity.y);
		//
		//				pCurrentMesh->pPhysProps->velocity.y = newVel;
		//
		//			}
		//		}

				// Update the draw location with the physics location
		//		pCurrentMesh->drawPosition = pCurrentMesh->pPhysProps->position;

				// Copy the "physics objects" info to to the mesh
		//		pCurrentPhysObject->pTheAssociatedMesh->transInfo = pCurrentPhysObject->transInfo;

		//pCurrentPhysObject->pTheAssociatedMesh->setTransformInfo(pCurrentPhysObject->position);

		// Can't do this anymore:
//		pCurrentPhysObject->pTheAssociatedMesh->meshName = "Bunny.ply"


		// Is this a sphere? 
		if (pCurrentPhysObject->friendlyName == "camera")
		{
			// Find the ground object
			cMesh* pTheGround = cGameManager::GetGMInstance()->FindMeshByFriendlyName("snowground");

			if (!pTheGround)
			{
				return;
			}

			float sphereRadius = 1.0f;

			// Get all the triangle from this "ground" model
			sModelDrawInfo groundMeshInfo;
			glm::vec3 closestPointToTriangle = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
			float closestDistanceSoFar = FLT_MAX;
			glm::vec3 closestTriangleVertices[3] = { glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f) };
			unsigned int indexOfClosestTriangle = INT_MAX;


			//    _____ _           _        _                     _     _        _                   _      
			//   |  ___(_)_ __   __| |   ___| | ___  ___  ___  ___| |_  | |_ _ __(_) __ _ _ __   __ _| | ___ 
			//   | |_  | | '_ \ / _` |  / __| |/ _ \/ __|/ _ \/ __| __| | __| '__| |/ _` | '_ \ / _` | |/ _ \
			//   |  _| | | | | | (_| | | (__| | (_) \__ \  __/\__ \ |_  | |_| |  | | (_| | | | | (_| | |  __/
			//   |_|   |_|_| |_|\__,_|  \___|_|\___/|___/\___||___/\__|  \__|_|  |_|\__,_|_| |_|\__, |_|\___|
			//                                                                                  |___/        
			if (::g_pMeshManager->FindDrawInfoByModelName(pTheGround->meshName, groundMeshInfo))
			{
				// Which triangle is closest to this sphere (right now)
				for (unsigned int index = 0; index != groundMeshInfo.numberOfIndices; index += 3)
				{
					glm::vec3 verts[3];

					verts[0].x = groundMeshInfo.pVertices[groundMeshInfo.pIndices[index]].x;
					verts[0].y = groundMeshInfo.pVertices[groundMeshInfo.pIndices[index]].y;
					verts[0].z = groundMeshInfo.pVertices[groundMeshInfo.pIndices[index]].z;

					verts[1].x = groundMeshInfo.pVertices[groundMeshInfo.pIndices[index + 1]].x;
					verts[1].y = groundMeshInfo.pVertices[groundMeshInfo.pIndices[index + 1]].y;
					verts[1].z = groundMeshInfo.pVertices[groundMeshInfo.pIndices[index + 1]].z;

					verts[2].x = groundMeshInfo.pVertices[groundMeshInfo.pIndices[index + 2]].x;
					verts[2].y = groundMeshInfo.pVertices[groundMeshInfo.pIndices[index + 2]].y;
					verts[2].z = groundMeshInfo.pVertices[groundMeshInfo.pIndices[index + 2]].z;

					// Transform this object in world space using the same technique we did to render it
					// 
					// Here's the key line: 	
					// 
					//		vertexWorldPos = matModel * vec4( vPos.xyz, 1.0f);
					// 
					// THIS BLOCK OF CODE IS FROM DrawObject()

					glm::mat4 matModel = glm::mat4(1.0f);

					// Translation
					glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
						glm::vec3(pTheGround->drawPosition.x,
							pTheGround->drawPosition.y,
							pTheGround->drawPosition.z));

					// Rotation matrix generation
					glm::mat4 matRotateX = glm::rotate(glm::mat4(1.0f),
						pTheGround->drawOrientation.x, // (float)glfwGetTime(),
						glm::vec3(1.0f, 0.0, 0.0f));


					glm::mat4 matRotateY = glm::rotate(glm::mat4(1.0f),
						pTheGround->drawOrientation.y, // (float)glfwGetTime(),
						glm::vec3(0.0f, 1.0, 0.0f));

					glm::mat4 matRotateZ = glm::rotate(glm::mat4(1.0f),
						pTheGround->drawOrientation.z, // (float)glfwGetTime(),
						glm::vec3(0.0f, 0.0, 1.0f));

					// Scaling matrix
					glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
						glm::vec3(pTheGround->drawScale.x,
							pTheGround->drawScale.y,
							pTheGround->drawScale.z));
					//--------------------------------------------------------------

					// Combine all these transformation
					matModel = matModel * matTranslate;

					matModel = matModel * matRotateX;
					matModel = matModel * matRotateY;
					matModel = matModel * matRotateZ;

					matModel = matModel * matScale;


					// vertexWorldPos = matModel * vec4(vPos.xyz, 1.0f);

					glm::vec4 vertsWorld[3];
					vertsWorld[0] = (matModel * glm::vec4(verts[0], 1.0f));
					vertsWorld[1] = (matModel * glm::vec4(verts[1], 1.0f));
					vertsWorld[2] = (matModel * glm::vec4(verts[2], 1.0f));

					// And make sure you multiply the normal by the inverse transpose
					// OR recalculate it right here! 

					// ******************************************************


					glm::vec3 thisTriangleClosestPoint = ClosestPtPointTriangle(pTheGround->drawPosition,
						vertsWorld[0], vertsWorld[1], vertsWorld[2]);

					// Is this the closest so far
					float distanceToThisTriangle = glm::distance(thisTriangleClosestPoint, pTheGround->drawPosition);

					if (distanceToThisTriangle < closestDistanceSoFar)
					{
						// this one is closer
						closestDistanceSoFar = distanceToThisTriangle;
						// Make note of the triangle index
						indexOfClosestTriangle = index;
						// 
						closestTriangleVertices[0] = vertsWorld[0];
						closestTriangleVertices[1] = vertsWorld[1];
						closestTriangleVertices[2] = vertsWorld[2];
					}


				} //for ( unsigned int index...

				//    ____  _     _                  _     _ _     _        _                   _     ___ 
				//   |  _ \(_) __| | __      _____  | |__ (_) |_  | |_ _ __(_) __ _ _ __   __ _| | __|__ \
				//   | | | | |/ _` | \ \ /\ / / _ \ | '_ \| | __| | __| '__| |/ _` | '_ \ / _` | |/ _ \/ /
				//   | |_| | | (_| |  \ V  V /  __/ | | | | | |_  | |_| |  | | (_| | | | | (_| | |  __/_| 
				//   |____/|_|\__,_|   \_/\_/ \___| |_| |_|_|\__|  \__|_|  |_|\__,_|_| |_|\__, |_|\___(_) 
				//                                                                        |___/           
//				if (TestSphereTriangle(sphereRadius, 
//									   closestTriangleVertices[0], 
//									   closestTriangleVertices[1], 
//									   closestTriangleVertices[2], 
//									   pCurrentMesh->drawPosition) )
				// Compare the radius

				if (closestDistanceSoFar < sphereRadius)
				{
					// Hit it!
					// Take the normal of that triangle and bounce the sphere along it

					// How do we calculate the new direction after the "bounce"? 
					// 
					// Answer: it's based on the REFLECTION vector around the normal.
					// The sphere is travelling along its VELOCITY vector (at this moment)
					//	and will "bounce off" along an angle reflected by the normal

					// Calculate the current "direction" vector 
					// We're using the velocity
					glm::vec3 sphereDirection = pCurrentPhysObject->velocity;
					// Normalize... 
					sphereDirection = glm::normalize(sphereDirection);

					// Calcualte the current normal from the TRANSFORMED vertices
					glm::vec3 edgeA = closestTriangleVertices[1] - closestTriangleVertices[0];
					glm::vec3 edgeB = closestTriangleVertices[2] - closestTriangleVertices[0];

					glm::vec3 triNormal = glm::normalize(glm::cross(edgeA, edgeB));

					// Calculate the reflection vector from the normal	
					// https://registry.khronos.org/OpenGL-Refpages/gl4/html/reflect.xhtml
					// 1st parameter is the "incident" vector
					// 2nd parameter is the "normal" vector
					glm::vec3 reflectionVec = glm::reflect(sphereDirection, triNormal);

					// Update the  velocity based on this reflection vector
					float sphereSpeed = glm::length(pCurrentPhysObject->velocity);
					glm::vec3 newVelocity = reflectionVec * sphereSpeed;

					//pCurrentPhysObject->velocity = newVelocity;
				}

			}//if (::g_pMeshManager->FindDrawInfoByModelName..



//			if ((pCurrentMesh->pPhysProps->position.y - sphereRadius) <= pTheGround->drawPosition.y)
//			{
//				// "Invert" the velocity
//				// Velocity goes "up" +ve.
//				float newVel = fabs(pCurrentMesh->pPhysProps->velocity.y);
//
//				pCurrentMesh->pPhysProps->velocity.y = newVel;
//
//			}
		}//if (pCurrentMesh->friendlyName == "Sphere")


// ***********************************************************************
//
		// HACK: Determine what is the closest position to each triangle in 
		//	the ground mesh
//
//		sModelDrawInfo groundMeshInfo;
//		if (::g_pMeshManager->FindDrawInfoByModelName("Flat_Grid_100x100.ply", groundMeshInfo))
//		{
//			// For each triangle, get the vertices, then pass them to the 
//			//	sphere-triangle collision function, returning the "closest point"
//			for ( unsigned int index = 0; index != groundMeshInfo.numberOfIndices; index += 3 )
//			{	
//				glm::vec3 verts[3];
//
//				verts[0].x = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index] ].x;
//				verts[0].y = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index] ].y;
//				verts[0].z = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index] ].z;
//
//				verts[1].x = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index + 1] ].x;
//				verts[1].y = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index + 1] ].y;
//				verts[1].z = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index + 1] ].z;
//
//				verts[2].x = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index + 2] ].x;
//				verts[2].y = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index + 2] ].y;
//				verts[2].z = groundMeshInfo.pVertices[ groundMeshInfo.pIndices[index + 2] ].z;
//
//				glm::vec3 closestPoint = ClosestPtPointTriangle(pCurrentMesh->drawPosition,
//																verts[0], verts[1], verts[2]);
//
//
////				::g_pDebugRenderer->AddLine(
//
////				// HACK:
////				if (index == 99 )
////				{
////					std::cout << closestPoint.x << ", " << closestPoint.y << ", " << closestPoint.z << std::endl;
////				}
//				//if (pDebugSphere != NULL)
////				if (pDebugSphere)			// NON zero => true, zero = false;
////				{
////					pDebugSphere->drawPosition = closestPoint;
////					pDebugSphere->scale = 0.5f;
////					pDebugSphere->bIsVisible = true;
////
////// HACK: 
////					extern glm::mat4 matProjection;    // "projection"
////					extern glm::mat4 matView;          // "view" or "camera"
////					extern GLuint shaderProgramID;
////
////					DrawObject(pDebugSphere, glm::mat4(1.0f), 
////							   matProjection, matView, shaderProgramID);
////
////					pDebugSphere->bIsVisible = false;
////				}
////
//			}//for ( unsigned int index = 0...
//		}//if (pMeshManager->FindDrawInfoByModelName(
//
//	}// for (unsigned int meshIndex

	}// For loop

	return;
}

bool TestSphereTriangle(float sphereRadius, glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2,
	glm::vec3 sphereCentre)
{
	// Find point P on triangle ABC closest to sphere center
	glm::vec3 closestPoint = ClosestPtPointTriangle(sphereCentre, vert0, vert1, vert2);

	// Sphere and triangle intersect if the (squared) distance from sphere
	// center to point p is less than the (squared) sphere radius
	glm::vec3 v = closestPoint - sphereCentre;

	bool isItIntersecting = false;

	if (glm::dot(v, v) <= (sphereRadius * sphereRadius))
	{
		isItIntersecting = true;
	}

	return isItIntersecting;
}

glm::vec3 ClosestPtPointTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 bc = c - b;

	// Compute parametric position s for projection P' of P on AB,
	// P' = A + s*AB, s = snom/(snom+sdenom)
	float snom = glm::dot(p - a, ab), sdenom = glm::dot(p - b, a - b);

	// Compute parametric position t for projection P' of P on AC,
	// P' = A + t*AC, s = tnom/(tnom+tdenom)
	float tnom = glm::dot(p - a, ac), tdenom = glm::dot(p - c, a - c);

	if (snom <= 0.0f && tnom <= 0.0f) return a; // Vertex region early out

	// Compute parametric position u for projection P' of P on BC,
	// P' = B + u*BC, u = unom/(unom+udenom)
	float unom = glm::dot(p - b, bc), udenom = glm::dot(p - c, b - c);

	if (sdenom <= 0.0f && unom <= 0.0f) return b; // Vertex region early out
	if (tdenom <= 0.0f && udenom <= 0.0f) return c; // Vertex region early out


	// P is outside (or on) AB if the triple scalar product [N PA PB] <= 0
	glm::vec3 n = glm::cross(b - a, c - a);
	float vc = glm::dot(n, glm::cross(a - p, b - p));
	// If P outside AB and within feature region of AB,
	// return projection of P onto AB
	if (vc <= 0.0f && snom >= 0.0f && sdenom >= 0.0f)
		return a + snom / (snom + sdenom) * ab;

	// P is outside (or on) BC if the triple scalar product [N PB PC] <= 0
	float va = glm::dot(n, glm::cross(b - p, c - p));
	// If P outside BC and within feature region of BC,
	// return projection of P onto BC
	if (va <= 0.0f && unom >= 0.0f && udenom >= 0.0f)
		return b + unom / (unom + udenom) * bc;

	// P is outside (or on) CA if the triple scalar product [N PC PA] <= 0
	float vb = glm::dot(n, glm::cross(c - p, a - p));
	// If P outside CA and within feature region of CA,
	// return projection of P onto CA
	if (vb <= 0.0f && tnom >= 0.0f && tdenom >= 0.0f)
		return a + tnom / (tnom + tdenom) * ac;

	// P must project inside face region. Compute Q using barycentric coordinates
	float u = va / (va + vb + vc);
	float v = vb / (va + vb + vc);
	float w = 1.0f - u - v; // = vc / (va + vb + vc)
	return u * a + v * b + w * c;
}
