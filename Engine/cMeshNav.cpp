#include "cMeshNav.h"
#include <fstream>
#include <string>
#include <glm/geometric.hpp>

cMeshNav::cMeshNav()
{

}

class cNodePointerMatch 
{
public:
    cNodePointerMatch(cMeshNav::sNode::sNodeCostPair& nodeToFind)
    {
        this->m_nodeToFind = nodeToFind;
    }
    bool operator() (cMeshNav::sNode::sNodeCostPair& testNode)
    {
        if (this->m_nodeToFind.pNode == testNode.pNode)
        {
            return true;
        }
        return false;
    }
private:
    cMeshNav::sNode::sNodeCostPair m_nodeToFind;
};

bool cMeshNav::CalculateNavMeshFromPly(std::string plyFileName)
{
    struct sTriangle
    {
        unsigned int triVertIndices[3];
        // store the nodes here, temporarily
        sNode* pNodes[3] = { NULL };
        //std::vector < sNode* > vec_pNodes;

    };
    std::vector< sTriangle> vecTriangles;

    unsigned int numberOfVertices = 0;
    unsigned int numberOfTriangles = 0;

    std::ifstream plyFile(plyFileName.c_str());
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

    plyFile >> numberOfVertices;

    while (plyFile >> temp)
    {
        if (temp == "face")
        {
            break;
        }
    };

    plyFile >> numberOfTriangles;

    //drawInfo.numberOfIndices = drawInfo.numberOfTriangles * 3;

    while (plyFile >> temp)
    {
        if (temp == "end_header")
        {
            break;
        }
    };


    for (unsigned int index = 0; index != numberOfVertices; index++)
    {
        sNode* pTempNode = new sNode();
        plyFile >> pTempNode->position.x;                //std::cin >> a.x;
        plyFile >> pTempNode->position.y;                //std::cin >> a.y;
        plyFile >> pTempNode->position.z;                //std::cin >> a.z;

        vec_pNodes.push_back(pTempNode);
    }

    for (unsigned int index = 0; index != numberOfTriangles; index++)
    {
        sTriangle tempTriangle;

        unsigned int discard;
        plyFile >> discard;            // 3

        unsigned int vertIndex = 0;

        plyFile >> vertIndex;
        tempTriangle.pNodes[0] = vec_pNodes[vertIndex];

        plyFile >> vertIndex;
        tempTriangle.pNodes[1] = vec_pNodes[vertIndex];

        plyFile >> vertIndex;
        tempTriangle.pNodes[2] = vec_pNodes[vertIndex];

        vecTriangles.push_back(tempTriangle);
    }

    for (sTriangle curTri : vecTriangles)
    {
        // Compare each node (vertex) with its neighbor
        sNode* pN1 = curTri.pNodes[0];
        sNode* pN2 = curTri.pNodes[1];
        sNode* pN3 = curTri.pNodes[2];

        pN1->vec_pAdjacentNodes.push_back(sNode::sNodeCostPair(pN2, CalcCost(pN1, pN2)));
        pN1->vec_pAdjacentNodes.push_back(sNode::sNodeCostPair(pN3, CalcCost(pN1, pN3)));

        pN2->vec_pAdjacentNodes.push_back(sNode::sNodeCostPair(pN1, CalcCost(pN2, pN1)));
        pN2->vec_pAdjacentNodes.push_back(sNode::sNodeCostPair(pN3, CalcCost(pN2, pN3)));

        pN3->vec_pAdjacentNodes.push_back(sNode::sNodeCostPair(pN1, CalcCost(pN3, pN1)));
        pN3->vec_pAdjacentNodes.push_back(sNode::sNodeCostPair(pN2, CalcCost(pN3, pN2)));
    }

    // remove duplicate edges

    for (sNode* pCurNode : this->vec_pNodes)
    {
        // Make a copy of the adjacent nodes (and costs)
        std::vector< sNode::sNodeCostPair > vecCopyOfNC = pCurNode->vec_pAdjacentNodes;

        // Clear this nodes adjacent node vector
        pCurNode->vec_pAdjacentNodes.clear();

        // Only copy back ones that aren't duplicates
        for (sNode::sNodeCostPair& curNodeCost : vecCopyOfNC)
        {
            // Is this one already in the vector?
            std::vector< sNode::sNodeCostPair >::iterator itFoundNode =
                std::find_if(pCurNode->vec_pAdjacentNodes.begin(), pCurNode->vec_pAdjacentNodes.end(), cNodePointerMatch(curNodeCost));

            if (itFoundNode == pCurNode->vec_pAdjacentNodes.end())
            {
                // Not there, so add it
                pCurNode->vec_pAdjacentNodes.push_back(curNodeCost);
            }
        }
    }

    return true;
}

float cMeshNav::CalcCost(sNode* pStart, sNode* pEnd)
{

    return pEnd->position.y - pStart->position.y;
}

cMeshNav::sNode* cMeshNav::pFindClosestNode(glm::vec2 XZ)
{
    sNode* pClosestNode = this->vec_pNodes[0];

    for (sNode* pTestNode : vec_pNodes)
    {
        // is this node closer than the closest node
        float distToCurrentClosest = glm::distance(XZ,
            glm::vec2(pClosestNode->position.x, pClosestNode->position.z));

        float distToThisNode = glm::distance(XZ,
            glm::vec2(pTestNode->position.x, pTestNode->position.z));

        if (distToThisNode < distToCurrentClosest)
        {
            // this is now the closest node
            pClosestNode = pTestNode;
        }
    }

    return pClosestNode;
}

bool cMeshNav::CalculateNavMeshFromFile(std::string fileName)
{

    return false;
}

