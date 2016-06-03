/**
 *  @file   WorkshopContent/examplecontent/ExampleContent.h
 * 
 *  @brief  Header file detailing example pandora content
 * 
 *  $Log: $
 */
#ifndef EXAMPLE_CONTENT_H
#define EXAMPLE_CONTENT_H 1

#include "examplecontent/ExampleAlgorithms/AccessListsAlgorithm.h"
#include "examplecontent/ExampleAlgorithms/CreateAdditionalCaloHitsAlgorithm.h"
#include "examplecontent/ExampleAlgorithms/CreateClustersAlgorithm.h"
#include "examplecontent/ExampleAlgorithms/CreateClustersDaughterAlgorithm.h"
#include "examplecontent/ExampleAlgorithms/CreatePfosAlgorithm.h"
#include "examplecontent/ExampleAlgorithms/CreateVerticesAlgorithm.h"
#include "examplecontent/ExampleAlgorithms/DeleteClustersAlgorithm.h"
#include "examplecontent/ExampleAlgorithms/DisplayListsAlgorithm.h"
#include "examplecontent/ExampleAlgorithms/FragmentClustersAlgorithm.h"
#include "examplecontent/ExampleAlgorithms/MergeClustersAlgorithm.h"
#include "examplecontent/ExampleAlgorithms/ReconfigureClustersAlgorithm.h"
#include "examplecontent/ExampleAlgorithms/SelectHitSubsetAlgorithm.h"
#include "examplecontent/ExampleAlgorithms/UseAlgorithmToolAlgorithm.h"
#include "examplecontent/ExampleAlgorithms/UsePluginsAlgorithm.h"
#include "examplecontent/ExampleAlgorithms/WriteTreeAlgorithm.h"

#include "examplecontent/ExampleAlgorithmTools/ExampleAlgorithmTool.h"

#include "examplecontent/ExamplePlugins/ExampleEnergyCorrectionPlugin.h"
#include "examplecontent/ExamplePlugins/ExampleParticleIdPlugin.h"
#include "examplecontent/ExamplePlugins/ExamplePseudoLayerPlugin.h"

/**
 *  @brief  ExampleContent class
 */
class ExampleContent
{
public:
    #define EXAMPLE_ALGORITHM_LIST(d)                                                                                   \
        d("AccessListsExample",                         example_content::AccessListsAlgorithm::Factory)                 \
        d("CreateAdditionalCaloHitsExample",            example_content::CreateAdditionalCaloHitsAlgorithm::Factory)    \
        d("CreateClustersExample",                      example_content::CreateClustersAlgorithm::Factory)              \
        d("CreateClustersDaughterExample",              example_content::CreateClustersDaughterAlgorithm::Factory)      \
        d("CreatePfosExample",                          example_content::CreatePfosAlgorithm::Factory)                  \
        d("CreateVerticesExample",                      example_content::CreateVerticesAlgorithm::Factory)              \
        d("DeleteClustersExample",                      example_content::DeleteClustersAlgorithm::Factory)              \
        d("DisplayListsExample",                        example_content::DisplayListsAlgorithm::Factory)                \
        d("FragmentClustersExample",                    example_content::FragmentClustersAlgorithm::Factory)            \
        d("MergeClustersExample",                       example_content::MergeClustersAlgorithm::Factory)               \
        d("ReconfigureClustersExample",                 example_content::ReconfigureClustersAlgorithm::Factory)         \
        d("SelectHitSubsetExample",                     example_content::SelectHitSubsetAlgorithm::Factory)             \
        d("UseAlgorithmToolExample",                    example_content::UseAlgorithmToolAlgorithm::Factory)            \
        d("UsePluginsExample",                          example_content::UsePluginsAlgorithm::Factory)                  \
        d("WriteTreeExample",                           example_content::WriteTreeAlgorithm::Factory)

    #define EXAMPLE_ALGORITHM_TOOL_LIST(d)                                                                              \
        d("AlgorithmToolExample",                       example_content::ExampleAlgorithmTool::Factory)

    #define EXAMPLE_ENERGY_CORRECTION_LIST(d)                                                                           \
        d("EnergyCorrectionExample", pandora::HADRONIC, example_content::ExampleEnergyCorrectionPlugin)

    #define EXAMPLE_PARTICLE_ID_LIST(d)                                                                                 \
        d("ParticleIdExample",                          example_content::ExampleParticleIdPlugin)

    /**
     *  @brief  Register all the example content algorithms and tools with pandora
     * 
     *  @param  pandora the pandora instance with which to register content
     */
    static pandora::StatusCode RegisterExampleAlgorithms(const pandora::Pandora &pandora);

    /**
     *  @brief  Register the example content plugins with pandora
     * 
     *  @param  pandora the pandora instance with which to register content
     */
    static pandora::StatusCode RegisterExamplePlugins(const pandora::Pandora &pandora);
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::StatusCode ExampleContent::RegisterExampleAlgorithms(const pandora::Pandora &pandora)
{
    EXAMPLE_ALGORITHM_LIST(PANDORA_REGISTER_ALGORITHM);
    EXAMPLE_ALGORITHM_TOOL_LIST(PANDORA_REGISTER_ALGORITHM_TOOL);

    return pandora::STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::StatusCode ExampleContent::RegisterExamplePlugins(const pandora::Pandora &pandora)
{
    EXAMPLE_ENERGY_CORRECTION_LIST(PANDORA_REGISTER_ENERGY_CORRECTION);
    EXAMPLE_PARTICLE_ID_LIST(PANDORA_REGISTER_PARTICLE_ID);
    PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::SetPseudoLayerPlugin(pandora, new example_content::ExamplePseudoLayerPlugin));

    return pandora::STATUS_CODE_SUCCESS;
}

#endif // #ifndef EXAMPLE_CONTENT_H
