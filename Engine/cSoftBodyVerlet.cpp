#include "cSoftBodyVerlet.h"
#include "cVAOManager/cVAOManager.h"
#include <iostream>
#include "MathUtils.h"
#include "cGameManager.h"
#include "Engine.h"
#include "cScene.h"

extern cVAOManager* g_pMeshManager;
extern cScene* g_currentScene;
extern cMesh* g_particleMeshModel;

//extern cMesh* tractorMesh;
//extern cMesh* chain1Mesh;
//extern cMesh* chain2Mesh;
//extern cMesh* chain3Mesh;
//extern cMesh* chain4Mesh;
//extern cMesh* bridgeMesh;

extern cMesh* oceanMesh;

extern Engine engine;

void cSoftBodyVerlet::Start(GLuint shaderProgramID)
{
    //tractorMesh = g_currentScene->GetMeshByFriendlyName("tractor");
    //chain1Mesh = g_currentScene->GetMeshByFriendlyName("chain1");
    //chain2Mesh = g_currentScene->GetMeshByFriendlyName("chain2");
    //oceanMesh = g_currentScene->GetMeshByFriendlyName("mars");
    //bridgeMesh = g_currentScene->GetMeshByFriendlyName("bridge");
}

void cSoftBodyVerlet::Update(double deltaTime, GLuint shaderProgramID)
{
    VerletUpdate(deltaTime);
    SatisfyConstraints();
    ApplyCollision(deltaTime);

    glm::mat4 matModel = glm::mat4(1.0f);   // Identity matrix

    for (cSoftBodyVerlet::sParticle* particle : vec_pParticles)
    {

        if (particle->isBottomParticle)
        {
            //engine.g_DrawDebugSphere(particle->position, 0.10f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), deltaTime);
        }
        else
        {
            //engine.g_DrawDebugSphere(particle->position, 0.10f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), deltaTime);
        }

        //g_particleMeshModel->drawPosition = particle->position;
        //g_particleMeshModel->setUniformDrawScale(1.0f);
        //g_particleMeshModel->setDrawOrientation(particle->orientation);
        //engine.DrawObject(g_particleMeshModel, matModel, shaderProgramID, deltaTime);
        //g_softBody.CreateSoftBody(particle);

    }

    //engine.DrawObject(g_particleMeshModel, matModel, shaderProgramID, deltaTime);


    UpdateVertexPositions();
    UpdateNormals();
    g_pMeshManager->UpdateVertexBuffers(
        m_modelVertexInfo.meshName,
        m_modelVertexInfo,
        shaderProgramID);


    //if (tag == "tractor")
    //{
    //    glm::mat4 matTranslate = glm::mat4(1.0f);// glm::translate(glm::mat4(1.0f), flagMesh->drawPosition);

    //    cMesh softBodyMesh;
    //    softBodyMesh.meshName = m_modelVertexInfo.meshName;
    //    softBodyMesh.textureName[0] = tractorMesh->textureName[0];
    //    softBodyMesh.textureRatios[0] = 1.0f;
    //    //softBodyMesh.drawPosition = tractorMesh->drawPosition;
    //    softBodyMesh.bDrawBothSides = false;

    //    //GLint isFlag = glGetUniformLocation(shaderProgramID, "isFlag");
    //    //glUniform1f(isFlag, (GLfloat)GL_TRUE);

    //    //DrawObject(&softBodyMesh, matTranslate, shaderProgramID);
    //    glDisable(GL_CULL_FACE);

    //    softBodyMesh.setDrawOrientation(glm::vec3(0.0f));
    //    engine.DrawObject(&softBodyMesh, matTranslate, shaderProgramID, deltaTime);
    //    glEnable(GL_CULL_FACE);
    //    //glUniform1f(isFlag, (GLfloat)GL_FALSE);

    //}

    //if (tag == "chain1")
    //{
    //    glm::mat4 matTranslate = glm::mat4(1.0f);// glm::translate(glm::mat4(1.0f), flagMesh->drawPosition);

    //    cMesh chainMesh;
    //    chainMesh.meshName = chain1Mesh->meshName;
    //    chainMesh.textureName[0] = chain1Mesh->textureName[0];
    //    chainMesh.textureRatios[0] = 1.0f;
    //    //softBodyMesh.drawPosition = tractorMesh->drawPosition;
    //    chainMesh.bDrawBothSides = false;
    //    chainMesh.friendlyName = chain1Mesh->friendlyName;
    //    //chainMesh.setDrawOrientation(glm::vec3(0.0f));

    //    glDisable(GL_CULL_FACE);
    //    engine.DrawObject(&chainMesh, matTranslate, shaderProgramID, deltaTime);
    //    glEnable(GL_CULL_FACE);

    //    for (cSoftBodyVerlet::sParticle* particle : vec_pParticles)
    //    {
    //        if (particle->isLeftFrontHinge)
    //        {
    //            engine.g_DrawDebugSphere(particle->position, 0.10f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), deltaTime);
    //        }
    //        else
    //        {
    //            engine.g_DrawDebugSphere(particle->position, 0.10f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), deltaTime);
    //        }
    //    }
    //}

    //if (tag == "chain2")
    //{
    //    glm::mat4 matTranslate = glm::mat4(1.0f);// glm::translate(glm::mat4(1.0f), flagMesh->drawPosition);

    //    cMesh chainMesh;
    //    chainMesh.meshName = chain2Mesh->meshName;
    //    chainMesh.textureName[0] = chain2Mesh->textureName[0];
    //    chainMesh.textureRatios[0] = 1.0f;
    //    //softBodyMesh.drawPosition = tractorMesh->drawPosition;
    //    chainMesh.bDrawBothSides = false;
    //    chainMesh.friendlyName = chain2Mesh->friendlyName;
    //    //chainMesh.setDrawOrientation(glm::vec3(0.0f));

    //    glDisable(GL_CULL_FACE);
    //    engine.DrawObject(&chainMesh, matTranslate, shaderProgramID, deltaTime);
    //    glEnable(GL_CULL_FACE);

    //    for (cSoftBodyVerlet::sParticle* particle : vec_pParticles)
    //    {
    //        if (particle->isLeftFrontHinge)
    //        {
    //            engine.g_DrawDebugSphere(particle->position, 0.10f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), deltaTime);
    //        }
    //        else
    //        {
    //            engine.g_DrawDebugSphere(particle->position, 0.10f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), deltaTime);
    //        }
    //    }
    //}

    

    //{
    //    cMesh* flagMesh = cGameManager::GetGMInstance()->FindMeshByFriendlyName("flag");

    //    windBlowingTimer -= deltaTime;

    //    if (windBlowingTimer < 0.0f)
    //    {
    //        isWindBlowing = (!isWindBlowing);
    //        windBlowingTimer = windBlowingInitialTimer;
    //        flagWind = 0.0;
    //    }

    //    if (isWindBlowing)
    //    {
    //        flagWind += deltaTime;
    //        double windStrength = glm::sin(flagWind);
    //        windStrength = windStrength / 2.0f;
    //        acceleration.z = (float)windStrength * (-1.0f);
    //        acceleration.y = -0.2f;
    //    }
    //    else
    //    {
    //        acceleration.z = 0.0f;
    //        acceleration.y = -0.2f;
    //    }

    //    glm::mat4 matTranslate = glm::mat4(1.0f);// glm::translate(glm::mat4(1.0f), flagMesh->drawPosition);

    //    m_modelVertexInfo.pVertices[50].w = 1.0f;

    //    //g_pMeshManager->LoadModelIntoVAO()

    //    cMesh softBodyMesh;
    //    softBodyMesh.meshName = m_modelVertexInfo.meshName;
    //    softBodyMesh.textureName[0] = "night_lord_flag_texture.bmp";
    //    softBodyMesh.textureRatios[0] = 1.0f;
    //    softBodyMesh.bDrawBothSides = false;
    //    //softBodyMesh.bDoNotLight = true;

    //    engine.SetBoolInFragmentShader("isFlag", true);

    //    //DrawObject(&softBodyMesh, matTranslate, shaderProgramID);
    //    glDisable(GL_CULL_FACE);
    //    engine.DrawObject(&softBodyMesh, matTranslate, shaderProgramID);
    //    glEnable(GL_CULL_FACE);

    //    engine.SetBoolInFragmentShader("isFlag", false);
    //}
}

cSoftBodyVerlet::cSoftBodyVerlet()
{
}

cSoftBodyVerlet::~cSoftBodyVerlet()
{
}

void cSoftBodyVerlet::ResetFlagPosition()
{
    cMesh* flagMesh = cGameManager::GetGMInstance()->FindMeshByFriendlyName("flag");
    //flagMesh->bIsVisible = false;

    if (flagMesh)
    {
        acceleration = glm::vec3(0.0f);

        vec_pParticles.clear();
        vec_pConstraints.clear();

        isFlagConnected = true;

        CreateSoftBody(flagMesh);
        std::cout << "Soft body created! Constraints: " << vec_pConstraints.size() << std::endl;
    }
}

bool cSoftBodyVerlet::CreateSoftBody(sModelDrawInfo modelInfo, glm::mat4 matInitalTransform)
{
    m_modelVertexInfo = modelInfo;

    // Dynamically allocate memory on the heap;
    // sVertexPlyFile* pTheVerticesFile = new sVertexPlyFile[g_numberOfVertices];
    sVertex* pVerticesLocalCopy = new sVertex[modelInfo.numberOfVertices];
    for (unsigned int index = 0; index != modelInfo.numberOfVertices; index++)
    {
        glm::vec4 theVertex = glm::vec4(
            modelInfo.pVertices[index].x ,
            modelInfo.pVertices[index].y ,
            modelInfo.pVertices[index].z , 1.0f);
        // Apply transformation
        theVertex = matInitalTransform * theVertex;

        pVerticesLocalCopy[index].x = theVertex.x;
        pVerticesLocalCopy[index].y = theVertex.y;
        pVerticesLocalCopy[index].z = theVertex.z;
        pVerticesLocalCopy[index].w = modelInfo.pVertices[index].w;

        pVerticesLocalCopy[index].r = modelInfo.pVertices[index].r;
        pVerticesLocalCopy[index].g = modelInfo.pVertices[index].g;
        pVerticesLocalCopy[index].b = modelInfo.pVertices[index].b;
        pVerticesLocalCopy[index].a = modelInfo.pVertices[index].a;

        pVerticesLocalCopy[index].nx = modelInfo.pVertices[index].nx;
        pVerticesLocalCopy[index].ny = modelInfo.pVertices[index].ny;
        pVerticesLocalCopy[index].nz = modelInfo.pVertices[index].nz;
        pVerticesLocalCopy[index].nw = modelInfo.pVertices[index].nw;

        pVerticesLocalCopy[index].u = modelInfo.pVertices[index].u;
        pVerticesLocalCopy[index].v = modelInfo.pVertices[index].v;
    }

    unsigned int* pIndicesLocalCopy = new unsigned int[modelInfo.numberOfIndices];
    for (unsigned int index = 0; index != modelInfo.numberOfIndices; index++)
    {
        pIndicesLocalCopy[index] = modelInfo.pIndices[index];
    }

    this->m_modelVertexInfo.pVertices = pVerticesLocalCopy;
    this->m_modelVertexInfo.pIndices = pIndicesLocalCopy;


    vec_pParticles.reserve(m_modelVertexInfo.numberOfVertices);

    for (unsigned int index = 0; index != m_modelVertexInfo.numberOfVertices; index++)
    {
        sParticle* particle = new sParticle();
        particle->position.x = m_modelVertexInfo.pVertices[index].x;
        particle->position.y = m_modelVertexInfo.pVertices[index].y;
        particle->position.z = m_modelVertexInfo.pVertices[index].z;

        particle->oldPosition = particle->position;

        particle->pModelVertex = &(m_modelVertexInfo.pVertices[index]);

        vec_pParticles.push_back(particle);
    }

    // Constraints
    for (unsigned int index = 0; index < this->m_modelVertexInfo.numberOfIndices; index += 3)
    {
        sParticle* pParticle1 = this->vec_pParticles[ this->m_modelVertexInfo.pIndices[index] ];
        sParticle* pParticle2 = this->vec_pParticles[ this->m_modelVertexInfo.pIndices[index + 1] ];
        sParticle* pParticle3 = this->vec_pParticles[ this->m_modelVertexInfo.pIndices[index + 2] ];

        sConstraint* pEdge1 = new sConstraint();
        pEdge1->pParticleA = pParticle1;
        pEdge1->pParticleB = pParticle2;
        pEdge1->restLength = CalcDistanceBetween(pEdge1->pParticleA, pEdge1->pParticleB);

        sConstraint* pEdge2 = new sConstraint();
        pEdge2->pParticleA = pParticle2;
        pEdge2->pParticleB = pParticle3;
        pEdge2->restLength = CalcDistanceBetween(pEdge2->pParticleA, pEdge2->pParticleB);

        sConstraint* pEdge3 = new sConstraint();
        pEdge3->pParticleA = pParticle3;
        pEdge3->pParticleB = pParticle1;
        pEdge3->restLength = CalcDistanceBetween(pEdge3->pParticleA, pEdge3->pParticleB);

        vec_pConstraints.push_back(pEdge1);
        vec_pConstraints.push_back(pEdge2);
        vec_pConstraints.push_back(pEdge3);
    }

    return true;
}

bool cSoftBodyVerlet::CreateSoftBody(cMesh* mesh, glm::mat4 matInitalTransform)
{
    sModelDrawInfo ModelInfo;

    if (!(g_pMeshManager->FindDrawInfoByModelName(mesh->meshName, ModelInfo)))
    {
        std::cout << "Mesh name not found!" << std::endl;
    }

    this->m_modelVertexInfo = ModelInfo;

    glm::vec3 meshPosition = mesh->drawPosition;

    sVertex* pVerticesLocalCopy = new sVertex[ModelInfo.numberOfVertices];
    for (unsigned int index = 0; index != ModelInfo.numberOfVertices; index++)
    {
        glm::vec4 theVertex = glm::vec4(
            ModelInfo.pVertices[index].x + meshPosition.x,
            ModelInfo.pVertices[index].y + meshPosition.y,
            ModelInfo.pVertices[index].z + meshPosition.z, 1.0f);
        // Apply transformation
        theVertex = matInitalTransform * theVertex;

        pVerticesLocalCopy[index].x = theVertex.x;
        pVerticesLocalCopy[index].y = theVertex.y;
        pVerticesLocalCopy[index].z = theVertex.z;
        pVerticesLocalCopy[index].w = ModelInfo.pVertices[index].w;

        pVerticesLocalCopy[index].r = ModelInfo.pVertices[index].r;
        pVerticesLocalCopy[index].g = ModelInfo.pVertices[index].g;
        pVerticesLocalCopy[index].b = ModelInfo.pVertices[index].b;
        pVerticesLocalCopy[index].a = ModelInfo.pVertices[index].a;

        pVerticesLocalCopy[index].nx = ModelInfo.pVertices[index].nx;
        pVerticesLocalCopy[index].ny = ModelInfo.pVertices[index].ny;
        pVerticesLocalCopy[index].nz = ModelInfo.pVertices[index].nz;
        pVerticesLocalCopy[index].nw = ModelInfo.pVertices[index].nw;

        pVerticesLocalCopy[index].u = ModelInfo.pVertices[index].u;
        pVerticesLocalCopy[index].v = ModelInfo.pVertices[index].v;
    }//for (unsigned int index



    unsigned int* pIndicesLocalCopy = new unsigned int[ModelInfo.numberOfIndices];
    for (unsigned int index = 0; index != ModelInfo.numberOfIndices; index++)
    {
        pIndicesLocalCopy[index] = ModelInfo.pIndices[index];
    }//for (unsigned int index

    this->m_modelVertexInfo.pVertices = pVerticesLocalCopy;
    this->m_modelVertexInfo.pIndices = pIndicesLocalCopy;


    // From this point on, we are referencing the copy of the original vertices

    // Go through the model info and add the particles from the vertex locations
    this->vec_pParticles.reserve(this->m_modelVertexInfo.numberOfVertices);

    for (unsigned int index = 0; index != this->m_modelVertexInfo.numberOfVertices; index++)
    {
        sParticle* pParticle = new sParticle();
        pParticle->position.x = this->m_modelVertexInfo.pVertices[index].x;
        pParticle->position.y = this->m_modelVertexInfo.pVertices[index].y;
        pParticle->position.z = this->m_modelVertexInfo.pVertices[index].z;

        pParticle->oldPosition = pParticle->position;

        // Save the pointer address.
        // Note the "&" (address of)
        pParticle->pModelVertex = &(this->m_modelVertexInfo.pVertices[index]);

        this->vec_pParticles.push_back(pParticle);
    }//for ( unsigned int index =...

    // Constraints
    // Note: we are stepping through the array by 3s (3 vertices per triangle)
    for (unsigned int index = 0; index < this->m_modelVertexInfo.numberOfIndices; index += 3)
    {
        // Example: ply file has “3 733 1026 736” for this triangle.
        // pIndices would have:
        // 
        //	 733	this->m_ModelVertexInfo.pIndices[ index + 0 ]
        //  1026	this->m_ModelVertexInfo.pIndices[ index + 1 ]
        //   736	this->m_ModelVertexInfo.pIndices[ index + 2 ]
        //
        sParticle* pParticle1 = this->vec_pParticles[this->m_modelVertexInfo.pIndices[index /* +0 */]];
        sParticle* pParticle2 = this->vec_pParticles[this->m_modelVertexInfo.pIndices[index + 1]];
        sParticle* pParticle3 = this->vec_pParticles[this->m_modelVertexInfo.pIndices[index + 2]];

        sConstraint* pEdge1 = new sConstraint();
        pEdge1->pParticleA = pParticle1;
        pEdge1->pParticleB = pParticle2;
        pEdge1->restLength = this->CalcDistanceBetween(pEdge1->pParticleA, pEdge1->pParticleB);

        sConstraint* pEdge2 = new sConstraint();
        pEdge2->pParticleA = pParticle2;
        pEdge2->pParticleB = pParticle3;
        pEdge2->restLength = this->CalcDistanceBetween(pEdge2->pParticleA, pEdge2->pParticleB);

        sConstraint* pEdge3 = new sConstraint();
        pEdge3->pParticleA = pParticle3;
        pEdge3->pParticleB = pParticle1;
        pEdge3->restLength = this->CalcDistanceBetween(pEdge3->pParticleA, pEdge3->pParticleB);

        this->vec_pConstraints.push_back(pEdge1);
        this->vec_pConstraints.push_back(pEdge2);
        this->vec_pConstraints.push_back(pEdge3);
    }//for ( unsigned int index

    return true;
}

void cSoftBodyVerlet::ActivateAllParticles()
{
    //std::vector< sParticle* >::iterator particleIt;
    //for (particleIt = vec_pParticles.begin(); particleIt != vec_pParticles.end(); particleIt++)
    //{
    //    (*particleIt)->isActive = true;
    //}

    //std::vector< sConstraint* >::iterator constraintIt;
    //for (constraintIt = vec_pConstraints.begin(); constraintIt != vec_pConstraints.end(); constraintIt++)
    //{
    //    (*constraintIt)->bIsActive = true;
    //}
}

void cSoftBodyVerlet::CreateRandomBracing(unsigned int numberOfBraces, float minDistanceBetweenVertices)
{
    for (unsigned int count = 0; count != numberOfBraces; count++)
    {
        bool bKeepLookingForParticles = false;

        do
        {
            bKeepLookingForParticles = false;

            unsigned int particleIndex1 = rand() % this->vec_pParticles.size();
            unsigned int particleIndex2 = rand() % this->vec_pParticles.size();;

            sParticle* pParticle1 = this->vec_pParticles[particleIndex1];
            sParticle* pParticle2 = this->vec_pParticles[particleIndex2];
            float distBetween = CalcDistanceBetween(pParticle1, pParticle2);

            // Distance ok?
            if (distBetween < minDistanceBetweenVertices)
            {
                bKeepLookingForParticles = true;
            }
            else
            {
                sConstraint* pBracingConstraint = new sConstraint();
                pBracingConstraint->pParticleA = pParticle1;
                pBracingConstraint->pParticleB = pParticle2;
                pBracingConstraint->restLength = CalcDistanceBetween(pBracingConstraint->pParticleA, pBracingConstraint->pParticleB);
                this->vec_pConstraints.push_back(pBracingConstraint);
            }
        } while (bKeepLookingForParticles);
    }

    return;
}

void cSoftBodyVerlet::CreateFlagBracing()
{
    for (unsigned int count = 0; count < 100; count += 2)
    {
        unsigned int particleIndex1 = count;
        unsigned int particleIndex2 = count + 1;

        sParticle* pParticle1 = this->vec_pParticles[particleIndex1];
        sParticle* pParticle2 = this->vec_pParticles[particleIndex2];
        float distBetween = CalcDistanceBetween(pParticle1, pParticle2);

        sConstraint* pBracingConstraint = new sConstraint();
        pBracingConstraint->pParticleA = pParticle1;
        pBracingConstraint->pParticleB = pParticle2;
        pBracingConstraint->restLength = CalcDistanceBetween(pBracingConstraint->pParticleA, pBracingConstraint->pParticleB);
        this->vec_pConstraints.push_back(pBracingConstraint);
    }

    return;
}

void cSoftBodyVerlet::DisconnectRandomHole()
{
    //int randomIndex = MathUtils::GetRandomInt(0, m_modelVertexInfo.numberOfVertices);

    //m_modelVertexInfo.pVertices[randomIndex].w = 1.0f;

    //m_modelVertexInfo.pVertices.

    //int randomIndex = MathUtils::GetRandomInt(0, (unsigned int)vec_pParticles.size() - 11);
    //sParticle* particleA = vec_pParticles[randomIndex];
    //sParticle* particleB = nullptr;

    ////sParticle* particleB = centerConstraint->pParticleA;

    //particleA->isActive = false;

    //std::vector<sParticle* > particlesToDelete;

    //std::vector< sConstraint* >::iterator constraintIt;
    //for (constraintIt = vec_pConstraints.begin(); constraintIt != vec_pConstraints.end(); )
    //{
    //    if (((*constraintIt)->pParticleA == particleA) || ((*constraintIt)->pParticleB == particleA))
    //    {
    //        (*constraintIt)->bIsActive = false;

    //        int randomIndex = MathUtils::GetRandomInt(0, (unsigned int)vec_pParticles.size() - 11);
    //        particleB = (*constraintIt)->pParticleB;

    //        //particleB->isActive = false;

    //        constraintIt++;
    //    }
    //    else
    //    {
    //        constraintIt++;
    //    }
    //}

    //for (unsigned int i = 0; i < vec_pConstraints.size(); i++)
    //{
    //    float distance = glm::distance(particleA->position, vec_pConstraints[i]->pParticleA->position);

    //    if (distance < 1.0f)
    //    {
    //        vec_pConstraints[i]->bIsActive = false;
    //        vec_pConstraints[i]->pParticleA->isActive = false;
    //        vec_pConstraints[i]->pParticleB->isActive = false;
    //    }
    //}

    //std::vector< sParticle* >::iterator particleAIt;
    //for (particleAIt = vec_pParticles.begin(); particleAIt != vec_pParticles.end(); )
    //{
    //    if (false == (*particleAIt)->isActive)
    //    {
    //        delete* particleAIt;
    //        particleAIt = vec_pParticles.erase(particleAIt);
    //    }
    //    else
    //    {
    //        particleAIt++;
    //    }
    //}

    //delete particleA;
    //vec_pParticles.erase(vec_pParticles.begin() + particleToDeleteIndex);
}

// Will update the vertex information to match the particles
void cSoftBodyVerlet::UpdateVertexPositions(void)
{
    // Copy the current particle positions to the local vertex locations

    // ENTER CRITICAL SECTION
    for (sParticle* curParticle : this->vec_pParticles)
    {
        curParticle->pModelVertex->x = curParticle->position.x;
        curParticle->pModelVertex->y = curParticle->position.y;
        curParticle->pModelVertex->z = curParticle->position.z;
    }
    // LEAVE CRITICAL SECTION

    // At this point, our local sModelDrawInfo pVertices array has the 
    //	current locations of the particles (i.e. the mesh is updated)

    // TODO: Update normals, etc.

    return;
}

void cSoftBodyVerlet::UpdateNormals(void)
{
    // Go through each triagle and calculate the normals. 
    // Accumulate those normals at each vertex.
    // Normalize the normals

    // Clear the existing normals because they are invalid
    for (unsigned int vertIndex = 0; vertIndex != this->m_modelVertexInfo.numberOfVertices; vertIndex++)
    {
        this->m_modelVertexInfo.pVertices[vertIndex].nx = 0.0f;
        this->m_modelVertexInfo.pVertices[vertIndex].ny = 0.0f;
        this->m_modelVertexInfo.pVertices[vertIndex].nz = 0.0f;
    }

    for (unsigned int triIndex = 0; triIndex != this->m_modelVertexInfo.numberOfTriangles; triIndex++)
    {
        // Indices are sets of 3, one per 
        unsigned int vertAIndex = this->m_modelVertexInfo.pIndices[triIndex + 0];
        unsigned int vertBIndex = this->m_modelVertexInfo.pIndices[triIndex + 1];
        unsigned int vertCIndex = this->m_modelVertexInfo.pIndices[triIndex + 2];

        // note the references so that when we update this, it will update the mesh
        // (otherwise we'll be updating a copy of it)
        sVertex& vertexA = this->m_modelVertexInfo.pVertices[vertAIndex];
        sVertex& vertexB = this->m_modelVertexInfo.pVertices[vertBIndex];
        sVertex& vertexC = this->m_modelVertexInfo.pVertices[vertCIndex];

        glm::vec3 vertA = glm::vec3(vertexA.x, vertexA.y, vertexA.z);
        glm::vec3 vertB = glm::vec3(vertexB.x, vertexB.y, vertexB.z);
        glm::vec3 vertC = glm::vec3(vertexC.x, vertexC.y, vertexC.z);

        glm::vec3 triangleEdgeAtoB = vertB - vertA;
        glm::vec3 triangleEdgeAtoC = vertC - vertA;

        glm::vec3 theNormal = glm::cross(triangleEdgeAtoB, triangleEdgeAtoC);
        theNormal = glm::normalize(theNormal);

        // Add (accumulate) this normal to the three vertices
        vertexA.nx += theNormal.x;
        vertexA.ny += theNormal.y;
        vertexA.nz += theNormal.z;

        vertexB.nx += theNormal.x;
        vertexB.ny += theNormal.y;
        vertexB.nz += theNormal.z;

        vertexC.nx += theNormal.x;
        vertexC.ny += theNormal.y;
        vertexC.nz += theNormal.z;
    }// for ( unsigned int triIndex = 0

    // ENTER CRITICAL SECTION

    // Now normalize the accumulated normals
    for (unsigned int vertIndex = 0; vertIndex != this->m_modelVertexInfo.numberOfVertices; vertIndex++)
    {
        glm::vec3 theNormal = glm::vec3(this->m_modelVertexInfo.pVertices[vertIndex].nx,
            this->m_modelVertexInfo.pVertices[vertIndex].ny,
            this->m_modelVertexInfo.pVertices[vertIndex].nz);

        theNormal = glm::normalize(theNormal);

        this->m_modelVertexInfo.pVertices[vertIndex].nx = theNormal.x;
        this->m_modelVertexInfo.pVertices[vertIndex].ny = theNormal.y;
        this->m_modelVertexInfo.pVertices[vertIndex].nz = theNormal.z;
    }

    // LEAVE CRITICAL SECTION

    return;
}

glm::vec3 cSoftBodyVerlet::GetCenterPoint(void)
{
    glm::vec3 centerXYZ = glm::vec3(0.0f);

    for (sParticle* pCurrentParticle : vec_pParticles)
    {
        centerXYZ += pCurrentParticle->position;
    }

    centerXYZ = centerXYZ / (float)vec_pParticles.size();

    return centerXYZ;
}

bool cSoftBodyVerlet::UpdateDrawInfo()
{
    return false;
}

void cSoftBodyVerlet::VerletUpdate(double deltaTime)
{
    if (deltaTime > MAX_DELTATIME)
    {
        deltaTime = MAX_DELTATIME;
    }

    for (sParticle* particle : vec_pParticles)
    {
        glm::vec3 currentPosition = particle->position;
        glm::vec3 oldPosition = particle->oldPosition;

        particle->position += (currentPosition - oldPosition) + (acceleration * (float)(deltaTime * deltaTime));


        particle->oldPosition = currentPosition;
        //particle->position = newPosition;

        CleanZeros(particle->position);
        CleanZeros(particle->oldPosition);
    }

    return;
}

void cSoftBodyVerlet::SatisfyConstraints(void)
{
    const unsigned int NUM_ITERATIONS = 10;

    for (unsigned int iteration = 0; iteration != NUM_ITERATIONS; iteration++)
    {
        for (sConstraint* pCurConstraint : vec_pConstraints)
        {
            if (pCurConstraint->bIsActive)
            {
                cSoftBodyVerlet::sParticle* pX1 = pCurConstraint->pParticleA;
                cSoftBodyVerlet::sParticle* pX2 = pCurConstraint->pParticleB;

                if (pX1->isActive && pX2->isActive)
                {
                    glm::vec3 delta = pX2->position - pX1->position;
                    float deltaLength = glm::length(delta);
                    float diff = (deltaLength - pCurConstraint->restLength) / deltaLength;

                    float tightnessFactor = 0.6f;

                    pX1->position += delta * 0.5f * diff * tightnessFactor;
                    pX2->position -= delta * 0.5f * diff * tightnessFactor;

                    this->CleanZeros(pX1->position);
                    this->CleanZeros(pX2->position);
                }
            }
        }
    }

    

    //if (isFlagConnected)
    //{
    //    unsigned int lastIndex = (unsigned int)vec_pParticles.size() - 1;

    //    this->vec_pParticles[lastIndex - 9]->position.x = 2.499999f + 2.6f;
    //    this->vec_pParticles[lastIndex - 8]->position.x = 1.944444f + 2.6f;
    //    this->vec_pParticles[lastIndex - 7]->position.x = 1.388889f + 2.6f;
    //    this->vec_pParticles[lastIndex - 6]->position.x = 0.833333f + 2.6f;
    //    this->vec_pParticles[lastIndex - 5]->position.x = 0.2777778f + 2.6f;
    //    this->vec_pParticles[lastIndex - 4]->position.x = -0.2777776f + 2.6f;
    //    this->vec_pParticles[lastIndex - 3]->position.x = -0.8333328f + 2.6f;
    //    this->vec_pParticles[lastIndex - 2]->position.x = -1.388888f + 2.6f;
    //    this->vec_pParticles[lastIndex - 1]->position.x = -1.944444f + 2.6f;
    //    this->vec_pParticles[lastIndex]->position.x = -2.499999f + 2.6f;

    //    this->vec_pParticles[lastIndex - 9]->position.y = 40.0f;
    //    this->vec_pParticles[lastIndex - 8]->position.y = 40.0f;
    //    this->vec_pParticles[lastIndex - 7]->position.y = 40.0f;
    //    this->vec_pParticles[lastIndex - 6]->position.y = 40.0f;
    //    this->vec_pParticles[lastIndex - 5]->position.y = 40.0f;
    //    this->vec_pParticles[lastIndex - 4]->position.y = 40.0f;
    //    this->vec_pParticles[lastIndex - 3]->position.y = 40.0f;
    //    this->vec_pParticles[lastIndex - 2]->position.y = 40.0f;
    //    this->vec_pParticles[lastIndex - 1]->position.y = 40.0f;
    //    this->vec_pParticles[lastIndex]->position.y = 40.0f;

    //    this->vec_pParticles[lastIndex - 9]->position.z = this->vec_pParticles[lastIndex - 9]->oldPosition.z;
    //    this->vec_pParticles[lastIndex - 8]->position.z = this->vec_pParticles[lastIndex - 8]->oldPosition.z;
    //    this->vec_pParticles[lastIndex - 7]->position.z = this->vec_pParticles[lastIndex - 7]->oldPosition.z;
    //    this->vec_pParticles[lastIndex - 6]->position.z = this->vec_pParticles[lastIndex - 6]->oldPosition.z;
    //    this->vec_pParticles[lastIndex - 5]->position.z = this->vec_pParticles[lastIndex - 5]->oldPosition.z;
    //    this->vec_pParticles[lastIndex - 4]->position.z = this->vec_pParticles[lastIndex - 4]->oldPosition.z;
    //    this->vec_pParticles[lastIndex - 3]->position.z = this->vec_pParticles[lastIndex - 3]->oldPosition.z;
    //    this->vec_pParticles[lastIndex - 2]->position.z = this->vec_pParticles[lastIndex - 2]->oldPosition.z;
    //    this->vec_pParticles[lastIndex - 1]->position.z = this->vec_pParticles[lastIndex - 1]->oldPosition.z;
    //    this->vec_pParticles[lastIndex]->position.z = this->vec_pParticles[lastIndex]->oldPosition.z;

    //    // Getting leftmost particles
    //    for (unsigned int i = 0; i < vec_pParticles.size(); i++)
    //    {
    //        if (vec_pParticles[i]->position.x < -2.4f + 2.6f)
    //        {
    //            vec_pParticles[i]->position.x = -2.49f + 2.6f;
    //            vec_pParticles[i]->position.y = vec_pParticles[i]->oldPosition.y;

    //            vec_pParticles[i]->position.z = 0.0f;
    //        }

    //        //if (vec_pParticles[i]->position.z < 0.0f)
    //        //{
    //        //    vec_pParticles[i]->position.z = 0.0f;
    //        //}
    //    }

    //}

 

    return;
}

void cSoftBodyVerlet::ApplyCollision(double deltaTime)
{
    float grounfOffset = 0.6f;

    if (tag == "tractor")
    {
        for (sParticle* pCurrentParticle : vec_pParticles)
        {
            if (pCurrentParticle->isBottomParticle)
            {
                float lowestY = oceanMesh->GetVertexYByXAndZ(pCurrentParticle->position.x, pCurrentParticle->position.z);

                if (pCurrentParticle->position.y < lowestY + grounfOffset)
                {
                    pCurrentParticle->position.y = lowestY + grounfOffset;
                }
            }
        }
    }

    if (tag == "chain1")
    {
        for (sParticle* pCurrentParticle : vec_pParticles)
        {
            if (pCurrentParticle->isLeftBackHinge)
            {
                pCurrentParticle->position = pCurrentParticle->oldPosition;
            }
            else if (pCurrentParticle->isLeftFrontHinge)
            {
                pCurrentParticle->position = pCurrentParticle->oldPosition;
            }
        }
    }

    if (tag == "chain2")
    {
        for (sParticle* pCurrentParticle : vec_pParticles)
        {
            if (pCurrentParticle->isLeftBackHinge)
            {
                pCurrentParticle->position = pCurrentParticle->oldPosition;
            }
            else if (pCurrentParticle->isLeftFrontHinge)
            {
                pCurrentParticle->position = pCurrentParticle->oldPosition;
            }
        }
    }

    //	this->vec_pParticles[5'000]->position = glm::vec3(0.0f, 30.0f, 0.0f);

    // Collide with a sphere at 20 units above the origin
    //	with a radius of 5 units.
    // Check to see if this particle is inside this sphere...
    //for (sParticle* pCurrentParticle : vec_pParticles)
    //{
    //    glm::vec3 sphereCentre = glm::vec3(0.0f, 20.0f, 24.0f);
    //    float sphereRadius = 10.0f;

    //    float distanceToSphere = glm::distance(pCurrentParticle->position,
    //        sphereCentre);
    //    if (distanceToSphere < sphereRadius)
    //    {
    //        // it's 'inside' the sphere
    //        // Shift or slide the point along the ray from the centre of the sphere
    //        glm::vec3 particleToCentreRay = pCurrentParticle->position - sphereCentre;
    //        // Normalize to get the direction
    //        particleToCentreRay = glm::normalize(particleToCentreRay);
    //        // 
    //        pCurrentParticle->position = (particleToCentreRay * sphereRadius) + sphereCentre;

    //    }
    //}//for (sParticle* pCurrentParticle
    ////this->vec_pParticles[0]->position = glm::vec3(0.0f, 0.0f, 0.0f);
}

void cSoftBodyVerlet::SelectBottomParticles()
{
    for (sParticle* pCurrentParticle : vec_pParticles)
    {
        if (pCurrentParticle->position.y < 3.4f)
        {
            pCurrentParticle->isBottomParticle = true;
        }
    }

}

void cSoftBodyVerlet::SelectHinges()
{
    for (sParticle* pCurrentParticle : vec_pParticles)
    {
        if (pCurrentParticle->position.z < -4.0f)
        {
            pCurrentParticle->isLeftBackHinge = true;
        }

        if (pCurrentParticle->position.z > 11.0f)
        {
            pCurrentParticle->isLeftFrontHinge = true;
        }
    }

}

void cSoftBodyVerlet::CleanZeros(glm::vec3& value)
{
    const float minFloat = 1.192092896e-07f;
    if ((value.x < minFloat) && (value.x > -minFloat))
    {
        value.x = 0.0f;
    }
    if ((value.y < minFloat) && (value.y > -minFloat))
    {
        value.y = 0.0f;
    }
    if ((value.z < minFloat) && (value.z > -minFloat))
    {
        value.z = 0.0f;
    }
}

float cSoftBodyVerlet::CalcDistanceBetween(sParticle* particleA, sParticle* particleB)
{
    return glm::distance(particleA->position, particleB->position);
}

float cSoftBodyVerlet::GetHighestPointWithBoundsZ(float zValueMin, float zValueMax)
{
    std::vector<sParticle> particlesInSection;

    for (sParticle* pCurrentParticle : vec_pParticles)
    {
        if ((pCurrentParticle->position.z > zValueMin) && (pCurrentParticle->position.z < zValueMax))
        {
            particlesInSection.push_back(*pCurrentParticle);
        }
    }

    float yPosition = vec_pParticles[0]->position.y;
    for (const sParticle particle : particlesInSection)
    {
        if (yPosition < particle.position.y)
        {
            yPosition = particle.position.y;
        }
    }

    return yPosition;
}