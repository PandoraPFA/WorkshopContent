/**
 *  @file   LArExampleContent/larexamplecontent/ExamplePlugins/ExampleEnergyCorrectionPlugin.cc
 * 
 *  @brief  Implementation of the example energy correction plugin class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "larexamplecontent/ExamplePlugins/ExampleEnergyCorrectionPlugin.h"

using namespace pandora;

namespace lar_example_content
{

ExampleEnergyCorrectionPlugin::ExampleEnergyCorrectionPlugin() :
    m_energyMultiplier(1.f)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode ExampleEnergyCorrectionPlugin::MakeEnergyCorrections(const Cluster *const /*pCluster*/, float &correctedHadronicEnergy) const
{
    // Energy correction plugins are instantiated and registed (with the Pandora plugin manager) via the client app. They are then
    // associated with electromagnetic or hadronic energy estimators via the PandoraSettings xml file. The xml file specifies
    // which plugins to call and in which order, so multiple corrections are possible. Each plugin can have configurable parameters
    // and must provide an implementation of a MakeEnergyCorrections(const Cluster *const, float &) function.

    correctedHadronicEnergy *= m_energyMultiplier;

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode ExampleEnergyCorrectionPlugin::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "EnergyMultiplier", m_energyMultiplier));

    return STATUS_CODE_SUCCESS;
}

} // namespace lar_example_content
