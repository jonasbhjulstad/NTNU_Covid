#ifndef NV_NODE_INSTANCE_HPP
#define NV_NODE_INSTANCE_HPP
#include <vector>
#include "NV_Edge.hpp"
#include "NV_glTFBasicInstance.hpp"


    std::vector<NodeInstanceData> prepareInstanceData(std::vector<NodeInstanceData> &nodeInstanceData, const igraph_t& graph)
	{
		const uint32_t N_nodes = static_cast<uint32_t>(nodeInstanceData.size());

		igraph_es_t edgeSelector;
		igraph_es_all(&edgeSelector, IGRAPH_EDGEORDER_ID);

		igraph_eit_t edgeIterator;
		igraph_eit_create(&graph, edgeSelector, &edgeIterator);
		
		instanceData.edge.reserve(IGRAPH_EIT_SIZE(edgeIterator));
		igraph_integer_t node_source, node_target;
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
            instanceData.edge.push_back(
                { nodeSourcePos, nodeTargetPos, edgeScale });
			IGRAPH_EIT_NEXT(edgeIterator);
		}
	}


#endif
