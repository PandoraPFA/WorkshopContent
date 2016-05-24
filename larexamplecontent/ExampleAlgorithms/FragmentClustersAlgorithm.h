/**
 *  @file   LArExampleContent/larexamplecontent/ExampleAlgorithms/FragmentClustersAlgorithm.h
 * 
 *  @brief  Header file for the fragment clusters algorithm class.
 * 
 *  $Log: $
 */
#ifndef FRAGMENT_CLUSTERS_ALGORITHM_H
#define FRAGMENT_CLUSTERS_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

namespace example_content
{

/**
 *  @brief  FragmentClustersAlgorithm class
 */
class FragmentClustersAlgorithm : public pandora::Algorithm
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
    FragmentClustersAlgorithm();

private:
    pandora::StatusCode Run();
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    /**
     *  @brief  Creating m_nFragmentsPerCluster new fragment clusters from the current calo hit list (which will have
     *          been set up, by the fragmentation mechanism, as those hits in the original input cluster).
     */
    pandora::StatusCode PerformFragmentation() const;

    unsigned int    m_nClustersToFragment;      ///< The number of clusters to fragment
    unsigned int    m_nFragmentsPerCluster;     ///< The number of fragment pieces per cluster
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *FragmentClustersAlgorithm::Factory::CreateAlgorithm() const
{
    return new FragmentClustersAlgorithm();
}

} // namespace example_content

#endif // #ifndef FRAGMENT_CLUSTERS_ALGORITHM_H
