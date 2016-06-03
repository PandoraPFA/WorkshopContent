/**
 *  @file   WorkshopContent/examplecontent/ExampleHelpers/ExampleHelper.cc
 * 
 *  @brief  Implementation of the example helper class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "examplecontent/ExampleHelpers/ExampleHelper.h"

using namespace pandora;

namespace example_content
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

//------------------------------------------------------------------------------------------------------------------------------------------

bool ExampleHelper::ExampleCaloHitSort(const CaloHit *const pLhs, const CaloHit *const pRhs)
{
    const CartesianVector deltaPosition(pRhs->GetPositionVector() - pLhs->GetPositionVector());

    if (std::fabs(deltaPosition.GetX()) > std::numeric_limits<float>::epsilon())
        return (deltaPosition.GetX() > std::numeric_limits<float>::epsilon());

    if (std::fabs(deltaPosition.GetY()) > std::numeric_limits<float>::epsilon())
        return (deltaPosition.GetY() > std::numeric_limits<float>::epsilon());

    if (std::fabs(deltaPosition.GetZ()) > std::numeric_limits<float>::epsilon())
        return (deltaPosition.GetZ() > std::numeric_limits<float>::epsilon());

    // ATTN Use energy as final tie-breaker
    return (pLhs->GetHadronicEnergy() > pRhs->GetHadronicEnergy());
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool ExampleHelper::ExampleClusterSort(const Cluster *const pLhs, const Cluster *const pRhs)
{
    if ((0 == pLhs->GetNCaloHits()) || (0 == pRhs->GetNCaloHits()))
        return (pLhs->GetNCaloHits() > pRhs->GetNCaloHits());

    const CartesianVector deltaPosition(pRhs->GetCentroid(pRhs->GetInnerPseudoLayer()) - pLhs->GetCentroid(pLhs->GetInnerPseudoLayer()));

    if (std::fabs(deltaPosition.GetX()) > std::numeric_limits<float>::epsilon())
        return (deltaPosition.GetX() > std::numeric_limits<float>::epsilon());

    if (std::fabs(deltaPosition.GetY()) > std::numeric_limits<float>::epsilon())
        return (deltaPosition.GetY() > std::numeric_limits<float>::epsilon());

    if (std::fabs(deltaPosition.GetZ()) > std::numeric_limits<float>::epsilon())
        return (deltaPosition.GetZ() > std::numeric_limits<float>::epsilon());

    // ATTN Use energy as final tie-breaker
    return (pLhs->GetHadronicEnergy() > pRhs->GetHadronicEnergy());
}

} // namespace example_content
