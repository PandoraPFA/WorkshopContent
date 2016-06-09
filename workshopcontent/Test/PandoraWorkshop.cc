/**
 *  @file   WorkshopContent/workshopcontent/Test/PandoraWorkshop.cc
 * 
 *  @brief  Implementation for the pandora workshop application
 * 
 *  $Log: $
 */

#include "Api/PandoraApi.h"

#include "workshopcontent/Algorithms/MyParticleMergingAlgorithm.h"
#include "workshopcontent/Algorithms/MyParticleCreationAlgorithm.h"
#include "workshopcontent/Algorithms/MyClusterMergingAlgorithm.h"
#include "workshopcontent/Algorithms/MyTestAlgorithm.h"
#include "workshopcontent/Plugins/MicroBooNEPseudoLayerPlugin.h"
#include "workshopcontent/Plugins/MicroBooNETransformationPlugin.h"

#include "larpandoracontent/LArContent.h"

#ifdef MONITORING
#include "TApplication.h"
#endif

#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>

/**
 *  @brief  Parameters class
 */
class Parameters
{
public:
    /**
     *  @brief Default constructor
     */
    Parameters();

    std::string     m_pandoraSettingsFile;          ///< The path to the pandora settings file (mandatory parameter)
    int             m_nEventsToProcess;             ///< The number of events to process (default all events in file)
    bool            m_shouldDisplayEventNumber;     ///< Whether event numbers should be displayed (default false)
};

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 *  @brief  Parse the command line arguments, setting the application parameters
 * 
 *  @param  argc argument count
 *  @param  argv argument vector
 *  @param  parameters to receive the application parameters
 * 
 *  @return success
 */
bool ParseCommandLine(int argc, char *argv[], Parameters &parameters);

//------------------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    try
    {
        Parameters parameters;

        if (!ParseCommandLine(argc, argv, parameters))
            return 1;

#ifdef MONITORING
        TApplication *const pTApplication = new TApplication("Workshop", &argc, argv);
        pTApplication->SetReturnFromRun(kTRUE);
#endif
        const pandora::Pandora *const pPandora = new pandora::Pandora();

        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, LArContent::RegisterAlgorithms(*pPandora));
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, LArContent::RegisterBasicPlugins(*pPandora));
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, LArContent::SetLArPseudoLayerPlugin(*pPandora, new workshop_content::MicroBooNEPseudoLayerPlugin));
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, LArContent::SetLArTransformationPlugin(*pPandora, new workshop_content::MicroBooNETransformationPlugin));

        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::RegisterAlgorithmFactory(*pPandora,
            "MyParticleMerging", new workshop_content::MyParticleMergingAlgorithm::Factory));
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::RegisterAlgorithmFactory(*pPandora,
             "MyParticleCreation", new workshop_content::MyParticleCreationAlgorithm::Factory));
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::RegisterAlgorithmFactory(*pPandora,
            "MyClusterMerging", new workshop_content::MyClusterMergingAlgorithm::Factory));
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::RegisterAlgorithmFactory(*pPandora,
            "MyTest", new workshop_content::MyTestAlgorithm::Factory));

        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::ReadSettings(*pPandora, parameters.m_pandoraSettingsFile));

        int nEvents(0);
        while ((nEvents++ < parameters.m_nEventsToProcess) || (0 > parameters.m_nEventsToProcess))
        {
            if (parameters.m_shouldDisplayEventNumber)
                std::cout << std::endl << "   PROCESSING EVENT: " << (nEvents - 1) << std::endl << std::endl;

            PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::ProcessEvent(*pPandora));
            PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::Reset(*pPandora));
        }

        delete pPandora;
    }
    catch (pandora::StatusCodeException &statusCodeException)
    {
        std::cerr << "Pandora Exception caught: " << statusCodeException.ToString() << std::endl;
        return 1;
    }

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool ParseCommandLine(int argc, char *argv[], Parameters &parameters)
{
    int c(0);

    while ((c = getopt(argc, argv, "i:n:N?")) != -1)
    {
        switch (c)
        {
        case 'i':
            parameters.m_pandoraSettingsFile = optarg;
            break;
        case 'n':
            parameters.m_nEventsToProcess = atoi(optarg);
            break;
        case 'N':
            parameters.m_shouldDisplayEventNumber = true;
            break;
        case '?':
        default:
            std::cout << std::endl << "PandoraWorkshop " << std::endl
                      << "    -i PandoraSettings.xml (mandatory)" << std::endl
                      << "    -n NEventsToProcess    (mandatory)" << std::endl
                      << "    -N                     (optional, display event numbers)" << std::endl;
             return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

Parameters::Parameters() :
    m_pandoraSettingsFile(),
    m_nEventsToProcess(-1),
    m_shouldDisplayEventNumber(false)
{
}
