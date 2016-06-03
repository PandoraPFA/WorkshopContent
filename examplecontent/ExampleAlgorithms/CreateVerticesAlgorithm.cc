/**
 *  @file   WorkshopContent/examplecontent/ExampleAlgorithms/CreateVerticesAlgorithm.cc
 * 
 *  @brief  Implementation of the create vertices algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "examplecontent/ExampleAlgorithms/CreateVerticesAlgorithm.h"

using namespace pandora;

namespace example_content
{

StatusCode CreateVerticesAlgorithm::Run()
{
    // Create one vertex per input cluster
    const ClusterList *pClusterList(nullptr);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pClusterList));

    // Algorithms must either create a temporary list for newly created vertices. Any vertices remaining in a temporary
    // list at the end of the algorithm will be deleted, so all vertices must be saved before the algorithm ends.
    const VertexList *pTemporaryList(nullptr);
    std::string temporaryListName;
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::CreateTemporaryListAndSetCurrent(*this, pTemporaryList, temporaryListName));

    for (const Cluster *const pCluster : *pClusterList)
    {
        PandoraContentApi::Vertex::Parameters parameters;
        parameters.m_position = pCluster->GetCentroid(pCluster->GetInnerPseudoLayer());
        parameters.m_vertexLabel = VERTEX_START;
        parameters.m_vertexType = VERTEX_3D;

        const Vertex *pVertex(nullptr);
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::Vertex::Create(*this, parameters, pVertex));
    }

    // Choose to save the temporary vertices under a specified name and to set the vertex list to be the current list.
    if (!pTemporaryList->empty())
    {
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::SaveList<Vertex>(*this, m_outputListName));
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::ReplaceCurrentList<Vertex>(*this, m_outputListName));
    }

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode CreateVerticesAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "OutputListName", m_outputListName));

    return STATUS_CODE_SUCCESS;
}

} // namespace example_content
