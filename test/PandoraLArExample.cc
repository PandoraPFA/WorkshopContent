/**
 *  @file   ExamplePandora/test/PandoraLArExample.cc
 * 
 *  @brief  Implementation for the pandora lar example application
 * 
 *  $Log: $
 */

#include "Api/PandoraApi.h"

#include "larexamplecontent/LArExampleContent.h"
#include "larexamplecontent/ExamplePlugins/MicroBooNEPseudoLayerPlugin.h"
#include "larexamplecontent/ExamplePlugins/MicroBooNETransformationPlugin.h"

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

    /**
     *  @brief  Parse the command line arguments, setting the parameters
     * 
     *  @param  argc argument count
     *  @param  argv argument vector
     * 
     *  @return success (boolean)
     */
    bool ParseCommandLine(int argc, char *argv[]);

    std::string     m_pandoraSettingsFile;          ///< The path to the pandora settings file (mandatory parameter)
    int             m_nEventsToProcess;             ///< The number of events to process (default all events in file)
    bool            m_shouldDisplayEventNumber;     ///< Whether event numbers should be displayed (default false)
    bool            m_shouldCreateExampleHits;      ///< Whether to create a set of dummy/example hits as input

    int             m_nExampleHitGroupings;         ///< The number of hit groupings to generate for test purposes
    int             m_nExampleHitsPerGroup;         ///< The number of hits per group to generate for test purposes
    float           m_exampleGroupSideLength;       ///< The group volume cube side length
    float           m_exampleWorldSideLength;       ///< The world volume cube side length
};

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 *  @brief  Create a number of example pandora hits, randomly positioned in groups in a world volume cube of defined length
 * 
 *  @param  pandora the relevant pandora instance
 *  @param  parameters the application parameters
 * 
 *  @return success (status code)
 */
pandora::StatusCode CreateExampleHits(const pandora::Pandora &pandora, const Parameters &parameters);

//------------------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    try
    {
        // Parse command line parameters
        Parameters parameters;

        if (!parameters.ParseCommandLine(argc, argv))
            return 1;

#ifdef MONITORING
        TApplication *const pTApplication = new TApplication("MyTest", &argc, argv);
        pTApplication->SetReturnFromRun(kTRUE);
#endif

        // Construct pandora instance
        const pandora::Pandora *const pPandora = new pandora::Pandora();

        // Register content from the example pandora library
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, LArExampleContent::RegisterAlgorithms(*pPandora));
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, LArExampleContent::RegisterBasicPlugins(*pPandora));

        // Register content from the real pandora library
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, LArContent::RegisterAlgorithms(*pPandora));
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, LArContent::RegisterBasicPlugins(*pPandora));

        // Provide a couple of lar-specific plugins
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, LArContent::SetLArPseudoLayerPlugin(*pPandora, new lar_example_content::MicroBooNEPseudoLayerPlugin));
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, LArContent::SetLArTransformationPlugin(*pPandora, new lar_example_content::MicroBooNETransformationPlugin));

        // Read the PandoraSettings
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::ReadSettings(*pPandora, parameters.m_pandoraSettingsFile));

        // Process the events
        int nEvents(0);

        while ((nEvents++ < parameters.m_nEventsToProcess) || (0 > parameters.m_nEventsToProcess))
        {
            if (parameters.m_shouldDisplayEventNumber)
                std::cout << std::endl << "   PROCESSING EVENT: " << (nEvents - 1) << std::endl << std::endl;

            if (parameters.m_shouldCreateExampleHits)
                PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, CreateExampleHits(*pPandora, parameters));

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

pandora::StatusCode CreateExampleHits(const pandora::Pandora &pandora, const Parameters &inputParameters)
{
    for (int iGroup = 0; iGroup < inputParameters.m_nExampleHitGroupings; ++iGroup)
    {
        const pandora::CartesianVector groupCentre(
            ((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) - 0.5f) * inputParameters.m_exampleWorldSideLength,
            ((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) - 0.5f) * inputParameters.m_exampleWorldSideLength,
            ((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) - 0.5f) * inputParameters.m_exampleWorldSideLength);

        for (int iHit = 0; iHit < inputParameters.m_nExampleHitsPerGroup; ++iHit)
        {
            const pandora::CartesianVector localPosition(
                ((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) - 0.5f) * inputParameters.m_exampleGroupSideLength,
                ((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) - 0.5f) * inputParameters.m_exampleGroupSideLength,
                ((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) - 0.5f) * inputParameters.m_exampleGroupSideLength);

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
    m_shouldDisplayEventNumber(false),
    m_shouldCreateExampleHits(false),
    m_nExampleHitGroupings(10),
    m_nExampleHitsPerGroup(50),
    m_exampleGroupSideLength(10.f),
    m_exampleWorldSideLength(400.f)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool Parameters::ParseCommandLine(int argc, char *argv[])
{
    int c(0);

    while ((c = getopt(argc, argv, "i:n:N::H::gpws")) != -1)
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
        case 'H':
            m_shouldCreateExampleHits = true;
            break;
        case 'g':
            m_nExampleHitGroupings = std::min(100, atoi(optarg));
            break;
        case 'p':
            m_nExampleHitsPerGroup = std::min(1000, atoi(optarg));
            break;
        case 's':
            m_exampleGroupSideLength = atof(optarg);
        case 'w':
            m_exampleWorldSideLength = atof(optarg);
            break;
        case '?':
        default:
            std::cout << std::endl << "PandoraLArExample " << std::endl
                      << "    -i PandoraSettings.xml (mandatory)" << std::endl
                      << "    -n NEventsToProcess    (mandatory)" << std::endl
                      << "    -N                     (optional, display event numbers)" << std::endl
                      << "    -H                     (optional, whether to generate example hits)" << std::endl
                      << "    -g NHitGroupings       (optional, example hit parameter)" << std::endl
                      << "    -p NHitsPerGroup       (optional, example hit parameter)" << std::endl
                      << "    -s GroupSideLength     (optional, example hit parameter)" << std::endl
                      << "    -w WorldSideLength     (optional, example hit parameter)" << std::endl << std::endl;
             return false;
        }
    }

    return true;
}
