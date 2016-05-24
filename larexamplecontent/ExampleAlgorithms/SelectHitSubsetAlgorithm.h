/**
 *  @file   LArExampleContent/larexamplecontent/ExampleAlgorithms/SelectHitSubsetAlgorithm.h
 * 
 *  @brief  Header file for the select hit subset algorithm class.
 * 
 *  $Log: $
 */
#ifndef SELECT_HIT_SUBSET_ALGORITHM_H
#define SELECT_HIT_SUBSET_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

namespace example_content
{

/**
 *  @brief  SelectHitSubsetAlgorithm class
 */
class SelectHitSubsetAlgorithm : public pandora::Algorithm
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
    SelectHitSubsetAlgorithm();

private:
    pandora::StatusCode Run();
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    float           m_hitSelectionFraction;     ///< The fraction of the input hits to select
    std::string     m_outputListName;           ///< The name under which to save the output pfo list
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *SelectHitSubsetAlgorithm::Factory::CreateAlgorithm() const
{
    return new SelectHitSubsetAlgorithm();
}

} // namespace example_content

#endif // #ifndef SELECT_HIT_SUBSET_ALGORITHM_H
