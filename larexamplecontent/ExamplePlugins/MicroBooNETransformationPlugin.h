/**
 *  @file   larexamplecontent/ExamplePlugins/MicroBooNETransformationPlugin.h
 *
 *  @brief  Header file for the MicroBooNE transformation plugin class.
 *
 *  $Log: $
 */
#ifndef EXAMPLE_UBOONE_TRANSFORMATION_PLUGIN_H
#define EXAMPLE_UBOONE_TRANSFORMATION_PLUGIN_H 1

#include "larpandoracontent/LArPlugins/LArRotationalTransformationPlugin.h"

namespace lar_example
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

} // namespace lar_example

#endif // #ifndef EXAMPLE_UBOONE_TRANSFORMATION_PLUGIN_H
