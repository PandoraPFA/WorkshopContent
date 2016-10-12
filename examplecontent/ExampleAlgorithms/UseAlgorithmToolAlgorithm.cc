/**
 *  @file   WorkshopContent/examplecontent/ExampleAlgorithms/UseAlgorithmToolAlgorithm.cc
 * 
 *  @brief  Implementation of the use algorithm tool algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "examplecontent/ExampleAlgorithms/UseAlgorithmToolAlgorithm.h"

using namespace pandora;

namespace example_content
{

UseAlgorithmToolAlgorithm::UseAlgorithmToolAlgorithm() :
    m_anExampleUInt(1),
    m_anExampleFloatVector()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode UseAlgorithmToolAlgorithm::Run()
{
    // Algorithm tools inheriting from the IExampleAlgorithmTool interface class (defined in UseAlgorithmToolAlgorithm.h)
    // are registered with the Pandora AlgorithmManager, then can be specified under the "ExampleTools" xml tag for this algorithm.

    // Here we loop over all algorithm tools triggered via the PandoraSettings xml file, calling the example function that each
    // must provide. Algorithm tools typically process a large and expensive data object created by the calling algorithm.
    for (IExampleAlgorithmTool *const pIExampleAlgorithmTool : m_algorithmToolVector)
    {
        // Algorithm tools allow a user to drop-in (via xml) multiple methods of querying a data structure. Unlike calling daughter
        // algorithms, there is no underlying change in the Pandora list management, so it is just like being able to drop-in multiple
        // equivalent methods within an algorithm.
        pIExampleAlgorithmTool->ExampleToolFunctionality(m_anExampleUInt, m_anExampleFloatVector);
    }

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode UseAlgorithmToolAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    AlgorithmToolVector algorithmToolVector;
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ProcessAlgorithmToolList(*this, xmlHandle,
        "ExampleTools", algorithmToolVector));

    for (AlgorithmTool *const pAlgorithmTool : algorithmToolVector)
    {
        IExampleAlgorithmTool *const pIExampleAlgorithmTool(dynamic_cast<IExampleAlgorithmTool*>(pAlgorithmTool));

        if (nullptr == pIExampleAlgorithmTool)
            return STATUS_CODE_INVALID_PARAMETER;

        m_algorithmToolVector.push_back(pIExampleAlgorithmTool);
    }

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "AnExampleUInt", m_anExampleUInt));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadVectorOfValues(xmlHandle,
        "AnExampleFloatVector", m_anExampleFloatVector));

    return STATUS_CODE_SUCCESS;
}

} // namespace example_content
