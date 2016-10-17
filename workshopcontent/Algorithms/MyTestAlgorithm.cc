/**
 *  @file   WorkshopContent/workshopcontent/Algorithms/MyTestAlgorithm.cc
 * 
 *  @brief  Implementation of the mytest algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "larpandoracontent/LArHelpers/LArClusterHelper.h"

#include "workshopcontent/Algorithms/MyTestAlgorithm.h"

using namespace pandora;
using namespace lar_content;

namespace workshop_content
{

MyTestAlgorithm::MyTestAlgorithm() :
    m_outputClusterListName(),
    m_nHitsPerCluster(10)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode MyTestAlgorithm::Run()
{
    const CaloHitList *pCaloHitList(nullptr);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pCaloHitList));

    const ClusterList *pTemporaryList(nullptr);
    std::string temporaryListName;
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::CreateTemporaryListAndSetCurrent(*this, pTemporaryList, temporaryListName));
    
    CaloHitVector sortedCaloHits(pCaloHitList->begin(), pCaloHitList->end());
    std::sort(sortedCaloHits.begin(), sortedCaloHits.end(), LArClusterHelper::SortHitsByPosition);

    const Cluster *pCluster(nullptr);

    for (const CaloHit *const pCaloHit : sortedCaloHits)
    {
        if (!PandoraContentApi::IsAvailable(*this, pCaloHit))
            continue;

        if (!pCluster || (pCluster->GetNCaloHits() >= m_nHitsPerCluster))
        {
            PandoraContentApi::Cluster::Parameters parameters;
            parameters.m_caloHitList.push_back(pCaloHit);
            PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::Cluster::Create(*this, parameters, pCluster));
        }
        else
        {
            PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::AddToCluster(*this, pCluster, pCaloHit));
        }
    }

    if (!pTemporaryList->empty())
    {
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::SaveList<Cluster>(*this, m_outputClusterListName));
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::ReplaceCurrentList<Cluster>(*this, m_outputClusterListName));
    }

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode MyTestAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "OutputClusterListName", m_outputClusterListName));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "NHitsPerCluster", m_nHitsPerCluster));

    return STATUS_CODE_SUCCESS;
}

} // namespace workshop_content
