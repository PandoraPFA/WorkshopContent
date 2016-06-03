/**
 *  @file   WorkshopContent/workshopcontent/Algorithms/TemplateAlgorithm.cc
 * 
 *  @brief  Implementation of the template algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "workshopcontent/Algorithms/TemplateAlgorithm.h"

using namespace pandora;

namespace workshop_content
{

StatusCode TemplateAlgorithm::Run()
{
    // Algorithm code here

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode TemplateAlgorithm::ReadSettings(const TiXmlHandle /*xmlHandle*/)
{
    // Read settings from xml file here

    return STATUS_CODE_SUCCESS;
}

} // namespace workshop_content
