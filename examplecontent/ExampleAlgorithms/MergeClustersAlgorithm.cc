/**
 *  @file   WorkshopContent/examplecontent/ExampleAlgorithms/MergeClustersAlgorithm.cc
 * 
 *  @brief  Implementation of the merge clusters algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "examplecontent/ExampleAlgorithms/MergeClustersAlgorithm.h"

#include "examplecontent/ExampleHelpers/ExampleHelper.h"

#include <algorithm>

using namespace pandora;

namespace example_content
{

MergeClustersAlgorithm::MergeClustersAlgorithm() :
    m_nClusterMergesToMake(1),
    m_maxClusterDistance(std::numeric_limits<float>::max())
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode MergeClustersAlgorithm::Run()
{
    // Make a number of cluster merge operations, with each merge enlarging a parent cluster and deleting a daughter cluster.
    const ClusterList *pClusterList(nullptr);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pClusterList));

    if (pClusterList->size() < 2)
        return STATUS_CODE_SUCCESS;

    unsigned int nClusterMerges(0);

    // Need to be very careful with cluster list iterators here, as we are deleting elements from the std::unordered_set owned by the manager.
    // If user chooses to iterate over that same list, must adhere to rule that iterators pointing at the deleted element will be invalidated.

    // Here, iterate over a local copy of the cluster list
    const ClusterList localClusterList(*pClusterList);

    for (const Cluster *const pParentCluster : localClusterList)
    {
        try
        {
            // Check to see whether parent cluster (address stored in local vector) still exists in manager-owned list, and hasn't been
            // removed by the cluster merging operations in this algorithm. Many alternative methods to check this, of course.
            if (pClusterList->end() == std::find(pClusterList->begin(), pClusterList->end(), pParentCluster))
                continue;

            const Cluster *const pBestDaughterCluster(ExampleHelper::FindClosestCluster(pParentCluster, pClusterList, m_maxClusterDistance));

            if (++nClusterMerges > m_nClusterMergesToMake)
                break;

            // The API implementation will enforce the availability of the daughter cluster and ensure that the parent and daughter are not one and the same
            PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::MergeAndDeleteClusters(*this, pParentCluster, pBestDaughterCluster));

            // pBestDaughterCluster is now a dangling pointer, which exists only in the local cluster list - do not deference!
        }
        catch (StatusCodeException &)
        {
        }
    }

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode MergeClustersAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "NClusterMergesToMake", m_nClusterMergesToMake));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "MaxClusterDistance", m_maxClusterDistance));

    return STATUS_CODE_SUCCESS;
}

} // namespace example_content
