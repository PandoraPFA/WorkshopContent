/**
 *  @file   WorkshopContent/examplecontent/ExampleAlgorithms/DeleteClustersAlgorithm.cc
 * 
 *  @brief  Implementation of the delete clusters algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "examplecontent/ExampleAlgorithms/DeleteClustersAlgorithm.h"

using namespace pandora;

namespace example_content
{


DeleteClustersAlgorithm::DeleteClustersAlgorithm() :
    m_nClustersToDelete(1)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode DeleteClustersAlgorithm::Run()
{
    // Delete the requested number of clusters from the current list
    const ClusterList *pClusterList(nullptr);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pClusterList));

    unsigned int nClustersDeleted(0);

    // Need to be very careful with cluster list iterators here, as we are deleting elements from the std::unordered_set owned by the manager.
    // If user chooses to iterate over that same list, must adhere to rule that iterators pointing at the deleted element will be invalidated.

    // Here, iterate over a local copy of the cluster list
    const ClusterList localClusterList(*pClusterList);

    for (const Cluster *const pClusterToDelete : localClusterList)
    {
        if (++nClustersDeleted > m_nClustersToDelete)
            break;

        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::Delete(*this, pClusterToDelete));

        // pClusterToDelete is now a dangling pointer, which exists only in the local cluster list - do not deference!
    }

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode DeleteClustersAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "NClustersToDelete", m_nClustersToDelete));

    return STATUS_CODE_SUCCESS;
}

} // namespace example_content
