/**
 *  @file   WorkshopContent/workshopcontent/Algorithms/MyClusterMergingAlgorithm.cc
 * 
 *  @brief  Implementation of the myclustermerging algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "larpandoracontent/LArHelpers/LArClusterHelper.h"
#include "larpandoracontent/LArHelpers/LArPointingClusterHelper.h"

#include "larpandoracontent/LArObjects/LArTwoDSlidingFitResult.h"

#include "workshopcontent/Algorithms/MyClusterMergingAlgorithm.h"

using namespace pandora;
using namespace lar_content;

namespace workshop_content
{

MyClusterMergingAlgorithm::MyClusterMergingAlgorithm() :
    m_inputClusterListName(),
    m_minClusterCaloHits(5),
    m_maxClusterSeparation(2.f),
    m_maxImpactL(5.f),
    m_maxImpactT(2.f)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode MyClusterMergingAlgorithm::Run()
{
    const ClusterList *pClusterList(nullptr);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetList(*this, m_inputClusterListName, pClusterList));
    
    if (pClusterList->size() < 2)
        return STATUS_CODE_SUCCESS;

    ClusterVector sortedLongClusters;
    this->GetSortedLongClusters(pClusterList, sortedLongClusters);

    ClusterSet defunctClusters;

    for (const Cluster *const pParentCluster : sortedLongClusters)
    {
        if (defunctClusters.count(pParentCluster))
            continue;

        for (const Cluster *const pDaughterCluster : sortedLongClusters)
        {
            if ((pParentCluster == pDaughterCluster) || defunctClusters.count(pDaughterCluster))
                continue;

            if (!this->AreClustersAssociated(pParentCluster, pDaughterCluster))
                continue;

            PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::MergeAndDeleteClusters(*this, pParentCluster, pDaughterCluster));
            (void) defunctClusters.insert(pDaughterCluster);
        }
    }

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void MyClusterMergingAlgorithm::GetSortedLongClusters(const ClusterList *const pClusterList, ClusterVector &sortedLongClusters) const
{
    for (const Cluster *const pCluster : *pClusterList)
    {
        if (pCluster->GetNCaloHits() > m_minClusterCaloHits)
            sortedLongClusters.push_back(pCluster);
    }

    std::sort(sortedLongClusters.begin(), sortedLongClusters.end(), LArClusterHelper::SortByNHits);
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool MyClusterMergingAlgorithm::AreClustersAssociated(const Cluster *const pParentCluster, const Cluster *const pDaughterCluster) const
{
    try
    {
        // Useful constructs for pointing information
        const LArPointingCluster parentPointingCluster(pParentCluster);
        const LArPointingCluster daughterPointingCluster(pDaughterCluster);

        LArPointingCluster::Vertex closestVertexParent, closestVertexDaughter;
        LArPointingClusterHelper::GetClosestVertices(parentPointingCluster, daughterPointingCluster, closestVertexParent, closestVertexDaughter);

        // Impact parameters
        float parentDaughterImpactL(std::numeric_limits<float>::max()), parentDaughterImpactT(std::numeric_limits<float>::max());
        LArPointingClusterHelper::GetImpactParameters(closestVertexParent, closestVertexDaughter, parentDaughterImpactL, parentDaughterImpactT);

        float daughterParentImpactL(std::numeric_limits<float>::max()), daughterParentImpactT(std::numeric_limits<float>::max());
        LArPointingClusterHelper::GetImpactParameters(closestVertexDaughter, closestVertexParent, daughterParentImpactL, daughterParentImpactT);

        // Make decision
        if (((parentDaughterImpactL < m_maxImpactL) && (parentDaughterImpactT < m_maxImpactT)) ||
            ((daughterParentImpactL < m_maxImpactL) && (daughterParentImpactT < m_maxImpactT)))
        {
            return true;
        }
    }
    catch (const StatusCodeException &statusCodeException)
    {
        std::cout << "MyClusterMergingAlgorithm::AreClustersAssociated " << statusCodeException.ToString() << std::endl;
    }

    return false;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode MyClusterMergingAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "InputClusterListName", m_inputClusterListName));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "MinClusterCaloHits", m_minClusterCaloHits));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "MaxClusterSeparation", m_maxClusterSeparation));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "MaxImpactL", m_maxImpactL));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "MaxImpactT", m_maxImpactT));

    return STATUS_CODE_SUCCESS;
}

} // namespace workshop_content
