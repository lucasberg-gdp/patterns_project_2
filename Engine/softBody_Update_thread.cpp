#include "cSoftBodyVerlet.h"

#ifdef APIENTRY
#undef APIENTRY
#endif
#include <windows.h>

#include "sSoftBodyThreadInfo.h"

CRITICAL_SECTION g_AddNumbersTotal_CS;

DWORD __stdcall UpdateSoftBodyThread(LPVOID lpParameter)
{
    sSoftBodyThreadInfo* pSBTInfo = (sSoftBodyThreadInfo*)lpParameter;

    double lastFrameTime = glfwGetTime();
    double totalElapsedFrameTime = 0.0;

    DWORD sleepTime_ms = 1;

    while (pSBTInfo->bIsAlive)
    {
        if (pSBTInfo->bRun)
        {
            // Adjust sleep time based on actual framerate
            double currentTime = glfwGetTime();
            double deltaTime = currentTime - lastFrameTime;
            lastFrameTime = currentTime;

            totalElapsedFrameTime += deltaTime;

            if (totalElapsedFrameTime >= pSBTInfo->desiredUpdateTime)
            {
                // Reset the elapsed time
                totalElapsedFrameTime = 0.0;

                // Update the object

                pSBTInfo->p_theSoftBody->VerletUpdate(pSBTInfo->desiredUpdateTime);
                pSBTInfo->p_theSoftBody->ApplyCollision(pSBTInfo->desiredUpdateTime);
                pSBTInfo->p_theSoftBody->SatisfyConstraints();

                // These two update the pVertices array
                pSBTInfo->p_theSoftBody->UpdateVertexPositions();
                pSBTInfo->p_theSoftBody->UpdateNormals();

            }

            // Maybe something small like 1ms delay?
            Sleep(0);

        }//if ( pSBTInfo->bRun )
    }//while (pSBTInfo->bIsAlive)

    return 0;
}

void UpdateSoftBody(cSoftBodyVerlet& theSoftBody,double deltaTime)
{
    theSoftBody.VerletUpdate(deltaTime);
    // 
    theSoftBody.ApplyCollision(deltaTime);
    // 
    theSoftBody.SatisfyConstraints();

    //    for (cSoftBodyVerlet::sParticle* pCurParticle : ::g_SoftBody.vec_pParticles)
    //    {
    //        ::g_DrawDebugSphere(pCurParticle->position, 0.01f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    //    }

        // Update the mesh in the VAO based on the current particle positions
    theSoftBody.UpdateVertexPositions();
    theSoftBody.UpdateNormals();

    return;
}
