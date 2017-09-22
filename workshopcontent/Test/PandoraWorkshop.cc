/**
 *  @file   WorkshopContent/workshopcontent/Test/PandoraWorkshop.cc
 * 
 *  @brief  Implementation for the pandora workshop application
 * 
 *  $Log: $
 */

#include "Api/PandoraApi.h"

#include "larpandoracontent/LArContent.h"
#include "larpandoracontent/LArPlugins/LArPseudoLayerPlugin.h"
#include "larpandoracontent/LArPlugins/LArRotationalTransformationPlugin.h"

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

/**
 *  @brief  Print the list of configurable options
 *
 *  @return false, to force abort
 */
bool PrintOptions();

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
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::SetPseudoLayerPlugin(*pPandora, new lar_content::LArPseudoLayerPlugin));
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::SetLArTransformationPlugin(*pPandora, new lar_content::LArRotationalTransformationPlugin));

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
    if (1 == argc)
        return PrintOptions();

    int c(0);

    while ((c = getopt(argc, argv, "i:n:Nh")) != -1)
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
        case 'h':
        default:
            return PrintOptions();
        }
    }

    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool PrintOptions()
{
    std::cout << std::endl << "PandoraWorkshop " << std::endl
              << "    -i PandoraSettings.xml (required)" << std::endl
              << "    -n NEventsToProcess    (optional)" << std::endl
              << "    -N                     (optional, display event numbers)" << std::endl << std::endl;

     return false;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

Parameters::Parameters() :
    m_pandoraSettingsFile(),
    m_nEventsToProcess(-1),
    m_shouldDisplayEventNumber(false)
{
}
