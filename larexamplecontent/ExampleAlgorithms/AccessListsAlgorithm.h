/**
 *  @file   LArExampleContent/larexamplecontent/ExampleAlgorithms/AccessListsAlgorithm.h
 * 
 *  @brief  Header file for the access lists algorithm class.
 * 
 *  $Log: $
 */
#ifndef ACCESS_LISTS_ALGORITHM_H
#define ACCESS_LISTS_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

namespace example_content
{

/**
 *  @brief  AccessListsAlgorithm class
 */
class AccessListsAlgorithm : public pandora::Algorithm
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

    std::string     m_requestedCaloHitListName;         ///< The requested calo hit list name
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *AccessListsAlgorithm::Factory::CreateAlgorithm() const
{
    return new AccessListsAlgorithm();
}

} // namespace example_content

#endif // #ifndef ACCESS_LISTS_ALGORITHM_H
