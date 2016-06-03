/**
 *  @file   WorkshopContent/workshopcontent/Plugins/MicroBooNETransformationPlugin.cc
 *
 *  @brief  Implementation of the MicroBooNE transformation plugin class.
 *
 *  $Log: $
 */

#include "workshopcontent/Plugins/MicroBooNETransformationPlugin.h"

#include <cmath>

namespace workshop_content
{

MicroBooNETransformationPlugin::MicroBooNETransformationPlugin() :
    lar_content::LArRotationalTransformationPlugin(M_PI / 3., M_PI / 3., 1.)
{
}

} // namespace workshop_content
