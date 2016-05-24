/**
 *  @file   LArExampleContent/larexamplecontent/ExampleAlgorithms/DeleteClustersAlgorithm.h
 * 
 *  @brief  Header file for the delete clusters algorithm class.
 * 
 *  $Log: $
 */
#ifndef DELETE_CLUSTERS_ALGORITHM_H
#define DELETE_CLUSTERS_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

namespace lar_example_content
{

/**
 *  @brief  DeleteClustersAlgorithm class
 */
class DeleteClustersAlgorithm : public pandora::Algorithm
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
    DeleteClustersAlgorithm();

private:
    pandora::StatusCode Run();
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    unsigned int    m_nClustersToDelete;    ///< The number of clusters to delete
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *DeleteClustersAlgorithm::Factory::CreateAlgorithm() const
{
    return new DeleteClustersAlgorithm();
}

} // namespace lar_example_content

#endif // #ifndef DELETE_CLUSTERS_ALGORITHM_H
