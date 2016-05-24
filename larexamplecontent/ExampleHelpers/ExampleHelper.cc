/**
 *  @file   LArExampleContent/larexamplecontent/ExampleHelpers/ExampleHelper.cc
 * 
 *  @brief  Implementation of the example helper class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "larexamplecontent/ExampleHelpers/ExampleHelper.h"

using namespace pandora;

namespace lar_example_content
{

const Cluster *ExampleHelper::FindClosestCluster(const CaloHit *const pCaloHit, const ClusterList *const pClusterList, const float maxDistance)
{
    const Cluster *pClosestCluster(nullptr);
    float closestDistanceSquared(maxDistance * maxDistance);
    const CartesianVector positionVector(pCaloHit->GetPositionVector());

    for (const Cluster *const pCandidateCluster : *pClusterList)
    {
        const CartesianVector candidateCentroid(pCandidateCluster->GetCentroid(pCandidateCluster->GetInnerPseudoLayer()));
        const float distanceSquared((positionVector - candidateCentroid).GetMagnitudeSquared());

        if (distanceSquared < closestDistanceSquared)
        {
            closestDistanceSquared = distanceSquared;
            pClosestCluster = pCandidateCluster;
        }
    }

    if (!pClosestCluster)
        throw StatusCodeException(STATUS_CODE_NOT_FOUND);

    return pClosestCluster;
}

//------------------------------------------------------------------------------------------------------------------------------------------

const Cluster *ExampleHelper::FindClosestCluster(const Cluster *const pCluster, const ClusterList *const pClusterList, const float maxDistance)
{
    const Cluster *pClosestCluster(nullptr);
    float closestDistanceSquared(maxDistance * maxDistance);
    const CartesianVector centroid(pCluster->GetCentroid(pCluster->GetInnerPseudoLayer()));

    for (const Cluster *const pCandidateCluster : *pClusterList)
    {
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

    if (!pClosestCluster)
        throw StatusCodeException(STATUS_CODE_NOT_FOUND);

    return pClosestCluster;
}

//------------------------------------------------------------------------------------------------------------------------------------------

const Vertex *ExampleHelper::FindClosestVertex(const Cluster *const pCluster, const VertexList *const pVertexListList, const float maxDistance)
{
    const Vertex *pClosestVertex(nullptr);
    float closestDistanceSquared(maxDistance * maxDistance);
    const CartesianVector centroid(pCluster->GetCentroid(pCluster->GetInnerPseudoLayer()));

    for (const Vertex *const pCandidateVertex : *pVertexListList)
    {
        const float distanceSquared((centroid - pCandidateVertex->GetPosition()).GetMagnitudeSquared());

        if (distanceSquared < closestDistanceSquared)
        {
            closestDistanceSquared = distanceSquared;
            pClosestVertex = pCandidateVertex;
        }
    }

    if (!pClosestVertex)
        throw StatusCodeException(STATUS_CODE_NOT_FOUND);

    return pClosestVertex;
}

} // namespace lar_example_content
