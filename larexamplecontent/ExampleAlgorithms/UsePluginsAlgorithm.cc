/**
 *  @file   LArExampleContent/larexamplecontent/ExampleAlgorithms/UsePluginsAlgorithm.cc
 * 
 *  @brief  Implementation of the use plugins algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "ExampleAlgorithms/UsePluginsAlgorithm.h"

using namespace pandora;

namespace example_content
{

StatusCode UsePluginsAlgorithm::Run()
{
    // Query clusters in the current list with particle id and energy correction plugins.
    const ClusterList *pClusterList(NULL);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pClusterList));

    // Particle id plugins are instantiated and registed (with the Pandora plugin manager) via the client app. They are then
    // associated with particular particle id "slots" via the PandoraSettings xml file e.g. MuonIdPlugin, ElectronIdPlugin
    // Each plugin can have configurable parameters and must provide an implementation of an IsMatch(const Cluster *const) function.
    for (ClusterList::const_iterator iter = pClusterList->begin(), iterEnd = pClusterList->end(); iter != iterEnd; ++iter)
    {
        const Cluster *const pCluster(*iter);

        if ((MU_MINUS == std::abs(pCluster->GetParticleIdFlag())) || this->GetPandora().GetPlugins()->GetParticleId()->IsMuon(pCluster))
        {
            // Placeholder
        }
    }

    // Energy correction plugins are instantiated and registed (with the Pandora plugin manager) via the client app. They are then
    // associated with electromagnetic or hadronic energy estimators via the PandoraSettings xml file. The xml file specifies
    // which plugins to call and in which order, so multiple corrections are possible. Each plugin can have configurable parameters
    // and must provide an implementation of a MakeEnergyCorrections(const Cluster *const, float &) function.
    for (ClusterList::const_iterator iter = pClusterList->begin(), iterEnd = pClusterList->end(); iter != iterEnd; ++iter)
    {
        const Cluster *const pCluster(*iter);

        const float correctedElectomagneticEnergyMethod1(pCluster->GetCorrectedElectromagneticEnergy(this->GetPandora()));
        const float correctedHadronicEnergyMethod1(pCluster->GetCorrectedHadronicEnergy(this->GetPandora()));

        float correctedElectomagneticEnergyMethod2(0.f), correctedHadronicEnergyMethod2(0.f);
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, this->GetPandora().GetPlugins()->GetEnergyCorrections()->MakeEnergyCorrections(pCluster,
            correctedElectomagneticEnergyMethod2, correctedHadronicEnergyMethod2));

        if ((correctedElectomagneticEnergyMethod1 > std::numeric_limits<float>::epsilon()) ||
            (correctedElectomagneticEnergyMethod2 > std::numeric_limits<float>::epsilon()) ||
            (correctedHadronicEnergyMethod1 > std::numeric_limits<float>::epsilon()) ||
            (correctedHadronicEnergyMethod2 > std::numeric_limits<float>::epsilon()) )
        {
            // Placeholder
        }
    }

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode UsePluginsAlgorithm::ReadSettings(const TiXmlHandle /*xmlHandle*/)
{
    // Read settings from xml file here

    return STATUS_CODE_SUCCESS;
}

} // namespace example_content
