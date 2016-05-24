/**
 *  @file   LArExampleContent/larexamplecontent/ExampleAlgorithms/DeleteClustersAlgorithm.cc
 * 
 *  @brief  Implementation of the delete clusters algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "ExampleAlgorithms/DeleteClustersAlgorithm.h"

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
    const ClusterList *pClusterList(NULL);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pClusterList));

    unsigned int nClustersDeleted(0);

    // Need to be very careful with cluster list iterators here, as we are deleting elements from the std::set. With sets, the rule
    // is that only iterators pointing at the deleted element will be invalidated, so here we increment before deletion.
    for (ClusterList::const_iterator iter = pClusterList->begin(); iter != pClusterList->end(); /*no increment*/ )
    {
        const Cluster *const pClusterToDelete(*iter);
        ++iter;

        if (++nClustersDeleted > m_nClustersToDelete)
            break;

        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::Delete(*this, pClusterToDelete));

        // pClusterToDelete is now a dangling pointer - do not deference!
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
