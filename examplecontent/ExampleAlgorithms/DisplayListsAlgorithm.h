/**
 *  @file   WorkshopContent/examplecontent/ExampleAlgorithms/DisplayListsAlgorithm.h
 * 
 *  @brief  Header file for the display lists algorithm class.
 * 
 *  $Log: $
 */
#ifndef DISPLAY_LISTS_ALGORITHM_H
#define DISPLAY_LISTS_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

namespace example_content
{

/**
 *  @brief  DisplayListsAlgorithm class
 */
class DisplayListsAlgorithm : public pandora::Algorithm
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
    DisplayListsAlgorithm();

private:
    pandora::StatusCode Run();
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    bool    m_displayCurrentCaloHits;       ///< Whether to display the current calo hit list
    bool    m_displayCurrentTracks;         ///< Whether to display the current track list
    bool    m_displayCurrentMCParticles;    ///< Whether to display the current mc particle list
    bool    m_displayCurrentClusters;       ///< Whether to display the current cluster list
    bool    m_displayCurrentVertices;       ///< Whether to display the current vertex list
    bool    m_displayCurrentPfos;           ///< Whether to display the current pfo list
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *DisplayListsAlgorithm::Factory::CreateAlgorithm() const
{
    return new DisplayListsAlgorithm();
}

} // namespace example_content

#endif // #ifndef DISPLAY_LISTS_ALGORITHM_H
