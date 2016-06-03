/**
 *  @file   WorkshopContent/examplecontent/ExampleAlgorithms/FragmentClustersAlgorithm.cc
 * 
 *  @brief  Implementation of the fragment clusters algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "examplecontent/ExampleAlgorithms/FragmentClustersAlgorithm.h"

#include "examplecontent/ExampleHelpers/ExampleHelper.h"

using namespace pandora;

namespace example_content
{

FragmentClustersAlgorithm::FragmentClustersAlgorithm() :
    m_nClustersToFragment(1),
    m_nFragmentsPerCluster(2)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode FragmentClustersAlgorithm::Run()
{
    // Make a number of cluster fragmentation operations, each taking an input cluster from the current list and turning its
    // constituent hits into a configurable number of new fragment clusters.
    const ClusterList *pClusterList(nullptr);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pClusterList));

    unsigned int nClustersFragmented(0);

    // Need to be very careful with cluster list iterators here, as we are deleting elements from the std::unordered_set. With sets,
    // the rule is that only iterators pointing at the deleted element will be invalidated, so here we increment before deletion.
    for (ClusterList::const_iterator iter = pClusterList->begin(); iter != pClusterList->end(); /*no increment*/ )
    {
        const Cluster *const pOriginalCluster(*iter);
        ++iter;

        if (++nClustersFragmented > m_nClustersToFragment)
            break;

        // The fragmentation mechanism allows the original and new candidate clusters to exist concurrently, with the calo hit
        // availability monitoring functioning separately for the two different hit configuration options.
        ClusterList originalClusterList;
        originalClusterList.insert(pOriginalCluster);
        std::string originalClustersListName, fragmentClustersListName;
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::InitializeFragmentation(*this, originalClusterList, originalClustersListName, fragmentClustersListName));

        // At this point the original cluster has been moved to the cluster list with name originalClustersListName and a new
        // temporary cluster list (with name fragmentClustersListName) exists to receive new clusters. The current calo hit list
        // contains only those hits in the original cluster and all are available until added to a new candidate cluster
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, this->PerformFragmentation());

        // A simple decision must be made as to whether to keep the original cluster or the new fragment clusters and all the memory
        // is tidied accordingly. Here we automatically choose to keep the new fragments, so the original cluster is deleted.
        const std::string clusterListToSaveName(fragmentClustersListName), clusterListToDeleteName(originalClustersListName);
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::EndFragmentation(*this, clusterListToSaveName, clusterListToDeleteName));
    }

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode FragmentClustersAlgorithm::PerformFragmentation() const
{
    // This will be a temporary cluster list and will be empty immediately after calling the initialize fragmentation API.
    const ClusterList *pCurrentClusterList(nullptr);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pCurrentClusterList));

    if (!pCurrentClusterList->empty())
        return STATUS_CODE_NOT_ALLOWED;

    // Immediately after calling the initialize fragmentation API, this will contain just those hits in the original cluster.
    const CaloHitList *pCurrentCaloHitList(nullptr);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pCurrentCaloHitList));

    // Here we use the first m_nClustersToMake hits in the unordered calo hit list to seed new clusters. Subsequent hits are then
    // added to the closest seed cluster, based on a simple (rather than efficient) closest-hits calculation in the example helper.
    for (const CaloHit *const pCaloHit : *pCurrentCaloHitList)
    {
        if (!PandoraContentApi::IsAvailable(*this, pCaloHit))
            continue;

        if (pCurrentClusterList->size() >= m_nFragmentsPerCluster)
        {
            const Cluster *const pCluster(ExampleHelper::FindClosestCluster(pCaloHit, pCurrentClusterList, std::numeric_limits<float>::max()));
            PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::AddToCluster(*this, pCluster, pCaloHit));
        }
        else
        {
            const Cluster *pCluster(nullptr);
            PandoraContentApi::Cluster::Parameters parameters;
            parameters.m_caloHitList.insert(pCaloHit);
            PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::Cluster::Create(*this, parameters, pCluster));
        }
    }

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode FragmentClustersAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "NClustersToFragment", m_nClustersToFragment));

    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "NFragmentsPerCluster", m_nFragmentsPerCluster));

    return STATUS_CODE_SUCCESS;
}

} // namespace example_content
