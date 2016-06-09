/**
 *  @file   WorkshopContent/workshopcontent/Algorithms/MyClusterMergingAlgorithm.h
 * 
 *  @brief  Header file for the myclustermerging algorithm class.
 * 
 *  $Log: $
 */
#ifndef WORKSHOP_MYCLUSTERMERGING_ALGORITHM_H
#define WORKSHOP_MYCLUSTERMERGING_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

namespace workshop_content
{

/**
 *  @brief  MyClusterMergingAlgorithm class
 */
class MyClusterMergingAlgorithm : public pandora::Algorithm
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
    MyClusterMergingAlgorithm();

private:
    pandora::StatusCode Run();
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    /**
     *  @brief  Examine an input cluster list, providing a sorted container of long clusters
     *
     *  @param  pClusterList the address of the input cluster list
     *  @param  sortedLongClusters to receive the sorted vector of long clusters
     */
    void GetSortedLongClusters(const pandora::ClusterList *const pClusterList, pandora::ClusterVector &sortedLongClusters) const;

    /**
     *  @brief  Whether two clusters are associated and should be merged
     *
     *  @param  pParentCluster the address of the candidate parent cluster
     *  @param  pDaughterCluster the address of the candidate daughter cluster
     * 
     *  @return boolean
     */
    bool AreClustersAssociated(const pandora::Cluster *const pParentCluster, const pandora::Cluster *const pDaughterCluster) const;
    
    // Member variables here
    std::string     m_inputClusterListName;     ///< The input cluster list name
    unsigned int    m_minClusterCaloHits;       ///< The minimum number of hits in a candidate cluster
    float           m_maxClusterSeparation;     ///< The maximum separation between candidate clusters to allow merge
    float           m_maxImpactL;               ///<
    float           m_maxImpactT;               ///<
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *MyClusterMergingAlgorithm::Factory::CreateAlgorithm() const
{
    return new MyClusterMergingAlgorithm();
}

} // namespace workshop_content

#endif // #ifndef WORKSHOP_MYCLUSTERMERGING_ALGORITHM_H
