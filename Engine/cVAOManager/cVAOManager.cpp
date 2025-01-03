#include "cVAOManager.h"

#include "../OpenGLCommon.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include "../cAnimationSystem.h"

#define PRINT_SPACES(x) {int s = x; while(s--> 0) printf(" ");}

Assimp::Importer m_AssimpImporter;

extern cAnimationSystem* g_animationSystem;

void cVAOManager::setBasePath(std::string basePathWithoutSlash)
{
    this->m_basePathWithoutSlash = basePathWithoutSlash;
    return;
}

std::string cVAOManager::getBasePath(void)
{
    return this->m_basePathWithoutSlash;
}

bool cVAOManager::LoadModelIntoVAO(std::string fileName,
    unsigned int shaderProgramID)
{
    sModelDrawInfo tempModelInfo;
    return this->LoadModelIntoVAO(fileName, tempModelInfo, shaderProgramID);
}


bool cVAOManager::LoadModelIntoVAO(
    std::string fileName,
    sModelDrawInfo& drawInfo,
    unsigned int shaderProgramID,
    bool bIsDynamicBuffer /*=false*/)
{
    // Load the model from file
    // (We do this here, since if we can't load it, there's 
    //	no point in doing anything else, right?)

    drawInfo.meshName = fileName;
    std::string fileAndPath = this->m_basePathWithoutSlash + "/" + fileName;

    //    if ( ! this->m_LoadTheFile_Ply_XYZ_N_RGBA(fileAndPath, drawInfo) )
    if (!this->m_LoadTheFile_Ply_XYZ_N_RGBA_UV(fileAndPath, drawInfo))
    {
        return false;
    };


    // ***********************************************************
    // TODO: Load the model from file

    // ***********************************************************
    // 
    // 
    // Model is loaded and the vertices and indices are in the drawInfo struct
    // 

    // Create a VAO (Vertex Array Object), which will 
    //	keep track of all the 'state' needed to draw 
    //	from this buffer...

    // Ask OpenGL for a new buffer ID...
    glGenVertexArrays(1, &(drawInfo.VAO_ID));
    // "Bind" this buffer:
    // - aka "make this the 'current' VAO buffer
    glBindVertexArray(drawInfo.VAO_ID);

    // Now ANY state that is related to vertex or index buffer
    //	and vertex attribute layout, is stored in the 'state' 
    //	of the VAO... 


    // NOTE: OpenGL error checks have been omitted for brevity
//	glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &(drawInfo.VertexBufferID));

    //	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, drawInfo.VertexBufferID);
    // sVert vertices[3]

    // This is updated for bIsDynamicBuffer so:
    // * if true, then it's GL_DYNAMIC_DRAW
    // * if false, then it's GL_STATIC_DRAW
    // 
    // Honestly, it's not that Big Of A Deal in that you can still update 
    //  a buffer if it's set to STATIC, but in theory this will take longer.
    // Does it really take longer? Who knows?
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(sVertex) * drawInfo.numberOfVertices,	// ::g_NumberOfVertsToDraw,	// sizeof(vertices), 
        (GLvoid*)drawInfo.pVertices,							// pVertices,			//vertices, 
        (bIsDynamicBuffer ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));


    // Copy the index buffer into the video card, too
    // Create an index buffer.
    glGenBuffers(1, &(drawInfo.IndexBufferID));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.IndexBufferID);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,			// Type: Index element array
        sizeof(unsigned int) * drawInfo.numberOfIndices,
        (GLvoid*)drawInfo.pIndices,
        GL_STATIC_DRAW);

    // Set the vertex attributes.

    GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPos");	// program
    GLint vcol_location = glGetAttribLocation(shaderProgramID, "vCol");	// program;
    GLint vNormal_location = glGetAttribLocation(shaderProgramID, "vNormal");	// program;
    // AND the texture coordinate
    GLint vTextureCoords_location = glad_glGetAttribLocation(shaderProgramID, "vTextureCoords");    // in vec2 vTextureCoords;		// NOTE this is a vec2 not vec4;		// NOTE this is a vec2 not vec4

    GLint vBones_Id = glad_glGetAttribLocation(shaderProgramID, "vBoneIds");    // in vec2 vTextureCoords;		// NOTE this is a vec2 not vec4;		// NOTE this is a vec2 not vec4

    // Set the vertex attributes for this shader
    glEnableVertexAttribArray(vpos_location);	    // vPos
    glVertexAttribPointer(vpos_location, 4,		// vPos
        GL_FLOAT, GL_FALSE,
        sizeof(sVertex),
        (void*)offsetof(sVertex, x));

    glEnableVertexAttribArray(vcol_location);	    // vCol
    glVertexAttribPointer(vcol_location, 4,		// vCol
        GL_FLOAT, GL_FALSE,
        sizeof(sVertex),
        (void*)offsetof(sVertex, r));

    glEnableVertexAttribArray(vNormal_location);	// vNormal
    glVertexAttribPointer(vNormal_location, 4,		// vNormal
        GL_FLOAT, GL_FALSE,
        sizeof(sVertex),
        (void*)offsetof(sVertex, nx));

    glEnableVertexAttribArray(vTextureCoords_location);	    // vTextureCoords;
    glVertexAttribPointer(vTextureCoords_location, 2,		// in vec2 vTextureCoords;
        GL_FLOAT, GL_FALSE,
        sizeof(sVertex),
        (void*)offsetof(sVertex, u));

    glEnableVertexAttribArray(vBones_Id);	    // vTextureCoords;
    glVertexAttribPointer(vBones_Id, 4,		// in vec2 vTextureCoords;
        GL_FLOAT, GL_FALSE,
        sizeof(sVertex),
        (void*)offsetof(sVertex, bones1));

    // Now that all the parts are set up, set the VAO to zero
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(vpos_location);
    glDisableVertexAttribArray(vcol_location);
    glDisableVertexAttribArray(vNormal_location);
    glDisableVertexAttribArray(vTextureCoords_location);        // <-- New
    glDisableVertexAttribArray(vBones_Id);

    // Store the draw information into the map
    this->m_map_ModelName_to_VAOID[drawInfo.meshName] = drawInfo;


    return true;
}

bool cVAOManager::LoadModelIntoVAOWithoutTexture(std::string fileName, sModelDrawInfo& drawInfo, unsigned int shaderProgramID, bool bIsDynamicBuffer)
{
    // Load the model from file
       // (We do this here, since if we can't load it, there's 
       //	no point in doing anything else, right?)

    drawInfo.meshName = fileName;
    std::string fileAndPath = this->m_basePathWithoutSlash + "/" + fileName;

    if ( ! this->m_LoadTheFile_Ply_XYZ_N_RGBA(fileAndPath, drawInfo) )
    {
        return false;
    };

    // ***********************************************************
    // TODO: Load the model from file

    // ***********************************************************
    // 
    // 
    // Model is loaded and the vertices and indices are in the drawInfo struct
    // 

    // Create a VAO (Vertex Array Object), which will 
    //	keep track of all the 'state' needed to draw 
    //	from this buffer...

    // Ask OpenGL for a new buffer ID...
    glGenVertexArrays(1, &(drawInfo.VAO_ID));
    // "Bind" this buffer:
    // - aka "make this the 'current' VAO buffer
    glBindVertexArray(drawInfo.VAO_ID);

    // Now ANY state that is related to vertex or index buffer
    //	and vertex attribute layout, is stored in the 'state' 
    //	of the VAO... 


    // NOTE: OpenGL error checks have been omitted for brevity
//	glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &(drawInfo.VertexBufferID));

    //	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, drawInfo.VertexBufferID);
    // sVert vertices[3]

    // This is updated for bIsDynamicBuffer so:
    // * if true, then it's GL_DYNAMIC_DRAW
    // * if false, then it's GL_STATIC_DRAW
    // 
    // Honestly, it's not that Big Of A Deal in that you can still update 
    //  a buffer if it's set to STATIC, but in theory this will take longer.
    // Does it really take longer? Who knows?
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(sVertex) * drawInfo.numberOfVertices,	// ::g_NumberOfVertsToDraw,	// sizeof(vertices), 
        (GLvoid*)drawInfo.pVertices,							// pVertices,			//vertices, 
        (bIsDynamicBuffer ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));


    // Copy the index buffer into the video card, too
    // Create an index buffer.
    glGenBuffers(1, &(drawInfo.IndexBufferID));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.IndexBufferID);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,			// Type: Index element array
        sizeof(unsigned int) * drawInfo.numberOfIndices,
        (GLvoid*)drawInfo.pIndices,
        GL_STATIC_DRAW);

    // Set the vertex attributes.

    GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPos");	// program
    GLint vcol_location = glGetAttribLocation(shaderProgramID, "vCol");	// program;
    GLint vNormal_location = glGetAttribLocation(shaderProgramID, "vNormal");	// program;
    // AND the texture coordinate
    //GLint vTextureCoords_location = glad_glGetAttribLocation(shaderProgramID, "vTextureCoords");    // in vec2 vTextureCoords;		// NOTE this is a vec2 not vec4;		// NOTE this is a vec2 not vec4

    // Set the vertex attributes for this shader
    glEnableVertexAttribArray(vpos_location);	    // vPos
    glVertexAttribPointer(vpos_location, 4,		// vPos
        GL_FLOAT, GL_FALSE,
        sizeof(sVertex),
        (void*)offsetof(sVertex, x));

    glEnableVertexAttribArray(vcol_location);	    // vCol
    glVertexAttribPointer(vcol_location, 4,		// vCol
        GL_FLOAT, GL_FALSE,
        sizeof(sVertex),
        (void*)offsetof(sVertex, r));

    glEnableVertexAttribArray(vNormal_location);	// vNormal
    glVertexAttribPointer(vNormal_location, 4,		// vNormal
        GL_FLOAT, GL_FALSE,
        sizeof(sVertex),
        (void*)offsetof(sVertex, nx));

    //glEnableVertexAttribArray(vTextureCoords_location);	    // vTextureCoords;
    //glVertexAttribPointer(vTextureCoords_location, 2,		// in vec2 vTextureCoords;
    //    GL_FLOAT, GL_FALSE,
    //    sizeof(sVertex),
    //    (void*)offsetof(sVertex, u));

    // Now that all the parts are set up, set the VAO to zero
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(vpos_location);
    glDisableVertexAttribArray(vcol_location);
    glDisableVertexAttribArray(vNormal_location);
    //glDisableVertexAttribArray(vTextureCoords_location);        // <-- New

    // Store the draw information into the map
    this->m_map_ModelName_to_VAOID[drawInfo.meshName] = drawInfo;


    return true;
}

// We don't want to return an int, likely
bool cVAOManager::FindDrawInfoByModelName(
    std::string filename,
    sModelDrawInfo& drawInfo)
{
    std::map< std::string /*model name*/,
        sModelDrawInfo /* info needed to draw*/ >::iterator
        itDrawInfo = this->m_map_ModelName_to_VAOID.find(filename);

    // Find it? 
    if (itDrawInfo == this->m_map_ModelName_to_VAOID.end())
    {
        // Nope
        return false;
    }

    // Else we found the thing to draw
    // ...so 'return' that information
    drawInfo = itDrawInfo->second;
    return true;
}

const char** cVAOManager::GetNameOfAllModels()
{
    unsigned int size = 0;

    for (const std::pair<const std::string, sModelDrawInfo>& pair : m_map_ModelName_to_VAOID)
    {
        size += (unsigned int)pair.first.size();
    }

    const char** items = new const char* [size];
    unsigned int index = 0;
    for (const std::pair<const std::string, sModelDrawInfo>& pair : m_map_ModelName_to_VAOID)
    {
        items[index] = pair.first.c_str();
        index++;
    }

    return items;
}

unsigned int cVAOManager::GetNumberOfModels()
{
    return (unsigned int)m_map_ModelName_to_VAOID.size();
}

const char** cVAOManager::GetTypesOfModels()
{
    const char** modelTypesCharArray = new const char* [modelInfo.size()];

    for (unsigned int i = 0; i < modelInfo.size(); ++i)
    {
        modelTypesCharArray[i] = modelInfo[i].modelFolderName.c_str();
    }

    return modelTypesCharArray;
}

unsigned int cVAOManager::GetNumberOfModelTypes()
{
    return (unsigned int)modelInfo.size();
}

const char** cVAOManager::GetNameOfAllModelsByType(const char* type)
{
    unsigned int size = 0;

    for (unsigned int i = 0; i < modelInfo.size(); i++)
    {
        if (modelInfo[i].modelFolderName.c_str() == type)
        {
            const char** modelTypesCharArray = new const char* [modelInfo[i].modelFiles.size()];

            for (unsigned int j = 0; j < modelInfo[i].modelFiles.size(); ++j)
            {
                modelTypesCharArray[j] = modelInfo[i].modelFiles[j].c_str();
            }

            return modelTypesCharArray;
        }
    }

    return nullptr;
}

unsigned int cVAOManager::GetNumberOfModelsOfType(const char* type)
{
    for (unsigned int i = 0; i < modelInfo.size(); i++)
    {
        if (modelInfo[i].modelFolderName.c_str() == type)
        {
            unsigned int numberOfFilesInFolder = (unsigned int)modelInfo[i].modelFiles.size();

            return numberOfFilesInFolder;
        }
    }

    return 0;
}

std::vector<std::string> cVAOManager::GetModelsByType(std::string type)
{
    for (unsigned int i = 0; i < modelInfo.size(); ++i)
    {
        if (modelInfo[i].modelFolderName == type)
        {
            return modelInfo[i].modelFiles;
        }
    }

    return std::vector<std::string>();
}

bool cVAOManager::m_LoadTheFile_Ply_XYZ_N_RGBA(std::string theFileName, sModelDrawInfo& drawInfo)
{

    //sVertexPlyFile p;       p.x = 0.0f;     p.y = 1.0f; p.z = 2.0f;
    //std::cout << p.x;

    //sVertexPlyFile q;
    //std::cout << "Type in the x: ";
    //std::cin >> q.x;

    // Input Filestream 
    //    std::ifstream theBunnyFile("bathtub.ply");
    std::ifstream theBunnyFile(theFileName.c_str());
    if (!theBunnyFile.is_open())
    {
        // didn't open the file.
        return false;
    }

    std::string temp;
    while (theBunnyFile >> temp)
    {
        if (temp == "vertex")
        {
            break;
        }
    };
    //element vertex 8171
//    theBunnyFile >> g_numberOfVertices;
    theBunnyFile >> drawInfo.numberOfVertices;


    while (theBunnyFile >> temp)
    {
        if (temp == "face")
        {
            break;
        }
    };
    //element vertex 8171
//    theBunnyFile >> g_numberOfTriangles;
    theBunnyFile >> drawInfo.numberOfTriangles;

    drawInfo.numberOfIndices = drawInfo.numberOfTriangles * 3;

    while (theBunnyFile >> temp)
    {
        if (temp == "end_header")
        {
            break;
        }
    };

    // This most closely matches the ply file for the bunny
    struct sVertexPlyFile
    {
        float x;
        float y;
        float z;
        float nx, ny, nz;
        float r, g, b, a;
    };

    struct sTrianglePlyFile
    {
        //    sVertexPlyFile verts[3];
        //    unsigned int vertIndexes[3];
            // Vertices of the triangles
        unsigned int v0, v1, v2;
    };

    // Dynamically allocate memory on the heap;
    // sVertexPlyFile* pTheVerticesFile = new sVertexPlyFile[g_numberOfVertices];
    sVertexPlyFile* pTheVerticesFile = new sVertexPlyFile[drawInfo.numberOfVertices];

    // -0.036872 0.127727 0.00440925 
    // for (unsigned int index = 0; index != g_numberOfVertices; index++)
    for (unsigned int index = 0; index != drawInfo.numberOfVertices; index++)
    {
        sVertexPlyFile tempVertex;
        theBunnyFile >> tempVertex.x;                //std::cin >> a.x;
        theBunnyFile >> tempVertex.y;                //std::cin >> a.y;
        theBunnyFile >> tempVertex.z;                //std::cin >> a.z;

        theBunnyFile >> tempVertex.nx;
        theBunnyFile >> tempVertex.ny;
        theBunnyFile >> tempVertex.nz;

        theBunnyFile >> tempVertex.r;       tempVertex.r /= 255.0f;
        theBunnyFile >> tempVertex.g;       tempVertex.g /= 255.0f;
        theBunnyFile >> tempVertex.b;       tempVertex.b /= 255.0f;
        theBunnyFile >> tempVertex.a;       tempVertex.a /= 255.0f;


        pTheVerticesFile[index] = tempVertex;
    }


    // sTrianglePlyFile* pTheTriangles = new sTrianglePlyFile[g_numberOfTriangles];
    sTrianglePlyFile* pTheTriangles = new sTrianglePlyFile[drawInfo.numberOfTriangles];

    // 3 3495 3549 3548 
    for (unsigned int index = 0; index != drawInfo.numberOfTriangles; index++)
    {
        sTrianglePlyFile tempTriangle;

        unsigned int discard;
        theBunnyFile >> discard;            // 3
        theBunnyFile >> tempTriangle.v0;                //std::cin >> a.x;
        theBunnyFile >> tempTriangle.v1;                //std::cin >> a.y;
        theBunnyFile >> tempTriangle.v2;                //std::cin >> a.z;

        pTheTriangles[index] = tempTriangle;
    }

    // Before, we manually looked up the vertex indices and added them to the vertex buffer
    //g_NumberOfVerticesToDraw = g_numberOfTriangles * 3;
    //pVertices = new sVertex[g_NumberOfVerticesToDraw];

    //unsigned int vertIndex = 0;
    //for (unsigned int triIndex = 0; triIndex != g_numberOfTriangles; triIndex++)
    //{
    //    // 3 1582 1581 2063 
    //    pVertices[vertIndex + 0].x = pTheVerticesFile[pTheTriangles[triIndex].v0].x;
    //    pVertices[vertIndex + 0].y = pTheVerticesFile[pTheTriangles[triIndex].v0].y;
    //    pVertices[vertIndex + 0].z = pTheVerticesFile[pTheTriangles[triIndex].v0].z;

    //    pVertices[vertIndex + 0].r = pTheVerticesFile[pTheTriangles[triIndex].v0].r;
    //    pVertices[vertIndex + 0].g = pTheVerticesFile[pTheTriangles[triIndex].v0].g;
    //    pVertices[vertIndex + 0].b = pTheVerticesFile[pTheTriangles[triIndex].v0].b;

    //    pVertices[vertIndex + 1].x = pTheVerticesFile[pTheTriangles[triIndex].v1].x;
    //    pVertices[vertIndex + 1].y = pTheVerticesFile[pTheTriangles[triIndex].v1].y;
    //    pVertices[vertIndex + 1].z = pTheVerticesFile[pTheTriangles[triIndex].v1].z;

    //    pVertices[vertIndex + 1].r = pTheVerticesFile[pTheTriangles[triIndex].v1].r;
    //    pVertices[vertIndex + 1].g = pTheVerticesFile[pTheTriangles[triIndex].v1].g;
    //    pVertices[vertIndex + 1].b = pTheVerticesFile[pTheTriangles[triIndex].v1].b;

    //    pVertices[vertIndex + 2].x = pTheVerticesFile[pTheTriangles[triIndex].v2].x;
    //    pVertices[vertIndex + 2].y = pTheVerticesFile[pTheTriangles[triIndex].v2].y;
    //    pVertices[vertIndex + 2].z = pTheVerticesFile[pTheTriangles[triIndex].v2].z;

    //    pVertices[vertIndex + 2].r = pTheVerticesFile[pTheTriangles[triIndex].v2].r;
    //    pVertices[vertIndex + 2].g = pTheVerticesFile[pTheTriangles[triIndex].v2].g;
    //    pVertices[vertIndex + 2].b = pTheVerticesFile[pTheTriangles[triIndex].v2].b;

    //    vertIndex += 3;
    //}

    // ... now we just copy the vertices from the file as is (unchanged)
    drawInfo.pVertices = new sVertex[drawInfo.numberOfVertices];
    for (unsigned int vertIndex = 0; vertIndex != drawInfo.numberOfVertices; vertIndex++)
    {
        // 3 1582 1581 2063 
        drawInfo.pVertices[vertIndex].x = pTheVerticesFile[vertIndex].x;
        drawInfo.pVertices[vertIndex].y = pTheVerticesFile[vertIndex].y;
        drawInfo.pVertices[vertIndex].z = pTheVerticesFile[vertIndex].z;
        drawInfo.pVertices[vertIndex].w = 1.0f;


        drawInfo.pVertices[vertIndex].nx = pTheVerticesFile[vertIndex].nx;
        drawInfo.pVertices[vertIndex].ny = pTheVerticesFile[vertIndex].ny;
        drawInfo.pVertices[vertIndex].nz = pTheVerticesFile[vertIndex].nz;
        drawInfo.pVertices[vertIndex].nw = 1.0f;

        drawInfo.pVertices[vertIndex].r = pTheVerticesFile[vertIndex].r;
        drawInfo.pVertices[vertIndex].g = pTheVerticesFile[vertIndex].g;
        drawInfo.pVertices[vertIndex].b = pTheVerticesFile[vertIndex].b;
        drawInfo.pVertices[vertIndex].a = pTheVerticesFile[vertIndex].a;
    }

    // Allocate an array for all the indices (which is 3x the number of triangles)
    // Element array is an 1D array of integers
    drawInfo.pIndices = new unsigned int[drawInfo.numberOfIndices];

    unsigned int elementIndex = 0;
    for (unsigned int triIndex = 0; triIndex != drawInfo.numberOfTriangles; triIndex++)
    {
        // 3 1582 1581 2063 
        drawInfo.pIndices[elementIndex + 0] = pTheTriangles[triIndex].v0;
        drawInfo.pIndices[elementIndex + 1] = pTheTriangles[triIndex].v1;
        drawInfo.pIndices[elementIndex + 2] = pTheTriangles[triIndex].v2;

        elementIndex += 3;      // Next "triangle"
    }

    return true;
}

bool cVAOManager::m_LoadTheFile_Ply_XYZ_N_RGBA_UV(std::string fileName, sModelDrawInfo& drawInfo)
{
    std::ifstream plyFile(fileName.c_str());
    if (!plyFile.is_open())
    {
        // didn't open the file.
        return false;
    }

    std::string temp;
    while (plyFile >> temp)
    {
        if (temp == "vertex")
        {
            break;
        }
    };
    //element vertex 8171
//    theBunnyFile >> g_numberOfVertices;
    plyFile >> drawInfo.numberOfVertices;


    while (plyFile >> temp)
    {
        if (temp == "face")
        {
            break;
        }
    };
    //element vertex 8171
//    theBunnyFile >> g_numberOfTriangles;
    plyFile >> drawInfo.numberOfTriangles;

    drawInfo.numberOfIndices = drawInfo.numberOfTriangles * 3;

    while (plyFile >> temp)
    {
        if (temp == "end_header")
        {
            break;
        }
    };

    // This most closely matches the ply file for the bunny


    struct sTrianglePlyFile
    {
        unsigned int v0, v1, v2;
    };

    // Dynamically allocate memory on the heap;
    sVertexPlyFileWithTexture* pTheVerticesFile = new sVertexPlyFileWithTexture[drawInfo.numberOfVertices];

    // -0.036872 0.127727 0.00440925 
//    for (unsigned int index = 0; index != g_numberOfVertices; index++)
    for (unsigned int index = 0; index != drawInfo.numberOfVertices; index++)
    {
        sVertexPlyFileWithTexture tempVertex;
        plyFile >> tempVertex.x;                //std::cin >> a.x;
        plyFile >> tempVertex.y;                //std::cin >> a.y;
        plyFile >> tempVertex.z;                //std::cin >> a.z;

        plyFile >> tempVertex.nx;
        plyFile >> tempVertex.ny;
        plyFile >> tempVertex.nz;

        plyFile >> tempVertex.r;       tempVertex.r /= 255.0f;
        plyFile >> tempVertex.g;       tempVertex.g /= 255.0f;
        plyFile >> tempVertex.b;       tempVertex.b /= 255.0f;
        plyFile >> tempVertex.a;       tempVertex.a /= 255.0f;

        // ALSO load the UV coordinates
        plyFile >> tempVertex.u;
        plyFile >> tempVertex.v;

        pTheVerticesFile[index] = tempVertex;
    }

    //    sTrianglePlyFile* pTheTriangles = new sTrianglePlyFile[g_numberOfTriangles];
    sTrianglePlyFile* pTheTriangles = new sTrianglePlyFile[drawInfo.numberOfTriangles];

    // 3 3495 3549 3548 
    for (unsigned int index = 0; index != drawInfo.numberOfTriangles; index++)
    {
        sTrianglePlyFile tempTriangle;

        unsigned int discard;
        plyFile >> discard;            // 3
        plyFile >> tempTriangle.v0;                //std::cin >> a.x;
        plyFile >> tempTriangle.v1;                //std::cin >> a.y;
        plyFile >> tempTriangle.v2;                //std::cin >> a.z;

        pTheTriangles[index] = tempTriangle;
    }

    //std::vector<int> verticesCount;
    //verticesCount.resize(5);

    // ... now we just copy the vertices from the file as is (unchanged)
    drawInfo.pVertices = new sVertex[drawInfo.numberOfVertices];
    for (unsigned int vertIndex = 0; vertIndex != drawInfo.numberOfVertices; vertIndex++)
    {
        // 3 1582 1581 2063 
        drawInfo.pVertices[vertIndex].x = pTheVerticesFile[vertIndex].x;
        drawInfo.pVertices[vertIndex].y = pTheVerticesFile[vertIndex].y;
        drawInfo.pVertices[vertIndex].z = pTheVerticesFile[vertIndex].z;
        drawInfo.pVertices[vertIndex].w = 1.0f;

        drawInfo.pVertices[vertIndex].nx = pTheVerticesFile[vertIndex].nx;
        drawInfo.pVertices[vertIndex].ny = pTheVerticesFile[vertIndex].ny;
        drawInfo.pVertices[vertIndex].nz = pTheVerticesFile[vertIndex].nz;
        drawInfo.pVertices[vertIndex].nw = 1.0f;

        drawInfo.pVertices[vertIndex].r = pTheVerticesFile[vertIndex].r;
        drawInfo.pVertices[vertIndex].g = pTheVerticesFile[vertIndex].g;
        drawInfo.pVertices[vertIndex].b = pTheVerticesFile[vertIndex].b;
        drawInfo.pVertices[vertIndex].a = pTheVerticesFile[vertIndex].a;

        // Copy the UV coords to the array that will eventually be copied to the GPU
        drawInfo.pVertices[vertIndex].u = pTheVerticesFile[vertIndex].u;
        drawInfo.pVertices[vertIndex].v = pTheVerticesFile[vertIndex].v;

        //// Assign bones index
        //if (drawInfo.pVertices[vertIndex].y < 1.0f)
        //{
        //    drawInfo.pVertices[vertIndex].bones1 = 0;
        //    verticesCount[0]++;
        //}
        //else if (drawInfo.pVertices[vertIndex].y < 2.0f)
        //{
        //    drawInfo.pVertices[vertIndex].bones1 = 1;
        //    verticesCount[1]++;
        //}
        //else if (drawInfo.pVertices[vertIndex].y < 3.0f)
        //{
        //    drawInfo.pVertices[vertIndex].bones1 = 2;
        //    verticesCount[2]++;
        //}
        //else if (drawInfo.pVertices[vertIndex].y < 4.0f)
        //{
        //    drawInfo.pVertices[vertIndex].bones1 = 3;
        //    verticesCount[3]++;
        //}
        //else
        //{
        //    drawInfo.pVertices[vertIndex].bones1 = 4;
        //    verticesCount[4]++;
        //}
    }

    ////std::cout << theFileName << std::endl;
    //for (int numVertices : verticesCount)
    //{
    //    //std::cout << numVertices << std::endl;
    //}

    // Allocate an array for all the indices (which is 3x the number of triangles)
    // Element array is an 1D array of integers
    drawInfo.pIndices = new unsigned int[drawInfo.numberOfIndices];

    unsigned int elementIndex = 0;
    for (unsigned int triIndex = 0; triIndex != drawInfo.numberOfTriangles; triIndex++)
    {
        // 3 1582 1581 2063 
        drawInfo.pIndices[elementIndex + 0] = pTheTriangles[triIndex].v0;
        drawInfo.pIndices[elementIndex + 1] = pTheTriangles[triIndex].v1;
        drawInfo.pIndices[elementIndex + 2] = pTheTriangles[triIndex].v2;

        elementIndex += 3;      // Next "triangle"
    }

    return true;
}

bool cVAOManager::FindDrawInfoPointerByModelName(std::string fileName, sModelDrawInfo*& drawInfo, unsigned int shaderProgramID)
{
    drawInfo = &m_map_ModelName_to_VAOID[fileName];

    return true;
}

bool cVAOManager::UpdateVAOBuffers(std::string fileName,
    sModelDrawInfo& updatedDrawInfo,
    unsigned int shaderProgramID)
{
    // This exists? 
    sModelDrawInfo updatedDrawInfo_TEMP;
    if (!this->FindDrawInfoByModelName(fileName, updatedDrawInfo_TEMP))
    {
        // Didn't find this buffer
        return false;
    }


    glBindBuffer(GL_ARRAY_BUFFER, updatedDrawInfo.VertexBufferID);

    // Original call to create and copy the initial data:
    //     
    //    glBufferData(GL_ARRAY_BUFFER,
    //                 sizeof(sVertex) * updatedDrawInfo.numberOfVertices,	
    //                 (GLvoid*)updatedDrawInfo.pVertices,					
    //                 GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER,
        0,  // Offset
        sizeof(sVertex) * updatedDrawInfo.numberOfVertices,
        (GLvoid*)updatedDrawInfo.pVertices);

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, updatedDrawInfo.IndexBufferID);

    // Original call to create and copy the initial data:
    //     
    //    glBufferData(GL_ELEMENT_ARRAY_BUFFER,			// Type: Index element array
    //                 sizeof(unsigned int) * updatedDrawInfo.numberOfIndices,
    //                 (GLvoid*)updatedDrawInfo.pIndices,
    //                 GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
        0,  // Offset
        sizeof(unsigned int) * updatedDrawInfo.numberOfIndices,
        (GLvoid*)updatedDrawInfo.pIndices);

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    return true;
}

bool cVAOManager::UpdateVertexBuffers(std::string fileName, sModelDrawInfo& updatedDrawInfo, unsigned int shaderProgramID)
{
    // This exists? 
    sModelDrawInfo updatedDrawInfo_TEMP;
    if (!this->FindDrawInfoByModelName(fileName, updatedDrawInfo_TEMP))
    {
        // Didn't find this buffer
        return false;
    }


    glBindBuffer(GL_ARRAY_BUFFER, updatedDrawInfo.VertexBufferID);

    // Original call to create and copy the initial data:
    //     
    //    glBufferData(GL_ARRAY_BUFFER,
    //                 sizeof(sVertex) * updatedDrawInfo.numberOfVertices,	
    //                 (GLvoid*)updatedDrawInfo.pVertices,					
    //                 GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER,
        0,  // Offset
        sizeof(sVertex) * updatedDrawInfo.numberOfVertices,
        (GLvoid*)updatedDrawInfo.pVertices);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
}

void AssimpToGLM(const aiMatrix4x4& a, glm::mat4& g)
{
    g[0][0] = a.a1; g[0][1] = a.b1; g[0][2] = a.c1; g[0][3] = a.d1;
    g[1][0] = a.a2; g[1][1] = a.b2; g[1][2] = a.c2; g[1][3] = a.d2;
    g[2][0] = a.a3; g[2][1] = a.b3; g[2][2] = a.c3; g[2][3] = a.d3;
    g[3][0] = a.a4; g[3][1] = a.b4; g[3][2] = a.c4; g[3][3] = a.d4;
}

struct BoneWeightInfo
{
    BoneWeightInfo()
    {
        id[0] = 0;
        id[1] = 0;
        id[2] = 0;
        id[3] = 0;
        weight[0] = 0;
        weight[1] = 0;
        weight[2] = 0;
        weight[3] = 0;
    };

    int id[4];
    float weight[4];
};

Bones::Node* cVAOManager::CreateAnimNode(aiNode* node)
{
    Bones::Node* newNode = new Bones::Node(node->mName.C_Str());
    AssimpToGLM(node->mTransformation, newNode->Transformation);
    return newNode;
}

Bones::Node* cVAOManager::GenerateBoneHierarchy(aiNode* assimpNode, sModelDrawInfo& drawInfo, const int depth)
{
    Bones::Node* node = CreateAnimNode(assimpNode);
    //PRINT_SPACES(depth); printf("%s\n", assimpNode->mName.C_Str());
    aiMatrix4x4& transformation = assimpNode->mTransformation;
    aiVector3D position;
    aiQuaternion rotation;
    aiVector3D scaling;
    transformation.Decompose(scaling, rotation, position);
    //PRINT_SPACES(depth + 1); printf("Position: {%.3f, %.3f, %.3f}\n", position.x, position.y, position.z);
    //PRINT_SPACES(depth + 1); printf("Scaling: {%.3f, %.3f, %.3f}\n", scaling.x, scaling.y, scaling.z);
    //PRINT_SPACES(depth + 1); printf("Quaternion: {%.3f, %.3f, %.3f, %.3f}\n", rotation.x, rotation.y, rotation.z, rotation.w);
    //printf("\n");

    glm::mat4 glmMatrix;
    AssimpToGLM(transformation, glmMatrix);

    //glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.f), glm::vec3(position.x, position.y, position.z));
    //glm::mat4 RotationMatrix = glm::mat4_cast(glm::quat(rotation.w, rotation.x, rotation.y, rotation.z));
    //glm::mat4 ScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaling.x, scaling.y, scaling.z));

    //glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;
    drawInfo.NodeNameToIdMap.insert(std::pair<std::string, int>(assimpNode->mName.C_Str(), (int)(drawInfo.NodeHeirarchyTransformations.size())));

    //drawInfo.NodeNameToId.push_back(NodeNameToId(assimpNode->mName.C_Str(), (int)(drawInfo.NodeHeirarchyTransformations.size())));

    drawInfo.NodeHeirarchyTransformations.emplace_back(glmMatrix);

    for (unsigned int i = 0; i < assimpNode->mNumChildren; ++i)
    {
        node->Children.emplace_back(GenerateBoneHierarchy(assimpNode->mChildren[i], drawInfo, depth + 1));
    }

    return node;
}

bool cVAOManager::LoadModelWithAssimp(std::string fileName, sModelDrawInfo& drawInfo, unsigned int shaderProgramID, bool bIsDynamicBuffer)
{
    std::string fileAndPath = this->m_basePathWithoutSlash + "/" + fileName;

    const aiScene* scene = m_AssimpImporter.ReadFile(fileAndPath.c_str(), (aiProcess_Triangulate | aiProcess_GenNormals));

    std::cout << m_AssimpImporter.GetErrorString();

    drawInfo.meshName = fileName;
    drawInfo.VAO_ID = shaderProgramID;

    if (scene == nullptr)
    {
        std::cout << "Failed to load file: " << fileName << std::endl;
        return false;
    }

    std::vector<int> verticesCount;
    verticesCount.resize(5);

    if (scene->mNumMeshes > 1)
    {
        std::cout << "Too many meshes in one file" << std::endl;
        return false;
    }

    aiMesh* mesh = scene->mMeshes[0];

    drawInfo.RootNode = GenerateBoneHierarchy(scene->mRootNode, drawInfo);
    drawInfo.GlobalInverseTransformation = glm::inverse(drawInfo.RootNode->Transformation);

    std::vector<BoneWeightInfo> boneWeights;
        
    if (mesh->HasBones())
    {
        //std::cout << "mesh->mNumBones: " << mesh->mNumBones << std::endl;

        boneWeights.resize(mesh->mNumVertices);
        unsigned int numBones = mesh->mNumBones;

        for (unsigned int boneIdx = 0; boneIdx < numBones; ++boneIdx)
        {
            aiBone* bone = mesh->mBones[boneIdx];

            unsigned int numOfWeights = bone->mNumWeights;

            //std::string boneName(bone->mName.C_Str(), bone->mName.length);

            std::string boneName(bone->mName.C_Str());

            drawInfo.BoneNameToIdMap.insert(std::pair<std::string, int>(boneName, drawInfo.BoneInfoVec.size()));

            // Store the offset matrices
            Bones::BoneInfo info;
            AssimpToGLM(bone->mOffsetMatrix, info.BoneOffset);
            drawInfo.BoneInfoVec.emplace_back(info);

            //std::cout << "Bone: " << boneName << std::endl;
            //std::cout << "Number of weights: " << numOfWeights << std::endl;

            for (unsigned int weightIdx = 0; weightIdx < bone->mNumWeights; ++weightIdx)
            {
                aiVertexWeight& vertexWeight = bone->mWeights[weightIdx];
                // BoneId		:	boneIdx
                // Vertexid		:	vertexWeight.mVertexId
                // Weight		:	vertexWeight.mWeight

                BoneWeightInfo& boneInfo = boneWeights[vertexWeight.mVertexId];
                for (int infoIdx = 0; infoIdx < 4; ++infoIdx)
                {
                    if (boneInfo.weight[infoIdx] == 0.f)
                    {
                        boneInfo.id[infoIdx] = boneIdx;
                        boneInfo.weight[infoIdx] = vertexWeight.mWeight;
                        break;
                    }
                }
            }
        }
    }

    drawInfo.numberOfVertices = mesh->mNumVertices;
    drawInfo.numberOfTriangles = mesh->mNumFaces;

    drawInfo.numberOfIndices = mesh->mNumFaces * 3;

    drawInfo.pVertices = new sVertex[drawInfo.numberOfIndices * 10];
    drawInfo.pIndices = new GLuint[drawInfo.numberOfIndices * 10];
    drawInfo.pTriangles = new sTriangle[drawInfo.numberOfTriangles];

    std::vector<int> triangles;	// 1,2,3

    unsigned int vertArrayIndex = 0;

    for (unsigned int faceIdx = 0; faceIdx != mesh->mNumFaces; faceIdx++)
    {
        aiFace face = mesh->mFaces[faceIdx];

        aiVector3D vertex1 = mesh->mVertices[face.mIndices[0]];
        aiVector3D vertex2 = mesh->mVertices[face.mIndices[1]];
        aiVector3D vertex3 = mesh->mVertices[face.mIndices[2]];

        sTriangle faceTriangle;

        faceTriangle.vecA = glm::vec3(vertex1.x, vertex1.y, vertex1.z);
        faceTriangle.vecB = glm::vec3(vertex2.x, vertex2.y, vertex2.z);
        faceTriangle.vecC = glm::vec3(vertex3.x, vertex3.y, vertex3.z);

        drawInfo.pTriangles[faceIdx] = faceTriangle;

        for (int idx = 0; idx != 3; idx++)
        {
            unsigned int index = face.mIndices[idx];

            triangles.push_back(index);

            aiVector3D position = mesh->mVertices[index];
            drawInfo.pVertices[vertArrayIndex].x = position.x;
            drawInfo.pVertices[vertArrayIndex].y = position.y;
            drawInfo.pVertices[vertArrayIndex].z = position.z;
            drawInfo.pVertices[vertArrayIndex].w = 1.0f;

            if (mesh->HasTextureCoords(0))
            {
                aiVector3D textureCoord = mesh->mTextureCoords[0][index];
                drawInfo.pVertices[vertArrayIndex].u = textureCoord.x;
                drawInfo.pVertices[vertArrayIndex].v = textureCoord.y;
            }
            else
            {
                drawInfo.pVertices[vertArrayIndex].u = 0.0f;
                drawInfo.pVertices[vertArrayIndex].v = 0.0f;
            }

            aiVector3D normal = mesh->mNormals[index];
            drawInfo.pVertices[vertArrayIndex].nx = normal.x;
            drawInfo.pVertices[vertArrayIndex].ny = normal.y;
            drawInfo.pVertices[vertArrayIndex].nz = normal.z;
            drawInfo.pVertices[vertArrayIndex].nw = 0.0f;

            if (mesh->HasVertexColors(0))
            {
                //drawInfo.pVertices[vertArrayIndex].r = colors->r;
                //drawInfo.pVertices[vertArrayIndex].r /= 255.0f;
                //drawInfo.pVertices[vertArrayIndex].g = colors->g;
                //drawInfo.pVertices[vertArrayIndex].g /= 255.0f;
                //drawInfo.pVertices[vertArrayIndex].b = colors->b;
                //drawInfo.pVertices[vertArrayIndex].b /= 255.0f;
                //drawInfo.pVertices[vertArrayIndex].a = colors->a;
                //drawInfo.pVertices[vertArrayIndex].a /= 255.0f;

                drawInfo.pVertices[vertArrayIndex].r = 1.0f;
                drawInfo.pVertices[vertArrayIndex].g = 1.0f;
                drawInfo.pVertices[vertArrayIndex].b = 1.0f;
                drawInfo.pVertices[vertArrayIndex].a = 1.0f;
            }
            else
            {
                drawInfo.pVertices[vertArrayIndex].r = 1.0f;
                drawInfo.pVertices[vertArrayIndex].g = 1.0f;
                drawInfo.pVertices[vertArrayIndex].b = 1.0f;
                drawInfo.pVertices[vertArrayIndex].a = 1.0f;
            }

            if (mesh->HasBones())
            {
                BoneWeightInfo& boneInfo = boneWeights[index];

                drawInfo.pVertices[vertArrayIndex].bones1 = (float)boneInfo.id[0];
                drawInfo.pVertices[vertArrayIndex].bones2 = (float)boneInfo.id[1];
                drawInfo.pVertices[vertArrayIndex].bones3 = (float)boneInfo.id[2];
                drawInfo.pVertices[vertArrayIndex].bones4 = (float)boneInfo.id[3];

                drawInfo.pVertices[vertArrayIndex].boneWeight1 = boneInfo.weight[0];
                drawInfo.pVertices[vertArrayIndex].boneWeight2 = boneInfo.weight[1];
                drawInfo.pVertices[vertArrayIndex].boneWeight3 = boneInfo.weight[2];
                drawInfo.pVertices[vertArrayIndex].boneWeight4 = boneInfo.weight[3];

                float weight = boneInfo.weight[0] + boneInfo.weight[1] + boneInfo.weight[2] + boneInfo.weight[3];
                if (weight != 1.f)
                {
                    int breakhere = 0;
                }
            }

            drawInfo.pIndices[vertArrayIndex] = vertArrayIndex;

            vertArrayIndex++;
        }
    }

 
    for (unsigned int i = 0; i < scene->mNumMaterials; ++i) 
    {
        aiMaterial* material = scene->mMaterials[i];

        for (unsigned int j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
        {
            aiTextureType textureType = static_cast<aiTextureType>(j);
            aiString texturePath;

            if (material->GetTexture(textureType, 0, &texturePath) == AI_SUCCESS) 
            {
                //std::cout << "Texture type: " << textureType << ", Path: " << texturePath.C_Str() << std::endl;
                std::string textureName = texturePath.C_Str();
                size_t lastSeparatorPos = textureName.find_last_of("/\\");

                // Extract the substring after the last separator
                textureName = textureName.substr(lastSeparatorPos + 1);

                // Find the position of the dot (.) to remove the file extension
                size_t dotPos = textureName.find_last_of(".");
                if (dotPos != std::string::npos) {
                    textureName = textureName.substr(0, dotPos);
                }

                //std::cout << "Final texture name: " << std::string(textureName + ".bmp") << std::endl;

                //ModelTexture modelTexture = 

            }
        }
    }

    //// Allocate an array for all the indices (which is 3x the number of triangles)
    //drawInfo.numberOfTriangles = mesh->mNumFaces;

    //drawInfo.numberOfIndices = drawInfo.numberOfTriangles * 3;
    //drawInfo.pIndices = new unsigned int[drawInfo.numberOfIndices];
    //
    //aiFace* faces = mesh->mFaces;
    //unsigned int elementIndex = 0;
    //for (unsigned int triIndex = 0; triIndex != drawInfo.numberOfTriangles; triIndex++)
    //{
    //    //aiFace* faces = mesh->mFaces[triIndex];

    //    // 3 1582 1581 2063 
    //    drawInfo.pIndices[elementIndex + 0] = faces->mIndices[0];
    //    drawInfo.pIndices[elementIndex + 1] = faces->mIndices[1];
    //    drawInfo.pIndices[elementIndex + 2] = faces->mIndices[2];

    //    faces++;
    //    elementIndex += 3;      // Next "triangle"
    //}

    // Ask OpenGL for a new buffer ID...
    glGenVertexArrays(1, &(drawInfo.VAO_ID));
    // "Bind" this buffer:
    // - aka "make this the 'current' VAO buffer
    glBindVertexArray(drawInfo.VAO_ID);

    // Now ANY state that is related to vertex or index buffer
    //	and vertex attribute layout, is stored in the 'state' 
    //	of the VAO... 

    // NOTE: OpenGL error checks have been omitted for brevity
    glGenBuffers(1, &(drawInfo.VertexBufferID));

    //	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, drawInfo.VertexBufferID);
    // sVert vertices[3]

    // This is updated for bIsDynamicBuffer so:
    // * if true, then it's GL_DYNAMIC_DRAW
    // * if false, then it's GL_STATIC_DRAW
    // 
    // Honestly, it's not that Big Of A Deal in that you can still update 
    //  a buffer if it's set to STATIC, but in theory this will take longer.
    // Does it really take longer? Who knows?
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(sVertex) * drawInfo.numberOfVertices,	// ::g_NumberOfVertsToDraw,	// sizeof(vertices), 
        (GLvoid*)drawInfo.pVertices,							// pVertices,			//vertices, 
        (bIsDynamicBuffer ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));


    // Copy the index buffer into the video card, too
    // Create an index buffer.
    glGenBuffers(1, &(drawInfo.IndexBufferID));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.IndexBufferID);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,			// Type: Index element array
        sizeof(unsigned int) * drawInfo.numberOfIndices,
        (GLvoid*)drawInfo.pIndices,
        GL_STATIC_DRAW);

    // Set the vertex attributes.

    GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPos");	// program
    GLint vcol_location = glGetAttribLocation(shaderProgramID, "vCol");	// program;
    GLint vNormal_location = glGetAttribLocation(shaderProgramID, "vNormal");	// program;
    // AND the texture coordinate
    GLint vTextureCoords_location = glGetAttribLocation(shaderProgramID, "vTextureCoords");    // in vec2 vTextureCoords;		// NOTE this is a vec2 not vec4;		// NOTE this is a vec2 not vec4

    GLint vBones_Id = glGetAttribLocation(shaderProgramID, "vBoneIds");    // in vec2 vTextureCoords;		// NOTE this is a vec2 not vec4;		// NOTE this is a vec2 not vec4

    GLint vBones_Weights = glGetAttribLocation(shaderProgramID, "vBoneWeights");

    // Set the vertex attributes for this shader
    glEnableVertexAttribArray(vpos_location);	    // vPos
    glVertexAttribPointer(vpos_location, 4,		// vPos
        GL_FLOAT, GL_FALSE,
        sizeof(sVertex),
        (void*)offsetof(sVertex, x));

    glEnableVertexAttribArray(vcol_location);	    // vCol
    glVertexAttribPointer(vcol_location, 4,		// vCol
        GL_FLOAT, GL_FALSE,
        sizeof(sVertex),
        (void*)offsetof(sVertex, r));

    glEnableVertexAttribArray(vNormal_location);	// vNormal
    glVertexAttribPointer(vNormal_location, 4,		// vNormal
        GL_FLOAT, GL_FALSE,
        sizeof(sVertex),
        (void*)offsetof(sVertex, nx));

    glEnableVertexAttribArray(vTextureCoords_location);	    // vTextureCoords;
    glVertexAttribPointer(vTextureCoords_location, 2,		// in vec2 vTextureCoords;
        GL_FLOAT, GL_FALSE,
        sizeof(sVertex),
        (void*)offsetof(sVertex, u));

    glEnableVertexAttribArray(vBones_Id);	    // vTextureCoords;
    glVertexAttribPointer(vBones_Id, 4,		// in vec2 vTextureCoords;
        GL_FLOAT, GL_FALSE,
        sizeof(sVertex),
        (void*)offsetof(sVertex, bones1));

    glEnableVertexAttribArray(vBones_Weights);	    // vTextureCoords;
    glVertexAttribPointer(vBones_Weights, 4,		// in vec2 vTextureCoords;
        GL_FLOAT, GL_FALSE,
        sizeof(sVertex),
        (void*)offsetof(sVertex, boneWeight1));

    // Now that all the parts are set up, set the VAO to zero
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(vpos_location);
    glDisableVertexAttribArray(vcol_location);
    glDisableVertexAttribArray(vNormal_location);
    glDisableVertexAttribArray(vTextureCoords_location);        // <-- New
    glDisableVertexAttribArray(vBones_Id);
    glDisableVertexAttribArray(vBones_Weights);

    // Store the draw information into the map
    this->m_map_ModelName_to_VAOID[drawInfo.meshName] = drawInfo;

    return true;
}

bool cVAOManager::LoadAnimationWithAssimp(std::string fileName, sModelDrawInfo& drawInfo)
{
    std::string fileAndPath = this->m_basePathWithoutSlash + "/" + fileName;

    const aiScene* scene = m_AssimpImporter.ReadFile(fileAndPath.c_str(), (aiProcess_Triangulate | aiProcess_GenNormals));

    std::cout << m_AssimpImporter.GetErrorString();

    if (scene == nullptr)
    {
        std::cout << "Failed to load file: " << fileName << std::endl;
        return false;
    }

    std::vector<int> verticesCount;
    verticesCount.resize(5);

    if (scene->mNumMeshes > 1)
    {
        std::cout << "Too many meshes in one file" << std::endl;
        return false;
    }

    aiMesh* mesh = scene->mMeshes[0];

    if (scene->mNumAnimations > 0)
    {
        characterAnimation = new Bones::CharacterAnimation();
        aiAnimation* animation = scene->mAnimations[0];

        characterAnimation->Name = fileName;
        characterAnimation->Duration = animation->mDuration;
        characterAnimation->TicksPerSecond = animation->mTicksPerSecond;

        for (unsigned int i = 0; i < animation->mNumChannels; ++i)
        {
            aiNodeAnim* animationNode = animation->mChannels[i];
            Bones::NodeAnim* boneAnimation = new Bones::NodeAnim(animationNode->mNodeName.C_Str());
            for (unsigned int p = 0; p < animationNode->mNumPositionKeys; ++p)
            {
                aiVectorKey& positionKey = animationNode->mPositionKeys[p];
                boneAnimation->m_PositionKeyFrames.emplace_back(
                    cAnimation::PositionKeyFrame(glm::vec3(positionKey.mValue.x, positionKey.mValue.y, positionKey.mValue.z), positionKey.mTime));
            }
            for (unsigned int s = 0; s < animationNode->mNumScalingKeys; ++s)
            {
                aiVectorKey& scaleKey = animationNode->mScalingKeys[s];
                boneAnimation->m_ScaleKeyFrames.emplace_back(
                    cAnimation::ScaleKeyFrame(glm::vec3(scaleKey.mValue.x, scaleKey.mValue.y, scaleKey.mValue.z), scaleKey.mTime));
            }
            for (unsigned int q = 0; q < animationNode->mNumRotationKeys; ++q)
            {
                aiQuatKey& rotationKey = animationNode->mRotationKeys[q];
                boneAnimation->m_RotationKeyFrames.emplace_back(
                    cAnimation::RotationKeyFrame(glm::quat(rotationKey.mValue.w, rotationKey.mValue.x, rotationKey.mValue.y, rotationKey.mValue.z), rotationKey.mTime));
            }
            characterAnimation->Channels.emplace_back(boneAnimation);
        }

        characterAnimation->RootNode = GenerateBoneHierarchy(scene->mRootNode, drawInfo);
        g_animationSystem->characterAnimations.push_back(characterAnimation);
    }
    else
    {
        return false;
    }

    return true;
}
