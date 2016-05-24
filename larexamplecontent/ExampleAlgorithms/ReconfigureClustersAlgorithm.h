/**
 *  @file   LArExampleContent/larexamplecontent/ExampleAlgorithms/ReconfigureClustersAlgorithm.h
 * 
 *  @brief  Header file for the reconfigure clusters algorithm class.
 * 
 *  $Log: $
 */
#ifndef RECONFIGURE_CLUSTERS_ALGORITHM_H
#define RECONFIGURE_CLUSTERS_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

namespace lar_example_content
{

/**
 *  @brief  ReconfigureClustersAlgorithm class
 */
class ReconfigureClustersAlgorithm : public pandora::Algorithm
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
    ReconfigureClustersAlgorithm();

private:
    pandora::StatusCode Run();
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    unsigned int            m_nClustersToReconfigure;       ///< The number of clusters to reconfigure
    pandora::StringVector   m_clusteringAlgorithms;         ///< The ordered list of clustering algorithms to be used
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *ReconfigureClustersAlgorithm::Factory::CreateAlgorithm() const
{
    return new ReconfigureClustersAlgorithm();
}

} // namespace lar_example_content

#endif // #ifndef RECONFIGURE_CLUSTERS_ALGORITHM_H
