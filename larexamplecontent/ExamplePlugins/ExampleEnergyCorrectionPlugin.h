/**
 *  @file   LArExampleContent/larexamplecontent/ExamplePlugins/ExampleEnergyCorrectionPlugin.h
 * 
 *  @brief  Header file for the example energy correction plugin class.
 * 
 *  $Log: $
 */
#ifndef EXAMPLE_ENERGY_CORRECTION_PLUGIN_H
#define EXAMPLE_ENERGY_CORRECTION_PLUGIN_H 1

#include "Plugins/EnergyCorrectionsPlugin.h"

namespace example_content
{

/**
 *   @brief  ExampleEnergyCorrectionPlugin class.
 */
class ExampleEnergyCorrectionPlugin : public pandora::EnergyCorrectionPlugin
{
public:
    /**
     *  @brief  Default constructor
     */
    ExampleEnergyCorrectionPlugin();

    pandora::StatusCode MakeEnergyCorrections(const pandora::Cluster *const pCluster, float &correctedEnergy) const;

private:
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    float   m_energyMultiplier;     ///< Simple energy multiplier demonstration
};

} // namespace example_content

#endif // #ifndef EXAMPLE_ENERGY_CORRECTION_PLUGIN_H
