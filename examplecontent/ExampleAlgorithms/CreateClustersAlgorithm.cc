/**
 *  @file   WorkshopContent/examplecontent/ExampleAlgorithms/CreateClustersAlgorithm.cc
 * 
 *  @brief  Implementation of the create clusters algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "examplecontent/ExampleAlgorithms/CreateClustersAlgorithm.h"

#include "examplecontent/ExampleHelpers/ExampleHelper.h"

using namespace pandora;

namespace example_content
{

CreateClustersAlgorithm::CreateClustersAlgorithm() :
    m_maxClusterHitDistance(20.f)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode CreateClustersAlgorithm::Run()
{
    // Create clusters using calo hits in the current list as the building blocks.
    const CaloHitList *pCaloHitList(nullptr);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pCaloHitList));

    // Algorithms must either create a temporary list for newly created clusters, or ask to run a daughter clustering algorithm
    // (temporary list, owned by parent algorithm is then created automatically for you). Any Clusters remaining in a temporary
    // list at the end of the algorithm will be deleted, so all desired clusters must be saved before the algorithm ends.
    const ClusterList *pTemporaryList(nullptr);
    std::string temporaryListName;
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::CreateTemporaryListAndSetCurrent(*this, pTemporaryList, temporaryListName));

    // Here we use the first hit in the unordered calo hit list to seed a new clusters. Subsequent hits are then either added
    // to the closest seed cluster (provided it is within a specified maximum distance), or used to create an additional seed cluster.
    for (const CaloHit *const pCaloHit : *pCaloHitList)
    {
        // Once a calo hit has been added to a cluster, it is flagged as unavailable.
        if (!PandoraContentApi::IsAvailable(*this, pCaloHit))
            continue;

        try
        {
            const Cluster *const pCluster(ExampleHelper::FindClosestCluster(pCaloHit, pTemporaryList, m_maxClusterHitDistance));
            PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::AddToCluster(*this, pCluster, pCaloHit));
        }
        catch (StatusCodeException &)
        {
            const Cluster *pCluster(nullptr);
            PandoraContentApi::Cluster::Parameters parameters;
            parameters.m_caloHitList.insert(pCaloHit);
            PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::Cluster::Create(*this, parameters, pCluster));
        }
    }

    // Choose to save the temporary clusters under a specified name and to set the cluster list to be the current list.
    if (!pTemporaryList->empty())
    {
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::SaveList<Cluster>(*this, m_outputListName));
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::ReplaceCurrentList<Cluster>(*this, m_outputListName));
    }

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode CreateClustersAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "MaxClusterHitDistance", m_maxClusterHitDistance));

    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "OutputListName", m_outputListName));

    return STATUS_CODE_SUCCESS;
}

} // namespace example_content
