/**
 *  @file   WorkshopContent/examplecontent/ExampleAlgorithms/CreateAdditionalCaloHitsAlgorithm.cc
 * 
 *  @brief  Implementation of the create additional calo hits algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "examplecontent/ExampleAlgorithms/CreateAdditionalCaloHitsAlgorithm.h"

#include "examplecontent/ExampleObjects/ExampleCaloHit.h"

using namespace pandora;

namespace example_content
{

CreateAdditionalCaloHitsAlgorithm::CreateAdditionalCaloHitsAlgorithm() :
    m_nCaloHitsToMake(0),
    m_setCurrentListToInputList(false),
    m_worldSideLength(1000.f),
    m_groupSideLength(10.f),
    m_randomEngine(12345)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode CreateAdditionalCaloHitsAlgorithm::Run()
{
    // Create m_nCaloHitsToMake additional calo hits, following the basic recipe set-out in the PandoraExample application.
    // One grouping of hits about a single central position in the world volume is created.

    // Here add complexity (mostly hidden-away in the ExampleCaloHitFactory) to add extra parameters to the base pandora calo hit
    ExampleCaloHitFactory exampleCaloHitFactory;
    std::uniform_real_distribution<float> randomDistribution(0.f, 1.f);

    const CartesianVector centrePosition(
        (randomDistribution(m_randomEngine) - 0.5f) * m_worldSideLength,
        (randomDistribution(m_randomEngine) - 0.5f) * m_worldSideLength,
        (randomDistribution(m_randomEngine) - 0.5f) * m_worldSideLength);

    for (unsigned int iHit = 0; iHit < m_nCaloHitsToMake; ++iHit)
    {
        const CartesianVector localPosition(
            (randomDistribution(m_randomEngine) - 0.5f) * m_groupSideLength,
            (randomDistribution(m_randomEngine) - 0.5f) * m_groupSideLength,
            (randomDistribution(m_randomEngine) - 0.5f) * m_groupSideLength);

        // Mainly dummy parameters
        ExampleCaloHitParameters parameters;
        parameters.m_additionalProperty = "ExampleAdditionalProperty";
        parameters.m_positionVector = localPosition + centrePosition;
        parameters.m_expectedDirection = CartesianVector(0.f, 0.f, 1.f);
        parameters.m_cellNormalVector = CartesianVector(0.f, 0.f, 1.f);
        parameters.m_cellGeometry = RECTANGULAR;
        parameters.m_cellSize0 = 1.f;
        parameters.m_cellSize1 = 1.f;
        parameters.m_cellThickness = 1.f;
        parameters.m_nCellRadiationLengths = 1.f;
        parameters.m_nCellInteractionLengths = 1.f;
        parameters.m_time = 0.f;
        parameters.m_inputEnergy = 1.f;
        parameters.m_mipEquivalentEnergy = 1.f;
        parameters.m_electromagneticEnergy = 1.f;
        parameters.m_hadronicEnergy = 1.f;
        parameters.m_isDigital = false;
        parameters.m_hitType = HIT_CUSTOM;
        parameters.m_hitRegion = SINGLE_REGION;
        parameters.m_layer = 0;
        parameters.m_isInOuterSamplingLayer = false;
        parameters.m_pParentAddress = (void*)(static_cast<uintptr_t>(iHit));

        const CaloHit *pCaloHit(nullptr);
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::CaloHit::Create(*this, parameters, pCaloHit, exampleCaloHitFactory));
    }

    // Newly created calo hits always appear in the Input list. This list may not necessarily be the current list. The API below
    // will replace the current calo hit list with the Input list for this and subsequent algorithms.
    if (m_setCurrentListToInputList)
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::ReplaceCurrentList<CaloHit>(*this, "Input"));

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode CreateAdditionalCaloHitsAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "NCaloHitsToMake", m_nCaloHitsToMake));

    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "SetCurrentListToInputList", m_setCurrentListToInputList));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "WorldSideLength", m_worldSideLength));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "GroupSideLength", m_groupSideLength));

    return STATUS_CODE_SUCCESS;
}

} // namespace example_content
