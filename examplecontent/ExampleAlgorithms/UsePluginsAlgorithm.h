/**
 *  @file   WorkshopContent/examplecontent/ExampleAlgorithms/UsePluginsAlgorithm.h
 * 
 *  @brief  Header file for the use plugins algorithm class.
 * 
 *  $Log: $
 */
#ifndef USE_PLUGINS_ALGORITHM_H
#define USE_PLUGINS_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

namespace example_content
{

/**
 *  @brief  UsePluginsAlgorithm class
 */
class UsePluginsAlgorithm : public pandora::Algorithm
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
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *UsePluginsAlgorithm::Factory::CreateAlgorithm() const
{
    return new UsePluginsAlgorithm();
}

} // namespace example_content

#endif // #ifndef USE_PLUGINS_ALGORITHM_H
