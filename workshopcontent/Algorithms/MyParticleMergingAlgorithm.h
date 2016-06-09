/**
 *  @file   WorkshopContent/workshopcontent/Algorithms/MyParticleMergingAlgorithm.h
 * 
 *  @brief  Header file for the myparticlemerging algorithm class.
 * 
 *  $Log: $
 */
#ifndef WORKSHOP_MYPARTICLEMERGING_ALGORITHM_H
#define WORKSHOP_MYPARTICLEMERGING_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

namespace workshop_content
{

/**
 *  @brief  MyParticleMergingAlgorithm class
 */
class MyParticleMergingAlgorithm : public pandora::Algorithm
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

    /**
     *  @brief  Get the sorted list of input pfos
     *
     *  @param  sortedPfos to receive the sorted list of input pfos
     */
    void GetSortedPfos(pandora::PfoVector &sortedPfos) const;

    /**
     *  @brief  Merge and delete a pair of pfos, with a specific set of conventions for cluster merging, vertex use, etc.
     *
     *  @param  pPfoToEnlarge the address of the pfo to enlarge
     *  @param  pPfoToDelete the address of the pfo to delete (will become a dangling pointer)
     */
    void MergeAndDeletePfos(const pandora::ParticleFlowObject *const pPfoToEnlarge, const pandora::ParticleFlowObject *const pPfoToDelete) const;
    
    /**
     *  @brief  Select the parent cluster (same hit type and most hits) using a provided cluster list and hit type
     *
     *  @param  clusterList the cluster list
     *  @param  hitType the hit type
     *
     *  @return the address of the parent cluster
     */
    const pandora::Cluster *GetParentCluster(const pandora::ClusterList &clusterList, const pandora::HitType hitType) const;
    
    /**
     *  @brief  Find the name of the list hosting a specific object
     *
     *  @param  pT the address of the object
     *
     *  @return the name of the list
     */
    template <typename T>
    const std::string GetListName(const T *const pT) const;

    pandora::StringVector   m_inputPfoListNames;    ///< The list of input pfo list names
    pandora::StringVector   m_daughterListNames;    ///< The list of daughter list names
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *MyParticleMergingAlgorithm::Factory::CreateAlgorithm() const
{
    return new MyParticleMergingAlgorithm();
}

} // namespace workshop_content

#endif // #ifndef WORKSHOP_MYPARTICLEMERGING_ALGORITHM_H
