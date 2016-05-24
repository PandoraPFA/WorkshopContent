/**
 *  @file   LArExampleContent/larexamplecontent/ExampleAlgorithms/CreateClustersAlgorithm.h
 * 
 *  @brief  Header file for the create clusters algorithm class.
 * 
 *  $Log: $
 */
#ifndef CREATE_CLUSTERS_ALGORITHM_H
#define CREATE_CLUSTERS_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

namespace lar_example_content
{

/**
 *  @brief  CreateClustersAlgorithm class
 */
class CreateClustersAlgorithm : public pandora::Algorithm
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
    CreateClustersAlgorithm();

private:
    pandora::StatusCode Run();
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    float           m_maxClusterHitDistance;    ///< The maximum cluster to hit distance, above which will use hit to seed a new cluster
    std::string     m_outputListName;           ///< The name under which to save the output cluster list
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *CreateClustersAlgorithm::Factory::CreateAlgorithm() const
{
    return new CreateClustersAlgorithm();
}

} // namespace lar_example_content

#endif // #ifndef CREATE_CLUSTERS_ALGORITHM_H
