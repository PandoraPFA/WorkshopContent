/**
 *  @file   LArExampleContent/larexamplecontent/ExamplePlugins/ExampleParticleIdPlugin.cc
 * 
 *  @brief  Implementation of the example particle id plugin class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "larexamplecontent/ExamplePlugins/ExampleParticleIdPlugin.h"

using namespace pandora;

namespace lar_example_content
{

ExampleParticleIdPlugin::ExampleParticleIdPlugin() :
    m_exampleParameter(0)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool ExampleParticleIdPlugin::IsMatch(const Cluster *const /*pCluster*/) const
{
    // Particle id plugins are instantiated and registed (with the Pandora plugin manager) via the client app. They are then
    // associated with particular particle id "slots" via the PandoraSettings xml file e.g. MuonIdPlugin, ElectronIdPlugin
    // Each plugin can have configurable parameters and must provide an implementation of an IsMatch(const Cluster *const) function.

    return false;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode ExampleParticleIdPlugin::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "ExampleParameter", m_exampleParameter));

    return STATUS_CODE_SUCCESS;
}

} // namespace lar_example_content
