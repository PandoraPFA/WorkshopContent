/**
 *  @file   WorkshopContent/examplecontent/ExampleHelpers/ExampleHelper.h
 * 
 *  @brief  Header file for the example helper algorithm class.
 * 
 *  $Log: $
 */
#ifndef EXAMPLE_HELPER_H
#define EXAMPLE_HELPER_H 1

#include "Pandora/PandoraInputTypes.h"

namespace example_content
{

/**
 *  @brief  ExampleHelper class
 */
class ExampleHelper
{
public:
    /**
     *  @brief  Simple demonstration method: find the cluster closest to a provided calo hit
     * 
     *  @param  pCaloHit address of the calo hit
     *  @param  pClusterList address of the list of candidate clusters
     *  @param  maxDistance the maximum distance to a cluster, above which clusters will not be considered
     * 
     *  @return the address of the closest calo hit
     */
    static const pandora::Cluster *FindClosestCluster(const pandora::CaloHit *const pCaloHit, const pandora::ClusterList *const pClusterList, const float maxDistance);

    /**
     *  @brief  Simple demonstration method: find the (different) cluster closest to a provided cluster
     * 
     *  @param  pCluster address of the provided cluster
     *  @param  pClusterList address of the list of candidate clusters
     *  @param  maxDistance the maximum distance to a cluster, above which clusters will not be considered
     * 
     *  @return the address of the closest (different) cluster
     */
    static const pandora::Cluster *FindClosestCluster(const pandora::Cluster *const pCluster, const pandora::ClusterList *const pClusterList, const float maxDistance);

    /**
     *  @brief  Simple demonstration method: find the vertex closest to a provided cluster
     * 
     *  @param  pCluster address of the cluster
     *  @param  pVertexListList address of the list of candidate vertices
     *  @param  maxDistance the maximum distance to a vertex, above which vertices will not be considered
     * 
     *  @return the address of the closest vertex
     */
    static const pandora::Vertex *FindClosestVertex(const pandora::Cluster *const pCluster, const pandora::VertexList *const pVertexListList, const float maxDistance);
};

} // namespace example_content

#endif // #ifndef EXAMPLE_HELPER_H
