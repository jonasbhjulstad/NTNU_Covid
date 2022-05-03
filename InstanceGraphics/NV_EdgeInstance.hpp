#ifndef NV_EDGE_INSTANCE_HPP
#define NV_EDGE_INSTANCE_HPP
#include <vector>
#include <igraph/igraph.h>
#include <vulkan/vulkan.hpp>
#include "NV_NodeInstance.hpp"

struct EdgeInstanceData
{
    glm::vec3 startNode;
    glm::vec3 endNode;
    glm::vec3 scale;

    // Per-Instance attributes
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions(uint32_t bind_ID, uint32_t layoutOffset)
    {
        return {
            initializers::vertexInputAttributeDescription(bind_ID, layoutOffset, VK_FORMAT_R32G32B32_SFLOAT, 0),                     // StartPos
            initializers::vertexInputAttributeDescription(bind_ID, layoutOffset + 1, VK_FORMAT_R32G32B32_SFLOAT, sizeof(float)*3),                     // EndPos
            initializers::vertexInputAttributeDescription(bind_ID, layoutOffset + 2, VK_FORMAT_R32G32B32_SFLOAT, sizeof(float)*6),                     // Scale
        };
    }
};


    std::vector<EdgeInstanceData> prepareEdgeInstanceData(std::vector<NodeInstanceData> &nodeInstanceData, const igraph_t& graph)
	{
		const uint32_t N_nodes = static_cast<uint32_t>(nodeInstanceData.size());
        std::vector<EdgeInstanceData> instanceData;
		igraph_es_t edgeSelector;
		igraph_es_all(&edgeSelector, IGRAPH_EDGEORDER_ID);

		igraph_eit_t edgeIterator;
		igraph_eit_create(&graph, edgeSelector, &edgeIterator);
		
		instanceData.reserve(IGRAPH_EIT_SIZE(edgeIterator));
		igraph_integer_t node_source, node_target;
        igraph_matrix_e mat;
		while(!IGRAPH_EIT_END(edgeIterator))
		{
			igraph_edge(&graph, IGRAPH_EIT_GET(edgeIterator), &node_source, &node_target);
			
            glm::vec3 nodeSourcePos = glm::vec3(igraph_matrix_e(&mat, node_source, 0), 
			igraph_matrix_e(&mat, node_source, 1), 
			igraph_matrix_e(&mat, node_source, 2));
            glm::vec3 nodeTargetPos = glm::vec3(igraph_matrix_e(&mat, node_target, 0), 
			igraph_matrix_e(&mat, node_target, 1), 
			igraph_matrix_e(&mat, node_target, 2));
            
            glm::vec3 edgeScale = glm::vec3(1.0f, 1.0f, 1.0f);
            instanceData.push_back(
                { nodeSourcePos, nodeTargetPos, edgeScale });
			IGRAPH_EIT_NEXT(edgeIterator);
		}
        return instanceData;
	}

#endif
