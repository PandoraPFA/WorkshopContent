/**
 *  @file   LArExampleContent/larexamplecontent/ExampleAlgorithms/CreatePfosAlgorithm.cc
 * 
 *  @brief  Implementation of the create pfos algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "ExampleAlgorithms/CreatePfosAlgorithm.h"

#include "ExampleHelpers/ExampleHelper.h"

using namespace pandora;

namespace example_content
{

StatusCode CreatePfosAlgorithm::Run()
{
    // Create clusters using clusters and vertices in the current list as the building blocks.
    const ClusterList *pClusterList(NULL);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pClusterList));

    const VertexList *pVertexList(NULL);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pVertexList));

    // Algorithms must either create a temporary list for newly created pfos. Any pfos remaining in a temporary list at the end
    // of the algorithm will be deleted, so all desired pfos must be saved before the algorithm ends.
    const PfoList *pTemporaryList(NULL);
    std::string temporaryListName;
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::CreateTemporaryListAndSetCurrent(*this, pTemporaryList, temporaryListName));

    // Here we simply create one pfo per cluster in the current list, adding also the closest available vertex in the current list.
    for (ClusterList::const_iterator iter = pClusterList->begin(), iterEnd = pClusterList->end(); iter != iterEnd; ++iter)
    {
        const Cluster *const pCluster(*iter);

        // Once a cluster has been added to a pfo, it is flagged as unavailable.
        if (!PandoraContentApi::IsAvailable(*this, pCluster))
            continue;

        // Mainly dummy parameters
        PandoraContentApi::ParticleFlowObject::Parameters parameters;
        parameters.m_charge = 0;
        parameters.m_energy = 0.f;
        parameters.m_mass = 0.f;
        parameters.m_momentum = CartesianVector(0.f, 0.f, 0.f);
        parameters.m_particleId = 0;
        parameters.m_clusterList.insert(pCluster);

        try
        {
            const Vertex *const pVertex(ExampleHelper::FindClosestVertex(pCluster, pVertexList, std::numeric_limits<float>::max()));

            // Once a vertex has been added to a pfo, it is flagged as unavailable.
            if (PandoraContentApi::IsAvailable(*this, pVertex))
                parameters.m_vertexList.insert(pVertex);
        }
        catch (StatusCodeException &)
        {
        }

        const Pfo *pPfo(NULL);
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::ParticleFlowObject::Create(*this, parameters, pPfo));
    }

    // Choose to save the temporary pfos under a specified name and to set the pfo list to be the current list.
    if (!pTemporaryList->empty())
    {
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::SaveList<Pfo>(*this, m_outputListName));
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::ReplaceCurrentList<Pfo>(*this, m_outputListName));
    }

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode CreatePfosAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "OutputListName", m_outputListName));

    return STATUS_CODE_SUCCESS;
}

} // namespace example_content
