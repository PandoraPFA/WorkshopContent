/**
 *  @file   LArExampleContent/larexamplecontent/ExampleAlgorithms/CreateClustersDaughterAlgorithm.cc
 * 
 *  @brief  Implementation of the create clusters daughter algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "larexamplecontent/ExampleAlgorithms/CreateClustersDaughterAlgorithm.h"

#include "larexamplecontent/ExampleHelpers/ExampleHelper.h"

using namespace pandora;

namespace lar_example_content
{

CreateClustersDaughterAlgorithm::CreateClustersDaughterAlgorithm() :
    m_nClustersToMake(1)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode CreateClustersDaughterAlgorithm::Run()
{
    // Create clusters using calo hits in the current list as the building blocks.
    const CaloHitList *pCaloHitList(nullptr);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pCaloHitList));

    // This algorithm demonstrates cluster creation in the context of running a daughter algorithm, with newly clusters being placed
    // in the temporary cluster list owned by the calling parent algorithm. It is the responsibility of the parent algorithm to save
    // the clusters, which will otherwise be deleted when the parent algorithm ends.
    const ClusterList *pCurrentClusterList(nullptr);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pCurrentClusterList));

    // This will be a temporary cluster list and will be empty immediately after calling the daughter clustering algorithm.
    if (!pCurrentClusterList->empty())
        return STATUS_CODE_NOT_ALLOWED;

    // Here we use the first m_nClustersToMake hits in the unordered calo hit list to seed new clusters. Subsequent hits are then
    // added to the closest seed cluster, based on a simple (rather than efficient) closest-hits calculation in the example helper.
    for (const CaloHit *const pCaloHit : *pCaloHitList)
    {
        // Once a calo hit has been added to a cluster, it is flagged as unavailable.
        if (!PandoraContentApi::IsAvailable(*this, pCaloHit))
            continue;

        if (pCurrentClusterList->size() >= m_nClustersToMake)
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

    // Subsequent list manipulation (saving clusters, etc.) is here provided entirely by the parent algorithm. A daughter clustering
    // algorithm concentrates solely on the cluster creation, not list manipulation.

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode CreateClustersDaughterAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "NClustersToMake", m_nClustersToMake));

    return STATUS_CODE_SUCCESS;
}

} // namespace lar_example_content
