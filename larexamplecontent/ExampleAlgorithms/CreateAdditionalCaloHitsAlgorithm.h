/**
 *  @file   LArExampleContent/larexamplecontent/ExampleAlgorithms/CreateAdditionalCaloHitsAlgorithm.h
 * 
 *  @brief  Header file for the create additional calo hits algorithm class.
 * 
 *  $Log: $
 */
#ifndef CREATE_ADDITIONAL_CALO_HITS_ALGORITHM_H
#define CREATE_ADDITIONAL_CALO_HITS_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

namespace example_content
{

/**
 *  @brief  CreateAdditionalCaloHitsAlgorithm class
 */
class CreateAdditionalCaloHitsAlgorithm : public pandora::Algorithm
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
    CreateAdditionalCaloHitsAlgorithm();

private:
    pandora::StatusCode Run();
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    unsigned int    m_nCaloHitsToMake;                  ///< The number of new calo hits to make
    bool            m_setCurrentListToInputList;        ///< Whether to return calo hit list to the input list after hit creation

    float           m_worldSideLength;                  ///< The world volume cube side length
    float           m_groupSideLength;                  ///< The group volume cube side length
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *CreateAdditionalCaloHitsAlgorithm::Factory::CreateAlgorithm() const
{
    return new CreateAdditionalCaloHitsAlgorithm();
}

} // namespace example_content

#endif // #ifndef CREATE_ADDITIONAL_CALO_HITS_ALGORITHM_H
