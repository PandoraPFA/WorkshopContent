/**
 *  @file   WorkshopContent/examplecontent/ExampleAlgorithms/CreateClustersDaughterAlgorithm.h
 * 
 *  @brief  Header file for the create clusters daughter algorithm class.
 * 
 *  $Log: $
 */
#ifndef CREATE_CLUSTERS_DAUGHTER_ALGORITHM_H
#define CREATE_CLUSTERS_DAUGHTER_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

namespace example_content
{

/**
 *  @brief  CreateClustersDaughterAlgorithm class
 */
class CreateClustersDaughterAlgorithm : public pandora::Algorithm
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
    CreateClustersDaughterAlgorithm();

private:
    pandora::StatusCode Run();
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    unsigned int    m_nClustersToMake;  ///< The number of clusters to seed (if sufficient hits) before adding further hits to existing clusters
    std::string     m_outputListName;   ///< The name under which to save the output cluster list
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *CreateClustersDaughterAlgorithm::Factory::CreateAlgorithm() const
{
    return new CreateClustersDaughterAlgorithm();
}

} // namespace example_content

#endif // #ifndef CREATE_CLUSTERS_DAUGHTER_ALGORITHM_H
