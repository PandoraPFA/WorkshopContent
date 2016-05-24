/**
 *  @file   LArExampleContent/larexamplecontent/ExampleAlgorithms/DisplayListsAlgorithm.cc
 * 
 *  @brief  Implementation of the display lists algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "ExampleAlgorithms/DisplayListsAlgorithm.h"

#include "ExampleObjects/ExampleCaloHit.h"

using namespace pandora;

namespace example_content
{

DisplayListsAlgorithm::DisplayListsAlgorithm() :
    m_displayCurrentCaloHits(false),
    m_displayCurrentTracks(false),
    m_displayCurrentMCParticles(false),
    m_displayCurrentClusters(false),
    m_displayCurrentVertices(false),
    m_displayCurrentPfos(false)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode DisplayListsAlgorithm::Run()
{
    // Useful event display functionality for visualizing the operations performed in other example algorithms.

    // The PANDORA_MONITORING_API preprocessor macro automatically checks whether monitoring has been enabled via the
    // PandoraSettings xml file. If the MONITORING preprocessor flag has not been defined, its arguments will collapse to no code.

    if (m_displayCurrentCaloHits)
    {
        const CaloHitList *pCaloHitList(NULL);
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pCaloHitList));
        std::cout << "---" << pCaloHitList->size() << " calo hits in current list " << std::endl;

        // Display additional properties for any example user-defined calo hits
        for (CaloHitList::const_iterator iter = pCaloHitList->begin(), iterEnd = pCaloHitList->end(); iter != iterEnd; ++iter)
        {
            const ExampleCaloHit *const pExampleCaloHit(dynamic_cast<const ExampleCaloHit*>(*iter));

            if (pExampleCaloHit)
            {
                std::cout << "------ExampleCaloHit " << pExampleCaloHit << ", additional property: " << pExampleCaloHit->GetAdditionalProperty() << std::endl;
            }
        }

        PANDORA_MONITORING_API(VisualizeCaloHits(this->GetPandora(), pCaloHitList, "CurrentCaloHits", GRAY));
    }

    if (m_displayCurrentTracks)
    {
        const TrackList *pTrackList(NULL);
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pTrackList));
        std::cout << "---" << pTrackList->size() << " tracks in current list " << std::endl;
        PANDORA_MONITORING_API(VisualizeTracks(this->GetPandora(), pTrackList, "CurrentTracks", MAGENTA));
    }

    if (m_displayCurrentMCParticles)
    {
        const MCParticleList *pMCParticleList(NULL);
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pMCParticleList));
        std::cout << "---" << pMCParticleList->size() << " mc particles in current list " << std::endl;
        PANDORA_MONITORING_API(VisualizeMCParticles(this->GetPandora(), pMCParticleList, "CurrentMCParticles", CYAN));
    }

    if (m_displayCurrentClusters)
    {
        const ClusterList *pClusterList(NULL);
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pClusterList));
        std::cout << "---" << pClusterList->size() << " clusters in current list " << std::endl;

        for (ClusterList::const_iterator iter = pClusterList->begin(), iterEnd = pClusterList->end(); iter != iterEnd; ++iter)
        {
            const Cluster *const pCluster(*iter);
            std::cout << "------Cluster " << pCluster << ", nHits: " << pCluster->GetNCaloHits() << std::endl;
        }

        PANDORA_MONITORING_API(VisualizeClusters(this->GetPandora(), pClusterList, "CurrentClusterList", RED));
    }

    if (m_displayCurrentVertices)
    {
        const VertexList *pVertexList(NULL);
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pVertexList));
        std::cout << "---" << pVertexList->size() << " vertices in current list " << std::endl;
        PANDORA_MONITORING_API(VisualizeVertices(this->GetPandora(), pVertexList, "CurrentVertexList", GREEN));
    }

    if (m_displayCurrentPfos)
    {
        const PfoList *pPfoList(NULL);
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pPfoList));
        std::cout << "---" << pPfoList->size() << " pfos in current list " << std::endl;

        for (PfoList::const_iterator iter = pPfoList->begin(), iterEnd = pPfoList->end(); iter != iterEnd; ++iter)
        {
            const Pfo *const pPfo(*iter);
            std::cout << "------Pfo " << pPfo << ", nClusters: " << pPfo->GetClusterList().size() << ", nVertices: " << pPfo->GetVertexList().size() << std::endl;
        }

        PANDORA_MONITORING_API(VisualizeParticleFlowObjects(this->GetPandora(), pPfoList, "CurrentPfoList", BLUE));
    }

    PANDORA_MONITORING_API(ViewEvent(this->GetPandora()));

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode DisplayListsAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "DisplayCurrentCaloHits", m_displayCurrentCaloHits));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "DisplayCurrentTracks", m_displayCurrentTracks));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "DisplayCurrentMCParticles", m_displayCurrentMCParticles));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "DisplayCurrentClusters", m_displayCurrentClusters));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "DisplayCurrentVertices", m_displayCurrentVertices));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "DisplayCurrentPfos", m_displayCurrentPfos));

    return STATUS_CODE_SUCCESS;
}

} // namespace example_content
