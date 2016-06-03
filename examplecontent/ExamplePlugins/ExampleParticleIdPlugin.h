/**
 *  @file   WorkshopContent/examplecontent/ExamplePlugins/ExampleParticleIdPlugin.h
 * 
 *  @brief  Header file for the example particle id plugin class.
 * 
 *  $Log: $
 */
#ifndef EXAMPLE_PARTICLE_ID_PLUGIN_H
#define EXAMPLE_PARTICLE_ID_PLUGIN_H 1

#include "Plugins/ParticleIdPlugin.h"

namespace example_content
{

/**
 *   @brief  ExampleParticleIdPlugin class
 */
class ExampleParticleIdPlugin : public pandora::ParticleIdPlugin
{
public:
    /**
     *  @brief  Default constructor
     */
    ExampleParticleIdPlugin();

    bool IsMatch(const pandora::Cluster *const pCluster) const;

private:
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    unsigned int    m_exampleParameter;     ///< An example parameter
};

} // namespace example_content

#endif // #ifndef EXAMPLE_PARTICLE_ID_PLUGIN_H
