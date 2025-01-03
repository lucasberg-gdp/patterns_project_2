#include "cSkyBox.h"
#include "cMesh.h"
#include "iCamera.h"
#include "Engine.h"

extern iCamera* g_camera;
//extern void DrawObject(cMesh* pCurrentMesh, glm::mat4 matModel, GLuint shaderProgramID);

extern Engine engine;

cSkyBox::cSkyBox()
{

}

void cSkyBox::DrawSkyBox(GLuint shaderProgramID, double deltaTime)
{
    // HACK: I'm making this here, but hey...
    cMesh theSkyBox;
    theSkyBox.meshName = "shapes/Sphere_1_unit_Radius_xyz_n_rgba_uv.ply";
    theSkyBox.setUniformDrawScale(55'000.0f);
    theSkyBox.setDrawPosition(g_camera->GetPosition());
    //            theSkyBox.bIsWireframe = true;

                // Depth test
    //            glDisable(GL_DEPTH_TEST);       // Writes no matter what
                // Write to depth buffer (depth mask)
    //            glDepthMask(GL_FALSE);          // Won't write to the depth buffer

                // uniform bool bIsSkyBox;
    GLint bIsSkyBox_UL = glGetUniformLocation(shaderProgramID, "bIsSkyBox");
    glUniform1f(bIsSkyBox_UL, (GLfloat)GL_TRUE);

    // The normals for this sphere are facing "out" but we are inside the sphere
    glCullFace(GL_FRONT);

    engine.DrawObject(&theSkyBox, glm::mat4(1.0f), shaderProgramID, deltaTime);

    glUniform1f(bIsSkyBox_UL, (GLfloat)GL_FALSE);

    // Put the culling back to "normal" (back facing are not drawn)
    glCullFace(GL_BACK);
}
