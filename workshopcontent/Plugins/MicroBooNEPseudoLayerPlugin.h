/**
 *  @file   workshopcontent/Plugins/MicroBooNEPseudoLayerPlugin.h
 *
 *  @brief  Header file for the MicroBooNE pseudo layer plugin class.
 *
 *  $Log: $
 */
#ifndef WORKSHOP_UBOONE_PSEUDO_LAYER_PLUGIN_H
#define WORKSHOP_UBOONE_PSEUDO_LAYER_PLUGIN_H 1

#include "larpandoracontent/LArPlugins/LArPseudoLayerPlugin.h"

namespace workshop_content
{

/**
 *  @brief  MicroBooNEPseudoLayerPlugin class
 * 
 *          The pseudo layer plugin is instantiated and registed (with the Pandora plugin manager) via the client app. It is responsible
 *          for binning world coordinates in layers. In a 4-pi detector with collisions at a central interaction point, layers typically
 *          group together positions of equivalent depth in the detector. For fixed target collisions, the layer structure will typically
 *          represent a binning of distance from the target.
 */
class MicroBooNEPseudoLayerPlugin : public lar_content::LArPseudoLayerPlugin
{
public:
    /**
     *  @brief  Default constructor
     */
    MicroBooNEPseudoLayerPlugin();
};

} // namespace workshop_content

#endif // #ifndef WORKSHOP_UBOONE_PSEUDO_LAYER_PLUGIN_H
