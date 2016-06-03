/**
 *  @file   WorkshopContent/workshopcontent/Plugins/MicroBooNETransformationPlugin.h
 *
 *  @brief  Header file for the MicroBooNE transformation plugin class.
 *
 *  $Log: $
 */
#ifndef WORKSHOP_UBOONE_TRANSFORMATION_PLUGIN_H
#define WORKSHOP_UBOONE_TRANSFORMATION_PLUGIN_H 1

#include "larpandoracontent/LArPlugins/LArRotationalTransformationPlugin.h"

namespace workshop_content
{

/**
 *  @brief  MicroBooNETransformationPlugin class
 */
class MicroBooNETransformationPlugin : public lar_content::LArRotationalTransformationPlugin
{
public:
    /**
     *  @brief  Default constructor
     */
    MicroBooNETransformationPlugin();
};

} // namespace workshop_content

#endif // #ifndef WORKSHOP_UBOONE_TRANSFORMATION_PLUGIN_H
