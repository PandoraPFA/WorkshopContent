/**
 *  @file   WorkshopContent/workshopcontent/Algorithms/MyTrackShowerIdAlgorithm.h
 * 
 *  @brief  Header file for the mytrackshowerid algorithm class.
 * 
 *  $Log: $
 */
#ifndef WORKSHOP_MYTRACKSHOWERID_ALGORITHM_H
#define WORKSHOP_MYTRACKSHOWERID_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

namespace workshop_content
{

/**
 *  @brief  MyTrackShowerIdAlgorithm class
 */
class MyTrackShowerIdAlgorithm : public pandora::Algorithm
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
    MyTrackShowerIdAlgorithm();

    /**
     *  @brief  Destructor
     */
    ~MyTrackShowerIdAlgorithm();

private:
    pandora::StatusCode Run();
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    /**
     *  @brief  Whether cluster is identified as a clear track
     *
     *  @param  pCluster address of the relevant cluster
     *
     *  @return boolean
     */
    bool IsClearTrack(const pandora::Cluster *const pCluster) const;

    std::string     m_inputClusterListName;         ///< The input cluster list name
    bool            m_writeToTree;                  ///< Whether to write monitoring details to tree
    std::string     m_treeName;                     ///< Name of output tree
    std::string     m_fileName;                     ///< Name of output file
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *MyTrackShowerIdAlgorithm::Factory::CreateAlgorithm() const
{
    return new MyTrackShowerIdAlgorithm();
}

} // namespace workshop_content

#endif // #ifndef WORKSHOP_MYTRACKSHOWERID_ALGORITHM_H
