/**
 *  @file   LArExampleContent/larexamplecontent/ExampleAlgorithms/MergeClustersAlgorithm.h
 * 
 *  @brief  Header file for the merge clusters algorithm class.
 * 
 *  $Log: $
 */
#ifndef MERGE_CLUSTERS_ALGORITHM_H
#define MERGE_CLUSTERS_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

namespace example_content
{

/**
 *  @brief  MergeClustersAlgorithm class
 */
class MergeClustersAlgorithm : public pandora::Algorithm
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
    MergeClustersAlgorithm();

private:
    pandora::StatusCode Run();
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    unsigned int    m_nClusterMergesToMake;     ///< The number of cluster merges to make
    float           m_maxClusterDistance;       ///< The maximum distance between clusters for merging to occur
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *MergeClustersAlgorithm::Factory::CreateAlgorithm() const
{
    return new MergeClustersAlgorithm();
}

} // namespace example_content

#endif // #ifndef MERGE_CLUSTERS_ALGORITHM_H
