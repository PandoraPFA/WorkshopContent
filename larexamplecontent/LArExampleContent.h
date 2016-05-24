/**
 *  @file   LArExampleContent/larexamplecontent/ExampleContent.h
 * 
 *  @brief  Header file detailing example pandora content
 * 
 *  $Log: $
 */
#ifndef EXAMPLE_CONTENT_H
#define EXAMPLE_CONTENT_H 1

#include "larexamplecontent/ExampleAlgorithms/AccessListsAlgorithm.h"
#include "larexamplecontent/ExampleAlgorithms/CreateAdditionalCaloHitsAlgorithm.h"
#include "larexamplecontent/ExampleAlgorithms/CreateClustersAlgorithm.h"
#include "larexamplecontent/ExampleAlgorithms/CreateClustersDaughterAlgorithm.h"
#include "larexamplecontent/ExampleAlgorithms/CreatePfosAlgorithm.h"
#include "larexamplecontent/ExampleAlgorithms/CreateVerticesAlgorithm.h"
#include "larexamplecontent/ExampleAlgorithms/DeleteClustersAlgorithm.h"
#include "larexamplecontent/ExampleAlgorithms/DisplayListsAlgorithm.h"
#include "larexamplecontent/ExampleAlgorithms/FragmentClustersAlgorithm.h"
#include "larexamplecontent/ExampleAlgorithms/MergeClustersAlgorithm.h"
#include "larexamplecontent/ExampleAlgorithms/ReconfigureClustersAlgorithm.h"
#include "larexamplecontent/ExampleAlgorithms/SelectHitSubsetAlgorithm.h"
#include "larexamplecontent/ExampleAlgorithms/UseAlgorithmToolAlgorithm.h"
#include "larexamplecontent/ExampleAlgorithms/UsePluginsAlgorithm.h"
#include "larexamplecontent/ExampleAlgorithms/WriteTreeAlgorithm.h"

#include "larexamplecontent/ExampleAlgorithmTools/ExampleAlgorithmTool.h"

#include "larexamplecontent/ExamplePlugins/ExampleEnergyCorrectionPlugin.h"
#include "larexamplecontent/ExamplePlugins/ExampleParticleIdPlugin.h"

/**
 *  @brief  LArExampleContent class
 */
class LArExampleContent
{
public:
    #define LAR_EXAMPLE_ALGORITHM_LIST(d)                                                                                       \
        d("LArAccessListsExample",                          lar_example_content::AccessListsAlgorithm::Factory)                 \
        d("LArCreateAdditionalCaloHitsExample",             lar_example_content::CreateAdditionalCaloHitsAlgorithm::Factory)    \
        d("LArCreateClustersExample",                       lar_example_content::CreateClustersAlgorithm::Factory)              \
        d("LArCreateClustersDaughterExample",               lar_example_content::CreateClustersDaughterAlgorithm::Factory)      \
        d("LArCreatePfosExample",                           lar_example_content::CreatePfosAlgorithm::Factory)                  \
        d("LArCreateVerticesExample",                       lar_example_content::CreateVerticesAlgorithm::Factory)              \
        d("LArDeleteClustersExample",                       lar_example_content::DeleteClustersAlgorithm::Factory)              \
        d("LArDisplayListsExample",                         lar_example_content::DisplayListsAlgorithm::Factory)                \
        d("LArFragmentClustersExample",                     lar_example_content::FragmentClustersAlgorithm::Factory)            \
        d("LArMergeClustersExample",                        lar_example_content::MergeClustersAlgorithm::Factory)               \
        d("LArReconfigureClustersExample",                  lar_example_content::ReconfigureClustersAlgorithm::Factory)         \
        d("LArSelectHitSubsetExample",                      lar_example_content::SelectHitSubsetAlgorithm::Factory)             \
        d("LArUseAlgorithmToolExample",                     lar_example_content::UseAlgorithmToolAlgorithm::Factory)            \
        d("LArUsePluginsExample",                           lar_example_content::UsePluginsAlgorithm::Factory)                  \
        d("LArWriteTreeExample",                            lar_example_content::WriteTreeAlgorithm::Factory)

    #define LAR_EXAMPLE_ALGORITHM_TOOL_LIST(d)                                                                                  \
        d("LArAlgorithmToolExample",                        lar_example_content::ExampleAlgorithmTool::Factory)

    #define LAR_EXAMPLE_ENERGY_CORRECTION_LIST(d)                                                                               \
        d("LArEnergyCorrectionExample", pandora::HADRONIC,  lar_example_content::ExampleEnergyCorrectionPlugin)

    #define LAR_EXAMPLE_PARTICLE_ID_LIST(d)                                                                                     \
        d("LArParticleIdExample",                           lar_example_content::ExampleParticleIdPlugin)

    /**
     *  @brief  Register all the example content algorithms and tools with pandora
     * 
     *  @param  pandora the pandora instance with which to register content
     */
    static pandora::StatusCode RegisterAlgorithms(const pandora::Pandora &pandora);

    /**
     *  @brief  Register the example content plugins with pandora
     * 
     *  @param  pandora the pandora instance with which to register content
     */
    static pandora::StatusCode RegisterBasicPlugins(const pandora::Pandora &pandora);
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::StatusCode LArExampleContent::RegisterAlgorithms(const pandora::Pandora &pandora)
{
    LAR_EXAMPLE_ALGORITHM_LIST(PANDORA_REGISTER_ALGORITHM);
    LAR_EXAMPLE_ALGORITHM_TOOL_LIST(PANDORA_REGISTER_ALGORITHM_TOOL);

    return pandora::STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::StatusCode LArExampleContent::RegisterBasicPlugins(const pandora::Pandora &pandora)
{
    LAR_EXAMPLE_ENERGY_CORRECTION_LIST(PANDORA_REGISTER_ENERGY_CORRECTION);
    LAR_EXAMPLE_PARTICLE_ID_LIST(PANDORA_REGISTER_PARTICLE_ID);

    return pandora::STATUS_CODE_SUCCESS;
}

#endif // #ifndef LAR_CONTENT_H
