/**
 *  @file   ExamplePandora/test/PandoraLArExample.cc
 * 
 *  @brief  Implementation for the pandora lar example application
 * 
 *  $Log: $
 */

#include "Api/PandoraApi.h"

#include "larexamplecontent/LArExampleContent.h"
#include "larpandoracontent/LArContent.h"

#include "larexamplecontent/ExamplePlugins/MicroBooNEPseudoLayerPlugin.h"
#include "larexamplecontent/ExamplePlugins/MicroBooNETransformationPlugin.h"

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

    /**
     *  @brief  Parse the command line arguments, setting the parameters
     * 
     *  @param  argc argument count
     *  @param  argv argument vector
     * 
     *  @return success
     */
    bool ParseCommandLine(int argc, char *argv[]);

    std::string     m_pandoraSettingsFile;          ///< The path to the pandora settings file (mandatory parameter)
    int             m_nEventsToProcess;             ///< The number of events to process (default all events in file)
    bool            m_shouldDisplayEventNumber;     ///< Whether event numbers should be displayed (default false)
};

//------------------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    try
    {
        // Parse command line parameters
        Parameters parameters;

        if (!ParseCommandLine(argc, argv, parameters))
            return 1;
#ifdef MONITORING
        TApplication *const pTApplication = new TApplication("PandoraLArExample", &argc, argv);
        pTApplication->SetReturnFromRun(kTRUE);
#endif
        // Construct pandora instance
        const pandora::Pandora *const pPandora = new pandora::Pandora();

        // Register content from the example pandora library
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, LArExampleContent::RegisterAlgorithms(*pPandora));
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, LArExampleContent::RegisterPlugins(*pPandora));

        // Register content from the real pandora library
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, LArContent::RegisterAlgorithms(*pPandora));
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, LArContent::RegisterPlugins(*pPandora));

        // Provide a couple of lar-specific plugins
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, LArContent::SetLArPseudoLayerPlugin(*pPrimaryPandora, new lar_example::MicroBooNEPseudoLayerPlugin));
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, LArContent::SetLArTransformationPlugin(*pPrimaryPandora, new lar_example::MicroBooNETransformationPlugin));

        // Read the PandoraSettings
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::ReadSettings(*pPandora, parameters.m_pandoraSettingsFile));

        // Process the events
        int nEvents(0);

        while ((nEvents++ < parameters.m_nEventsToProcess) || (0 > parameters.m_nEventsToProcess))
        {
            PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::ProcessEvent(*pPandora));
            PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::Reset(*pPandora));
        }

        // Tidy up
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
//------------------------------------------------------------------------------------------------------------------------------------------

Parameters::Parameters() :
    m_pandoraSettingsFile(),
    m_nEventsToProcess(-1),
    m_shouldDisplayEventNumber(false)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool Parameters::ParseCommandLine(int argc, char *argv[])
{
    int c(0);

    while ((c = getopt(argc, argv, "i:n::N::h")) != -1)
    {
        switch (c)
        {
        case 'i':
            m_pandoraSettingsFile = optarg;
            break;
        case 'n':
            m_nEventsToProcess = atoi(optarg);
            break;
        case 'N':
            m_shouldDisplayEventNumber = true;
            break;
        case 'h':
        default:
            std::cout << std::endl << "PandoraLArExample " << std::endl
                      << "    -i PandoraSettings.xml  (mandatory)" << std::endl
                      << "    -n NEventsToProcess     (optional)" << std::endl
                      << "    -N                      (optional, display event numbers)" << std::endl << std::endl;
            return false;
        }
    }

    return true;
}
