/**
 *  @file   WorkshopContent/examplecontent/ExampleAlgorithms/CreateVerticesAlgorithm.h
 * 
 *  @brief  Header file for the create vertices algorithm class.
 * 
 *  $Log: $
 */
#ifndef CREATE_VERTICES_ALGORITHM_H
#define CREATE_VERTICES_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

namespace example_content
{

/**
 *  @brief  CreateVerticesAlgorithm class
 */
class CreateVerticesAlgorithm : public pandora::Algorithm
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

    std::string     m_outputListName;   ///< The name under which to save the output vertex list
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *CreateVerticesAlgorithm::Factory::CreateAlgorithm() const
{
    return new CreateVerticesAlgorithm();
}

} // namespace example_content

#endif // #ifndef CREATE_VERTICES_ALGORITHM_H
