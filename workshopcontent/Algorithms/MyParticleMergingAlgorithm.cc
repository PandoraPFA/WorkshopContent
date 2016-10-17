/**
 *  @file   WorkshopContent/workshopcontent/Algorithms/MyParticleMergingAlgorithm.cc
 * 
 *  @brief  Implementation of the myparticlemerging algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "larpandoracontent/LArHelpers/LArClusterHelper.h"
#include "larpandoracontent/LArHelpers/LArPfoHelper.h"

#include "workshopcontent/Algorithms/MyParticleMergingAlgorithm.h"

using namespace pandora;
using namespace lar_content;

namespace workshop_content
{

StatusCode MyParticleMergingAlgorithm::Run()
{
    PfoVector sortedPfos;
    this->GetSortedPfos(sortedPfos);

    for (const ParticleFlowObject *const pPfo1 : sortedPfos)
    {
        for (const ParticleFlowObject *const pPfo2 : sortedPfos)
        {
            // ATTN Check whether vector contents (size must remain fixed) is changing underneath; consider only top-level pfos
            if (!pPfo1 || !pPfo2 || (pPfo1 == pPfo2) || !pPfo1->GetParentPfoList().empty() || !pPfo2->GetParentPfoList().empty())
                continue;
            
            // TODO Add key pattern-recognition decision #1 here. Write a function to return this boolean.
            const bool shouldMergePfos(true);
            
            if (shouldMergePfos)
            {
                // TODO Add key pattern-recognition decision #2 here. Write a function to decide which pfo to enlarge.
                const ParticleFlowObject *const pPfoToEnlarge(pPfo1);
                const ParticleFlowObject *const pPfoToDelete(pPfo2);
                
                const PfoList enlargePfoList(1, pPfoToEnlarge), deletePfoList(1, pPfoToDelete);
                PandoraMonitoringApi::SetEveDisplayParameters(this->GetPandora(), false, DETECTOR_VIEW_XZ, -1.f, -1.f, 1.f);
                PandoraMonitoringApi::VisualizeParticleFlowObjects(this->GetPandora(), &enlargePfoList, "PfoToEnlarge", BLUE);
                PandoraMonitoringApi::VisualizeParticleFlowObjects(this->GetPandora(), &deletePfoList, "PfoToDelete", GREEN);
                PandoraMonitoringApi::ViewEvent(this->GetPandora());
                
                // ATTN Remove what will shortly become a dangling pointer from the container
                std::replace(sortedPfos.begin(), sortedPfos.end(), pPfoToDelete, static_cast<const ParticleFlowObject *>(nullptr));
                this->MergeAndDeletePfos(pPfoToEnlarge, pPfoToDelete);
                
                const PfoList mergedPfoList(1, pPfoToEnlarge);
                PandoraMonitoringApi::VisualizeParticleFlowObjects(this->GetPandora(), &mergedPfoList, "MergedPfo", RED);
                PandoraMonitoringApi::ViewEvent(this->GetPandora());
            }
        }
    }

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void MyParticleMergingAlgorithm::GetSortedPfos(PfoVector &sortedPfos) const
{
    for (const std::string &listName : m_inputPfoListNames)
    {
        const PfoList *pPfoList(nullptr);
        
        if (STATUS_CODE_SUCCESS == PandoraContentApi::GetList(*this, listName, pPfoList))
            sortedPfos.insert(sortedPfos.end(), pPfoList->begin(), pPfoList->end());
    }

    std::sort(sortedPfos.begin(), sortedPfos.end(), LArPfoHelper::SortByNHits);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void MyParticleMergingAlgorithm::MergeAndDeletePfos(const ParticleFlowObject *const pPfoToEnlarge, const ParticleFlowObject *const pPfoToDelete) const
{
    const PfoList daughterPfos(pPfoToDelete->GetDaughterPfoList());
    const ClusterVector daughterClusters(pPfoToDelete->GetClusterList().begin(), pPfoToDelete->GetClusterList().end());
    const VertexVector daughterVertices(pPfoToDelete->GetVertexList().begin(), pPfoToDelete->GetVertexList().end());
    
    PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::Delete(*this, pPfoToDelete, this->GetListName(pPfoToDelete)));
    
    for (const ParticleFlowObject *const pDaughterPfo : daughterPfos)
    {
        PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::SetPfoParentDaughterRelationship(*this, pPfoToEnlarge, pDaughterPfo));
    }
    
    for (const  Vertex *const pDaughterVertex : daughterVertices)
    {
        PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::Delete(*this, pDaughterVertex, this->GetListName(pDaughterVertex)));
    }
    
    for (const Cluster *const pDaughterCluster : daughterClusters)
    {
        const HitType daughterHitType(LArClusterHelper::GetClusterHitType(pDaughterCluster));
        const Cluster *pParentCluster(this->GetParentCluster(pPfoToEnlarge->GetClusterList(), daughterHitType));
        
        if (pParentCluster)
        {
            PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::MergeAndDeleteClusters(*this, pParentCluster, pDaughterCluster,
                this->GetListName(pParentCluster), this->GetListName(pDaughterCluster)));
        }
        else
        {
            PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::AddToPfo(*this, pPfoToEnlarge, pDaughterCluster));
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

const Cluster *MyParticleMergingAlgorithm::GetParentCluster(const ClusterList &clusterList, const HitType hitType) const
{
    unsigned int mostHits(0);
    const Cluster *pBestParentCluster(nullptr);
    
    for (const Cluster *const pParentCluster : clusterList)
    {
        if (hitType != LArClusterHelper::GetClusterHitType(pParentCluster))
            continue;
        
        const unsigned int nParentHits(pParentCluster->GetNCaloHits());
        
        if (nParentHits > mostHits)
        {
            mostHits = nParentHits;
            pBestParentCluster = pParentCluster;
        }
    }
    
    return pBestParentCluster;
}

//------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
const std::string MyParticleMergingAlgorithm::GetListName(const T *const pT) const
{
    std::string currentListName;
    const MANAGED_CONTAINER<const T*> *pCurrentList(nullptr);
    (void) PandoraContentApi::GetCurrentList(*this, pCurrentList, currentListName);

    if (pCurrentList && (pCurrentList->end() != std::find(pCurrentList->begin(), pCurrentList->end(), pT)))
        return currentListName;
    
    for (const std::string &listName : m_daughterListNames)
    {
        const MANAGED_CONTAINER<const T*> *pList(nullptr);
        (void) PandoraContentApi::GetList(*this, listName, pList);
        
        if (pList && (pList->end() != std::find(pList->begin(), pList->end(), pT)))
            return listName;
    }
    
    throw StatusCodeException(STATUS_CODE_NOT_FOUND);
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode MyParticleMergingAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadVectorOfValues(xmlHandle,
        "InputPfoListNames", m_inputPfoListNames));

    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadVectorOfValues(xmlHandle,
        "DaughterListNames", m_daughterListNames));
    
    m_daughterListNames.insert(m_daughterListNames.end(), m_inputPfoListNames.begin(), m_inputPfoListNames.end());

    return STATUS_CODE_SUCCESS;
}

} // namespace workshop_content
