/**
 *  @file   LArExampleContent/larexamplecontent/ExampleAlgorithms/CreatePfosAlgorithm.h
 * 
 *  @brief  Header file for the create pfos algorithm class.
 * 
 *  $Log: $
 */
#ifndef CREATE_PFOS_ALGORITHM_H
#define CREATE_PFOS_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

namespace example_content
{

/**
 *  @brief  CreatePfosAlgorithm class
 */
class CreatePfosAlgorithm : public pandora::Algorithm
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

private:
    pandora::StatusCode Run();
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    std::string     m_outputListName;   ///< The name under which to save the output pfo list
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *CreatePfosAlgorithm::Factory::CreateAlgorithm() const
{
    return new CreatePfosAlgorithm();
}

} // namespace example_content

#endif // #ifndef CREATE_PFOS_ALGORITHM_H
