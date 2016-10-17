/**
 *  @file   WorkshopContent/workshopcontent/Algorithms/MyParticleCreationAlgorithm.cc
 * 
 *  @brief  Implementation of the myparticlecreation algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "larpandoracontent/LArHelpers/LArClusterHelper.h"
#include "larpandoracontent/LArHelpers/LArGeometryHelper.h"

#include "larpandoracontent/LArObjects/LArTwoDSlidingFitResult.h"

#include "workshopcontent/Algorithms/MyParticleCreationAlgorithm.h"

using namespace pandora;
using namespace lar_content;

namespace workshop_content
{

MyParticleCreationAlgorithm::MyParticleCreationAlgorithm() :
    m_minClusterCaloHits(5),
    m_slidingFitWindow(20),
    m_nSamplingPoints(100)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------


StatusCode MyParticleCreationAlgorithm::Run()
{
    ClusterVector sortedLongClustersU, sortedLongClustersV, sortedLongClustersW;
    this->GetSortedLongClusters(sortedLongClustersU, sortedLongClustersV, sortedLongClustersW);

    const PfoList *pTemporaryList(nullptr); std::string temporaryListName;
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::CreateTemporaryListAndSetCurrent(*this, pTemporaryList, temporaryListName));

    const Cluster *pBestClusterU(nullptr), *pBestClusterV(nullptr), *pBestClusterW(nullptr);
    while (this->GetBestParticle(sortedLongClustersU, sortedLongClustersV, sortedLongClustersW, pBestClusterU, pBestClusterV, pBestClusterW))
    {
        this->CreateParticle(pBestClusterU, pBestClusterV, pBestClusterW);
    }

    if (!pTemporaryList->empty())
    {
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::SaveList<Pfo>(*this, m_outputPfoListName));
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::ReplaceCurrentList<Pfo>(*this, m_outputPfoListName));
    }

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void MyParticleCreationAlgorithm::GetSortedLongClusters(ClusterVector &sortedLongClustersU, ClusterVector &sortedLongClustersV,
    ClusterVector &sortedLongClustersW) const
{
    this->GetSortedLongClusters(m_inputClusterListNameU, sortedLongClustersU);
    this->GetSortedLongClusters(m_inputClusterListNameV, sortedLongClustersV);
    this->GetSortedLongClusters(m_inputClusterListNameW, sortedLongClustersW);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void MyParticleCreationAlgorithm::GetSortedLongClusters(const std::string &inputClusterListName, ClusterVector &sortedLongClusters) const
{
    const ClusterList *pClusterList(nullptr);
    PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetList(*this, inputClusterListName, pClusterList));

    for (const Cluster *const pCluster : *pClusterList)
    {
        if (pCluster->GetNCaloHits() > m_minClusterCaloHits)
            sortedLongClusters.push_back(pCluster);
    }
    
    std::sort(sortedLongClusters.begin(), sortedLongClusters.end(), LArClusterHelper::SortByNHits);
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool MyParticleCreationAlgorithm::GetBestParticle(const ClusterVector &sortedLongClustersU, const ClusterVector &sortedLongClustersV,
    const ClusterVector &sortedLongClustersW, const Cluster *&pBestClusterU, const Cluster *&pBestClusterV, const Cluster *&pBestClusterW) const
{
    float bestOverlapFigureOfMerit(std::numeric_limits<float>::epsilon());
    pBestClusterU = nullptr; pBestClusterV = nullptr; pBestClusterW = nullptr;

    for (const Cluster *const pClusterU : sortedLongClustersU)
    {
        for (const Cluster *const pClusterV : sortedLongClustersV)
        {
            for (const Cluster *const pClusterW : sortedLongClustersW)
            {
                if (!PandoraContentApi::IsAvailable(*this, pClusterU) ||
                    !PandoraContentApi::IsAvailable(*this, pClusterV) ||
                    !PandoraContentApi::IsAvailable(*this, pClusterW))
                {
                    continue;
                }

                const float overlapFigureOfMerit(this->GetOverlapFigureOfMerit(pClusterU, pClusterV, pClusterW));

                if (overlapFigureOfMerit > bestOverlapFigureOfMerit)
                {
                    bestOverlapFigureOfMerit = overlapFigureOfMerit;
                    pBestClusterU = pClusterU;
                    pBestClusterV = pClusterV;
                    pBestClusterW = pClusterW;
                }
            }
        }
    }

    return (pBestClusterU && pBestClusterV && pBestClusterW);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void MyParticleCreationAlgorithm::CreateParticle(const Cluster *const pClusterU, const Cluster *const pClusterV,
    const Cluster *const pClusterW) const
{
    PandoraContentApi::ParticleFlowObject::Parameters pfoParameters;

    pfoParameters.m_particleId = MU_MINUS; // Track
    pfoParameters.m_charge = PdgTable::GetParticleCharge(pfoParameters.m_particleId.Get());
    pfoParameters.m_mass = PdgTable::GetParticleMass(pfoParameters.m_particleId.Get());
    pfoParameters.m_energy = 0.f;
    pfoParameters.m_momentum = CartesianVector(0.f, 0.f, 0.f);

    pfoParameters.m_clusterList.push_back(pClusterU);
    pfoParameters.m_clusterList.push_back(pClusterV);
    pfoParameters.m_clusterList.push_back(pClusterW);
    
    const ParticleFlowObject *pPfo(nullptr);
    PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::ParticleFlowObject::Create(*this, pfoParameters, pPfo));
}

//------------------------------------------------------------------------------------------------------------------------------------------

float MyParticleCreationAlgorithm::GetOverlapFigureOfMerit(const Cluster *const pClusterU, const Cluster *const pClusterV,
    const Cluster *const pClusterW) const
{
    try
    {
        const float slidingFitPitch(LArGeometryHelper::GetWireZPitch(this->GetPandora()));
        const TwoDSlidingFitResult fitResultU(pClusterU, m_slidingFitWindow, slidingFitPitch);
        const TwoDSlidingFitResult fitResultV(pClusterV, m_slidingFitWindow, slidingFitPitch);
        const TwoDSlidingFitResult fitResultW(pClusterW, m_slidingFitWindow, slidingFitPitch);

        // ATTN Presence of more than one fit segment means complicated trajectory, winding back and forth in x (skip over here)
        if ((1 != fitResultU.GetFitSegmentList().size()) || (1 != fitResultV.GetFitSegmentList().size()) || (1 != fitResultW.GetFitSegmentList().size()))
            return 0.f;

        const FitSegment &fitSegmentU(fitResultU.GetFitSegmentList().front());
        const FitSegment &fitSegmentV(fitResultV.GetFitSegmentList().front());
        const FitSegment &fitSegmentW(fitResultW.GetFitSegmentList().front());
        
        float pseudoChi2Sum(0.f);
        const unsigned int nPoints(m_nSamplingPoints);
        const float minX(std::max(fitSegmentU.GetMinX(), std::max(fitSegmentV.GetMinX(), fitSegmentW.GetMinX())));
        const float maxX(std::min(fitSegmentU.GetMaxX(), std::min(fitSegmentV.GetMaxX(), fitSegmentW.GetMaxX())));
        
        for (unsigned int n = 0; n <= nPoints; ++n)
        {
            const float x(minX + (maxX - minX) * static_cast<float>(n) / static_cast<float>(nPoints));
            
            CartesianVector fitUVector(0.f, 0.f, 0.f), fitVVector(0.f, 0.f, 0.f), fitWVector(0.f, 0.f, 0.f);
            CartesianVector fitUDirection(0.f, 0.f, 0.f), fitVDirection(0.f, 0.f, 0.f), fitWDirection(0.f, 0.f, 0.f);
            
            if ((STATUS_CODE_SUCCESS != fitResultU.GetTransverseProjection(x, fitSegmentU, fitUVector, fitUDirection)) ||
                (STATUS_CODE_SUCCESS != fitResultV.GetTransverseProjection(x, fitSegmentV, fitVVector, fitVDirection)) ||
                (STATUS_CODE_SUCCESS != fitResultW.GetTransverseProjection(x, fitSegmentW, fitWVector, fitWDirection)))
            {
                continue;
            }
            
            const float u(fitUVector.GetZ()), v(fitVVector.GetZ()), w(fitWVector.GetZ());
            const float uv2w(LArGeometryHelper::MergeTwoPositions(this->GetPandora(), TPC_VIEW_U, TPC_VIEW_V, u, v));
            const float uw2v(LArGeometryHelper::MergeTwoPositions(this->GetPandora(), TPC_VIEW_U, TPC_VIEW_W, u, w));
            const float vw2u(LArGeometryHelper::MergeTwoPositions(this->GetPandora(), TPC_VIEW_V, TPC_VIEW_W, v, w));
            
            const float deltaU((vw2u - u) * fitUDirection.GetX());
            const float deltaV((uw2v - v) * fitVDirection.GetX());
            const float deltaW((uv2w - w) * fitWDirection.GetX());
            
            const float pseudoChi2(deltaW * deltaW + deltaV * deltaV + deltaU * deltaU);
            pseudoChi2Sum += pseudoChi2;
        }
        
        return pseudoChi2Sum;
    }
    catch (const StatusCodeException &statusCodeException)
    {
        std::cout << "MyParticleCreationAlgorithm::AreClustersAssociated " << statusCodeException.ToString() << std::endl;
    }

    return 0.f;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode MyParticleCreationAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "InputClusterListNameU", m_inputClusterListNameU));

    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "InputClusterListNameV", m_inputClusterListNameV));

    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "InputClusterListNameW", m_inputClusterListNameW));

    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "OutputPfoListName", m_outputPfoListName));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
         "MinClusterCaloHits", m_minClusterCaloHits));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
         "SlidingFitWindow", m_slidingFitWindow));
    m_nSamplingPoints=100;
    return STATUS_CODE_SUCCESS;
}

} // namespace workshop_content
