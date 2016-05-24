/**
 *  @file   LArExampleContent/larexamplecontent/ExampleAlgorithmTools/ExampleAlgorithmTool.h
 * 
 *  @brief  Header file for the example algorithm tool class.
 * 
 *  $Log: $
 */
#ifndef EXAMPLE_ALGORITHM_TOOL_H
#define EXAMPLE_ALGORITHM_TOOL_H 1

#include "larexamplecontent/ExampleAlgorithms/UseAlgorithmToolAlgorithm.h"

namespace lar_example_content
{

/**
 *  @brief  ExampleAlgorithmTool class
 */
class ExampleAlgorithmTool : public IExampleAlgorithmTool
{
public:
    /**
     *  @brief  Factory class for instantiating algorithm tool
     */
    class Factory : public pandora::AlgorithmToolFactory
    {
    public:
        pandora::AlgorithmTool *CreateAlgorithmTool() const;
    };

    void ExampleToolFunctionality(const unsigned int exampleInt, const pandora::FloatVector &exampleFloatVector);

private:
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::AlgorithmTool *ExampleAlgorithmTool::Factory::CreateAlgorithmTool() const
{
    return new ExampleAlgorithmTool();
}

} // namespace lar_example_content

#endif // #ifndef EXAMPLE_ALGORITHM_TOOL_H
