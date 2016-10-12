/**
 *  @file   WorkshopContent/examplecontent/ExampleAlgorithms/ReconfigureClustersAlgorithm.cc
 * 
 *  @brief  Implementation of the reconfigure clusters algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "examplecontent/ExampleAlgorithms/ReconfigureClustersAlgorithm.h"

using namespace pandora;

namespace example_content
{

ReconfigureClustersAlgorithm::ReconfigureClustersAlgorithm() :
    m_nClustersToReconfigure(1)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode ReconfigureClustersAlgorithm::Run()
{
    // Make a number of cluster fragmentation operations, each taking an input cluster from the current list and using any number
    // of clustering algorithms, defined in the PandoraSettings xml file, to reconfigure the constituent calo hits.
    const ClusterList *pClusterList(nullptr);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pClusterList));

    unsigned int nClustersReconfigured(0);

    // Need to be very careful with cluster list iterators here, as we are deleting elements from the std::unordered_set owned by the manager.
    // If user chooses to iterate over that same list, must adhere to rule that iterators pointing at the deleted element will be invalidated.

    // Here, iterate over a local copy of the cluster list
    const ClusterList localClusterList(*pClusterList);

    for (const Cluster *const pOriginalCluster : localClusterList)
    {
        if (++nClustersReconfigured > m_nClustersToReconfigure)
            break;

        // The reclustering mechanism allows the original and many attempts at new candidate clusters to exist concurrently, with the
        // calo hit availability monitoring functioning separately for each of the (potentially many) different hit configuration options.
        const ClusterList originalClusterList(1, pOriginalCluster);
        std::string originalClustersListName;
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::InitializeReclustering(*this, TrackList(), originalClusterList, originalClustersListName));

        std::string bestReclusterListName(originalClustersListName);

        // At this point the original cluster has been moved to the cluster list with name originalClustersListName.
        for (const std::string &algorithmName : m_clusteringAlgorithms)
        {
            // For each clustering algorithm specified in the PandoraSettings xml file a new temporary cluster list with
            // a unique name (received by reference, the reclusterListName) is created to receive any new clusters.
            std::string reclusterListName;
            const ClusterList *pReclusterList = nullptr;
            PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::RunClusteringAlgorithm(*this, algorithmName, pReclusterList, reclusterListName));

            if (pReclusterList->empty())
                continue;

            // Typically investigate the new candidate cluster configuration to obtain some figure of merit indicating how good
            // the configuration is. If it is the best configuration so far, save its list name to use when ending the reclustering.
            bestReclusterListName = reclusterListName;
        }

        // A simple decision must be made as to whether to keep the original cluster or one of the new cluster configurations
        // and all the memory is tidied accordingly. Here we automatically choose to keep the last of the new configurations.
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::EndReclustering(*this, bestReclusterListName));

        // pOriginalCluster is now a dangling pointer, which exists only in the local cluster list - do not deference!
    }

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode ReconfigureClustersAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "NClustersToReconfigure", m_nClustersToReconfigure));

    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ProcessAlgorithmList(*this, xmlHandle,
        "ReclusteringAlgorithms", m_clusteringAlgorithms));

    return STATUS_CODE_SUCCESS;
}

} // namespace example_content
