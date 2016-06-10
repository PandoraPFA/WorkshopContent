/**
 *  @file   WorkshopContent/examplecontent/Test/PandoraExample.cc
 * 
 *  @brief  Implementation for the pandora example application
 * 
 *  $Log: $
 */

#include "Api/PandoraApi.h"

#include "examplecontent/ExampleContent.h"

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
    int             m_nHitGroupings;                ///< The number of hit groupings to generate for test purposes
    int             m_nHitsPerGroup;                ///< The number of hits per group to generate for test purposes
    float           m_worldSideLength;              ///< The world volume cube side length
    float           m_groupSideLength;              ///< The group volume cube side length
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
 *  @brief  Generate a specified number of example pandora hits, randomly positioned in groups in a world volume cube of defined length
 * 
 *  @param  pandora the relevant pandora instance
 *  @param  parameters the application parameters
 * 
 *  @return success
 */
pandora::StatusCode GenerateExampleHits(const pandora::Pandora &pandora, const Parameters &parameters);

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
        TApplication *const pTApplication = new TApplication("MyTest", &argc, argv);
        pTApplication->SetReturnFromRun(kTRUE);
#endif
        // Construct pandora instance
        const pandora::Pandora *const pPandora = new pandora::Pandora();

        // Register content from external pandora libraries
        PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, ExampleContent::RegisterExampleAlgorithms(*pPandora));
        PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, ExampleContent::RegisterExamplePlugins(*pPandora));

        // Read in the PandoraSettings
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::ReadSettings(*pPandora, parameters.m_pandoraSettingsFile));

        // Process the events
        int nEvents(0);
        std::srand(12345);

        while ((nEvents++ < parameters.m_nEventsToProcess) || (0 > parameters.m_nEventsToProcess))
        {
            PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, GenerateExampleHits(*pPandora, parameters));
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

    while ((c = getopt(argc, argv, "i:n:g:p:w:s:?")) != -1)
    {
        switch (c)
        {
        case 'i':
            parameters.m_pandoraSettingsFile = optarg;
            break;
        case 'n':
            parameters.m_nEventsToProcess = atoi(optarg);
            break;
        case 'g':
            parameters.m_nHitGroupings = std::min(100, atoi(optarg));
            break;
        case 'p':
            parameters.m_nHitsPerGroup = std::min(1000, atoi(optarg));
            break;
        case 'w':
            parameters.m_worldSideLength = atof(optarg);
            break;
        case 's':
            parameters.m_groupSideLength = atof(optarg);
            break;
        case '?':
        default:
            std::cout << std::endl << "PandoraExample " << std::endl
                      << "    -i PandoraSettings.xml  (mandatory)" << std::endl
                      << "    -n NEventsToProcess     (optional)" << std::endl
                      << "    -g NHitGroupings        (optional)" << std::endl
                      << "    -p NHitsPerGroup        (optional)" << std::endl
                      << "    -w WorldSideLength      (optional, may require algorithm retuning)" << std::endl
                      << "    -s GroupSideLength      (optional, may require algorithm retuning)" << std::endl << std::endl;
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------

pandora::StatusCode GenerateExampleHits(const pandora::Pandora &pandora, const Parameters &inputParameters)
{
    for (int iGroup = 0; iGroup < inputParameters.m_nHitGroupings; ++iGroup)
    {
        const pandora::CartesianVector groupCentre(
            ((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) - 0.5f) * inputParameters.m_worldSideLength,
            ((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) - 0.5f) * inputParameters.m_worldSideLength,
            ((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) - 0.5f) * inputParameters.m_worldSideLength);

        for (int iHit = 0; iHit < inputParameters.m_nHitsPerGroup; ++iHit)
        {
            const pandora::CartesianVector localPosition(
                ((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) - 0.5f) * inputParameters.m_groupSideLength,
                ((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) - 0.5f) * inputParameters.m_groupSideLength,
                ((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) - 0.5f) * inputParameters.m_groupSideLength);

            // Mainly dummy parameters
            PandoraApi::CaloHit::Parameters parameters;
            parameters.m_positionVector = localPosition + groupCentre;
            parameters.m_expectedDirection = pandora::CartesianVector(0.f, 0.f, 1.f);
            parameters.m_cellNormalVector = pandora::CartesianVector(0.f, 0.f, 1.f);
            parameters.m_cellGeometry = pandora::RECTANGULAR;
            parameters.m_cellSize0 = 1.f;
            parameters.m_cellSize1 = 1.f;
            parameters.m_cellThickness = 1.f;
            parameters.m_nCellRadiationLengths = 1.f;
            parameters.m_nCellInteractionLengths = 1.f;
            parameters.m_time = 0.f;
            parameters.m_inputEnergy = 1.f;
            parameters.m_mipEquivalentEnergy = 1.f;
            parameters.m_electromagneticEnergy = 1.f;
            parameters.m_hadronicEnergy = 1.f;
            parameters.m_isDigital = false;
            parameters.m_hitType = pandora::HIT_CUSTOM;
            parameters.m_hitRegion = pandora::SINGLE_REGION;
            parameters.m_layer = 0;
            parameters.m_isInOuterSamplingLayer = false;
            parameters.m_pParentAddress = (void*)(static_cast<uintptr_t>(iHit));

            PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::CaloHit::Create(pandora, parameters));
        }
    }

    return pandora::STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

Parameters::Parameters() :
    m_pandoraSettingsFile(),
    m_nEventsToProcess(-1),
    m_nHitGroupings(10),
    m_nHitsPerGroup(100),
    m_worldSideLength(500.f),
    m_groupSideLength(10.f)
{
}
