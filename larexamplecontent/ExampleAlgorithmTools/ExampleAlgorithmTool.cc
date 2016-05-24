/**
 *  @file   LArExampleContent/larexamplecontent/ExampleAlgorithmTools/ExampleAlgorithmTool.cc
 * 
 *  @brief  Implementation of the example algorithm tool class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "larexamplecontent/ExampleAlgorithmTools/ExampleAlgorithmTool.h"

using namespace pandora;

namespace lar_example_content
{

void ExampleAlgorithmTool::ExampleToolFunctionality(const unsigned int /*exampleInt*/, const pandora::FloatVector &/*exampleFloatVector*/)
{
    if (this->GetPandora().GetSettings()->ShouldDisplayAlgorithmInfo())
       std::cout << "----> Running Algorithm Tool: " << this << ", " << this->GetType() << std::endl;

    // Typically receive large objects constructed by a parent algorithm and process within the tool implementation...
    
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode ExampleAlgorithmTool::ReadSettings(const TiXmlHandle /*xmlHandle*/)
{
    // Read settings from xml file here

    return STATUS_CODE_SUCCESS;
}

} // namespace lar_example_content
