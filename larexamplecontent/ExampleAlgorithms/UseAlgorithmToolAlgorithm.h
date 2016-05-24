/**
 *  @file   LArExampleContent/larexamplecontent/ExampleAlgorithms/UseAlgorithmToolAlgorithm.h
 * 
 *  @brief  Header file for the use algorithm tool algorithm class.
 * 
 *  $Log: $
 */
#ifndef USE_ALGORITHM_TOOL_ALGORITHM_H
#define USE_ALGORITHM_TOOL_ALGORITHM_H 1

#include "Pandora/Algorithm.h"
#include "Pandora/AlgorithmTool.h"

namespace example_content
{

/**
 *  @brief  IExampleAlgorithmTool class
 */
class IExampleAlgorithmTool : public pandora::AlgorithmTool
{
public:
    /**
     *  @brief  Use the example algorithm tool functionality
     *
     *  @param  exampleInt an example int
     *  @param  exampleFloatVector an example float vector
     */
    virtual void ExampleToolFunctionality(const unsigned int exampleInt, const pandora::FloatVector &exampleFloatVector) = 0;
};

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

/**
 *  @brief  UseAlgorithmToolAlgorithm class
 */
class UseAlgorithmToolAlgorithm : public pandora::Algorithm
{
public:
    /**
     *  @brief  Factory class for instantiating algorithm
     */
    class Factory : public pandora::AlgorithmFactory
    {
    public:
        pandora::Algorithm *CreateAlgorithm() const;
    };

    /**
     *  @brief  Default constructor
     */
    UseAlgorithmToolAlgorithm();

private:
    pandora::StatusCode Run();
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    typedef std::vector<IExampleAlgorithmTool*> ExampleAlgorithmToolList;   ///< The algorithm tool list typedef
    ExampleAlgorithmToolList    m_algorithmToolList;                        ///< The algorithm tool list

    unsigned int                m_anExampleUInt;                            ///< An example unsigned int member variable
    pandora::FloatVector        m_anExampleFloatVector;                     ///< An example float vector
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *UseAlgorithmToolAlgorithm::Factory::CreateAlgorithm() const
{
    return new UseAlgorithmToolAlgorithm();
}

} // namespace example_content

#endif // #ifndef USE_ALGORITHM_TOOL_ALGORITHM_H
