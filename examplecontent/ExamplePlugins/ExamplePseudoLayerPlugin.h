/**
 *  @file   WorkshopContent/examplecontent/ExamplePlugins/ExamplePseudoLayerPlugin.h
 * 
 *  @brief  Header file for the example pseudo layer plugin class.
 * 
 *  $Log: $
 */
#ifndef EXAMPLE_PSEUDO_LAYER_PLUGIN_H
#define EXAMPLE_PSEUDO_LAYER_PLUGIN_H 1

#include "Plugins/PseudoLayerPlugin.h"

namespace example_content
{

/**
 *  @brief  ExamplePseudoLayerPlugin class
 */
class ExamplePseudoLayerPlugin : public pandora::PseudoLayerPlugin
{
public:
    /**
     *  @brief  Default constructor
     */
    ExamplePseudoLayerPlugin();

private:
    unsigned int GetPseudoLayer(const pandora::CartesianVector &positionVector) const;
    unsigned int GetPseudoLayerAtIp() const;
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    float   m_zPitch;       ///< The z pitch
    float   m_zOffset;      ///< The z offset
};

} // namespace example_content

#endif // #ifndef EXAMPLE_PSEUDO_LAYER_PLUGIN_H
