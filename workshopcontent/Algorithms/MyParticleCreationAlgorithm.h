/**
 *  @file   WorkshopContent/workshopcontent/Algorithms/MyParticleCreationAlgorithm.h
 * 
 *  @brief  Header file for the myparticlecreation algorithm class.
 * 
 *  $Log: $
 */
#ifndef WORKSHOP_MYPARTICLECREATION_ALGORITHM_H
#define WORKSHOP_MYPARTICLECREATION_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

namespace workshop_content
{

/**
 *  @brief  MyParticleCreationAlgorithm class
 */
class MyParticleCreationAlgorithm : public pandora::Algorithm
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
    MyParticleCreationAlgorithm();

private:
    pandora::StatusCode Run();
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    /**
     *  @brief  Use the provided list names to read input cluster lists, select clusters passing cuts and store in sorted containers
     *
     *  @param  sortedLongClustersU to receive the sorted list of long clusters in the u view
     *  @param  sortedLongClustersV to receive the sorted list of long clusters in the v view
     *  @param  sortedLongClustersW to receive the sorted list of long clusters in the w view
     */
    void GetSortedLongClusters(pandora::ClusterVector &sortedLongClustersU, pandora::ClusterVector &sortedLongClustersV,
        pandora::ClusterVector &sortedLongClustersW) const;

    /**
     *  @brief  Use the provided list name to read input cluster lists, select clusters passing cuts and store in sorted container
     *
     *  @param  inputClusterListName the input cluster list name
     *  @param  sortedLongClustersV to receive the sorted list of long clusters
     */
    void GetSortedLongClusters(const std::string &inputClusterListName, pandora::ClusterVector &sortedLongClusters) const;

    /**
     *  @brief  Find the combination of u, v and w clusters that form the best, most plausible candidate particle
     *
     *  @param  sortedLongClustersU the sorted list of long clusters in the u view
     *  @param  sortedLongClustersV the sorted list of long clusters in the v view
     *  @param  sortedLongClustersW the sorted list of long clusters in the w view
     *  @param  pBestClusterU to receive the address of the u cluster identified as part of the best candidate particle
     *  @param  pBestClusterV to receive the address of the v cluster identified as part of the best candidate particle
     *  @param  pBestClusterW to receive the address of the w cluster identified as part of the best candidate particle
     *
     *  @return whether a candidate particle has been identified
     */
    bool GetBestParticle(const pandora::ClusterVector &sortedLongClustersU, const pandora::ClusterVector &sortedLongClustersV,
        const pandora::ClusterVector &sortedLongClustersW, const pandora::Cluster *&pBestClusterU, const pandora::Cluster *&pBestClusterV,
        const pandora::Cluster *&pBestClusterW) const;

    /**
     *  @brief  Create a new particle containing the provided combination of u, v and w clusters
     *
     *  @param  pClusterU the address of the u cluster for inclusion in the particle
     *  @param  pClusterV the address of the v cluster for inclusion in the particle
     *  @param  pClusterW the address of the w cluster for inclusion in the particle
     */
    void CreateParticle(const pandora::Cluster *const pClusterU, const pandora::Cluster *const pClusterV,
        const pandora::Cluster *const pClusterW) const;

    /**
     *  @brief  Get a figure of merit characterising the overlap agreement between a combination of u, v and w clusters
     *
     *  @param  pClusterU the address of the u cluster
     *  @param  pClusterV the address of the v cluster
     *  @param  pClusterW the address of the w cluster
     * 
     *  @return the figure of merit
     */
    float GetOverlapFigureOfMerit(const pandora::Cluster *const pClusterU, const pandora::Cluster *const pClusterV,
        const pandora::Cluster *const pClusterW) const;

    std::string     m_inputClusterListNameU;    ///<
    std::string     m_inputClusterListNameV;    ///<
    std::string     m_inputClusterListNameW;    ///<

    std::string     m_outputPfoListName;        ///<

    unsigned int    m_minClusterCaloHits;       ///<
    unsigned int    m_slidingFitWindow;         ///<
    unsigned int    m_nSamplingPoints;          ///<
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *MyParticleCreationAlgorithm::Factory::CreateAlgorithm() const
{
    return new MyParticleCreationAlgorithm();
}

} // namespace workshop_content

#endif // #ifndef WORKSHOP_MYPARTICLECREATION_ALGORITHM_H
