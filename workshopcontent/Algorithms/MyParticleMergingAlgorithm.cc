/**
 *  @file   WorkshopContent/workshopcontent/Algorithms/MyParticleMergingAlgorithm.cc
 * 
 *  @brief  Implementation of the myparticlemerging algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "workshopcontent/Algorithms/MyParticleMergingAlgorithm.h"

using namespace pandora;

namespace workshop_content
{

StatusCode MyParticleMergingAlgorithm::Run()
{
    // Algorithm code here

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode MyParticleMergingAlgorithm::ReadSettings(const TiXmlHandle /*xmlHandle*/)
{
    // Read settings from xml file here

    return STATUS_CODE_SUCCESS;
}

} // namespace workshop_content
