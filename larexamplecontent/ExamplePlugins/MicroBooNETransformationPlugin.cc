/**
 *  @file   larexamplecontent/ExamplePlugins/MicroBooNETransformationPlugin.cc
 *
 *  @brief  Implementation of the MicroBooNE transformation plugin class.
 *
 *  $Log: $
 */

#include "larexamplecontent/ExamplePlugins/MicroBooNETransformationPlugin.h"

#include <cmath>

namespace lar_example_content
{

MicroBooNETransformationPlugin::MicroBooNETransformationPlugin() :
    lar_content::LArRotationalTransformationPlugin(M_PI / 3., M_PI / 3., 1.)
{
}

} // namespace lar_example_content
