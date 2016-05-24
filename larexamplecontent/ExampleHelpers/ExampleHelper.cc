/**
 *  @file   LArExampleContent/larexamplecontent/ExampleHelpers/ExampleHelper.cc
 * 
 *  @brief  Implementation of the example helper class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "ExampleHelpers/ExampleHelper.h"

using namespace pandora;

namespace example_content
{

const Cluster *ExampleHelper::FindClosestCluster(const CaloHit *const pCaloHit, const ClusterList *const pClusterList, const float maxDistance)
{
    const Cluster *pClosestCluster(NULL);
    float closestDistanceSquared(maxDistance * maxDistance);
    const CartesianVector positionVector(pCaloHit->GetPositionVector());

    for (ClusterList::const_iterator iter = pClusterList->begin(), iterEnd = pClusterList->end(); iter != iterEnd; ++iter)
    {
        const Cluster *const pCandidateCluster(*iter);

        const CartesianVector candidateCentroid(pCandidateCluster->GetCentroid(pCandidateCluster->GetInnerPseudoLayer()));
        const float distanceSquared((positionVector - candidateCentroid).GetMagnitudeSquared());

        if (distanceSquared < closestDistanceSquared)
        {
            closestDistanceSquared = distanceSquared;
            pClosestCluster = pCandidateCluster;
        }
    }

    if (NULL == pClosestCluster)
        throw StatusCodeException(STATUS_CODE_NOT_FOUND);

    return pClosestCluster;
}

//------------------------------------------------------------------------------------------------------------------------------------------

const Cluster *ExampleHelper::FindClosestCluster(const Cluster *const pCluster, const ClusterList *const pClusterList, const float maxDistance)
{
    const Cluster *pClosestCluster(NULL);
    float closestDistanceSquared(maxDistance * maxDistance);
    const CartesianVector centroid(pCluster->GetCentroid(pCluster->GetInnerPseudoLayer()));

    for (ClusterList::const_iterator clusIter = pClusterList->begin(), clusIterEnd = pClusterList->end(); clusIter != clusIterEnd; ++clusIter)
    {
        const Cluster *const pCandidateCluster(*clusIter);

        if (pCluster == pCandidateCluster)
            continue;

        const CartesianVector candidateCentroid(pCandidateCluster->GetCentroid(pCandidateCluster->GetInnerPseudoLayer()));
        const float distanceSquared((centroid - candidateCentroid).GetMagnitudeSquared());

        if (distanceSquared < closestDistanceSquared)
        {
            closestDistanceSquared = distanceSquared;
            pClosestCluster = pCandidateCluster;
        }
    }

    if (NULL == pClosestCluster)
        throw StatusCodeException(STATUS_CODE_NOT_FOUND);

    return pClosestCluster;
}

//------------------------------------------------------------------------------------------------------------------------------------------

const Vertex *ExampleHelper::FindClosestVertex(const Cluster *const pCluster, const VertexList *const pVertexListList, const float maxDistance)
{
    const Vertex *pClosestVertex(NULL);
    float closestDistanceSquared(maxDistance * maxDistance);
    const CartesianVector centroid(pCluster->GetCentroid(pCluster->GetInnerPseudoLayer()));

    for (VertexList::const_iterator iter = pVertexListList->begin(), iterEnd = pVertexListList->end(); iter != iterEnd; ++iter)
    {
        const Vertex *const pCandidateVertex(*iter);
        const float distanceSquared((centroid - pCandidateVertex->GetPosition()).GetMagnitudeSquared());

        if (distanceSquared < closestDistanceSquared)
        {
            closestDistanceSquared = distanceSquared;
            pClosestVertex = pCandidateVertex;
        }
    }

    if (NULL == pClosestVertex)
        throw StatusCodeException(STATUS_CODE_NOT_FOUND);

    return pClosestVertex;
}

} // namespace example_content
