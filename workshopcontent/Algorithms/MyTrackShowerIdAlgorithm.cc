/**
 *  @file   WorkshopContent/workshopcontent/Algorithms/MyTrackShowerIdAlgorithm.cc
 * 
 *  @brief  Implementation of the mytrackshowerid algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "larpandoracontent/LArHelpers/LArClusterHelper.h"
#include "larpandoracontent/LArHelpers/LArGeometryHelper.h"
#include "larpandoracontent/LArHelpers/LArMCParticleHelper.h"

#include "larpandoracontent/LArObjects/LArTwoDSlidingShowerFitResult.h"

#include "workshopcontent/Algorithms/MyTrackShowerIdAlgorithm.h"

using namespace pandora;
using namespace lar_content;

namespace workshop_content
{

MyTrackShowerIdAlgorithm::MyTrackShowerIdAlgorithm() :
    m_writeToTree(false)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

MyTrackShowerIdAlgorithm::~MyTrackShowerIdAlgorithm()
{
    if (m_writeToTree)
        PandoraMonitoringApi::SaveTree(this->GetPandora(), m_treeName.c_str(), m_fileName.c_str(), "UPDATE");
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode MyTrackShowerIdAlgorithm::Run()
{
    const ClusterList *pClusterList(nullptr);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetList(*this, m_inputClusterListName, pClusterList));

    ClusterVector sortedClusters(pClusterList->begin(), pClusterList->end());
    std::sort(sortedClusters.begin(), sortedClusters.end(), LArClusterHelper::SortByNHits);

    for (const Cluster *const pCluster : sortedClusters)
    {
        if (this->IsClearTrack(pCluster))
        {
            PandoraContentApi::Cluster::Metadata metadata;
            metadata.m_particleId = MU_MINUS;
            PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::AlterMetadata(*this, pCluster, metadata));
        }
        else
        {
            PandoraContentApi::Cluster::Metadata metadata;
            metadata.m_particleId = E_MINUS;
            PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::AlterMetadata(*this, pCluster, metadata));
        }
    }
    
    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool MyTrackShowerIdAlgorithm::IsClearTrack(const Cluster *const pCluster) const
{
    int nHits(pCluster->GetNCaloHits()), isTrueTrack(-1);
    FloatVector xPositions, zPositions;

    try
    {
        // ATTN Slightly curious definition of a clear track, but this is most-likely what is needed for shower-growing
        const MCParticle *const pMCParticle(MCParticleHelper::GetMainMCParticle(pCluster));
        
        if ((PHOTON != pMCParticle->GetParticleId()) && (E_MINUS != std::abs(pMCParticle->GetParticleId())))
        {
            isTrueTrack = 1;
        }
        else
        {
            isTrueTrack = 0;
        }
    }
    catch (StatusCodeException &)
    {
        isTrueTrack = 0;
    }
    
    CaloHitList caloHitList;
    pCluster->GetOrderedCaloHitList().GetCaloHitList(caloHitList);
    
    for (const CaloHit *const pCaloHit : caloHitList)
    {
        xPositions.push_back(pCaloHit->GetPositionVector().GetX());
        zPositions.push_back(pCaloHit->GetPositionVector().GetZ());
    }

    PandoraMonitoringApi::SetTreeVariable(this->GetPandora(), m_treeName.c_str(), "nHits", nHits);
    PandoraMonitoringApi::SetTreeVariable(this->GetPandora(), m_treeName.c_str(), "isTrueTrack", isTrueTrack);
    PandoraMonitoringApi::SetTreeVariable(this->GetPandora(), m_treeName.c_str(), "xPositions", &xPositions);
    PandoraMonitoringApi::SetTreeVariable(this->GetPandora(), m_treeName.c_str(), "zPositions", &zPositions);
    
    const unsigned int m_slidingFitWindow(20); // ATTN
    
    float straightLinePathLength(-1.f), widthSum(-1.f);

    try
    {
        // Sliding fit to i) entire Cluster ('shw'), ii) positive ('pos') and iii) negative ('neg') shower edges
        const float slidingFitPitch(LArGeometryHelper::GetWireZPitch(this->GetPandora()));
        const TwoDSlidingShowerFitResult showerFitResult(pCluster, m_slidingFitWindow, slidingFitPitch);
        
        const LayerFitResultMap &layerFitResultMapShw(showerFitResult.GetShowerFitResult().GetLayerFitResultMap());
        const LayerFitResultMap &layerFitResultMapPos(showerFitResult.GetPositiveEdgeFitResult().GetLayerFitResultMap());
        const LayerFitResultMap &layerFitResultMapNeg(showerFitResult.GetNegativeEdgeFitResult().GetLayerFitResultMap());

        // Use sliding fit to entire Cluster to define Cluster length
        CartesianVector globalMinLayerPositionOnAxis(0.f, 0.f, 0.f);
        showerFitResult.GetShowerFitResult().GetGlobalPosition(layerFitResultMapShw.begin()->second.GetL(), 0.f, globalMinLayerPositionOnAxis);
        
        CartesianVector globalMaxLayerPositionOnAxis(0.f, 0.f, 0.f);
        showerFitResult.GetShowerFitResult().GetGlobalPosition(layerFitResultMapShw.rbegin()->second.GetL(), 0.f, globalMaxLayerPositionOnAxis);

        straightLinePathLength = ((globalMaxLayerPositionOnAxis - globalMinLayerPositionOnAxis).GetMagnitude());

        // Use sliding fits to positive and negative shower edges to define Cluster width - query edge fits in layers defined by full Cluster fit
        widthSum = 0.f;

        for (const LayerFitResultMap::value_type &shwFitResultEntry : layerFitResultMapShw)
        {
            LayerFitResultMap::const_iterator iterPos = layerFitResultMapPos.find(shwFitResultEntry.first);
            LayerFitResultMap::const_iterator iterNeg = layerFitResultMapNeg.find(shwFitResultEntry.first);
            
            if ((layerFitResultMapPos.end() == iterPos) || (layerFitResultMapNeg.end() == iterNeg))
                continue;
            
            widthSum += std::fabs(iterPos->second.GetFitT() - iterNeg->second.GetFitT());
        }
    }
    catch (const StatusCodeException &statusCodeException)
    {
        std::cout << "MyTrackShowerIdAlgorithm::IsClearTrack: " << statusCodeException.ToString() << std::endl;
    }

    PandoraMonitoringApi::SetTreeVariable(this->GetPandora(), m_treeName.c_str(), "straightLinePathLength", straightLinePathLength);
    PandoraMonitoringApi::SetTreeVariable(this->GetPandora(), m_treeName.c_str(), "widthSum", widthSum);
    PandoraMonitoringApi::FillTree(this->GetPandora(), m_treeName.c_str());

    // TODO return value motivated by selection on topological properties
    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode MyTrackShowerIdAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
       "InputClusterListName", m_inputClusterListName));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "WriteToTree", m_writeToTree));
    
    if (m_writeToTree)
    {
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle, "OutputTree", m_treeName));
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle, "OutputFile", m_fileName));
    }

    return STATUS_CODE_SUCCESS;
}

} // namespace workshop_content
