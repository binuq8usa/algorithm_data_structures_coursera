#include "graph.h"

void Graph::AddEdge(Edge edge){
    this->edges.push_back(edge);
    auto it = std::find(this->nodes.begin(),this->nodes.end(),edge.head);
    // add the head node
    if(it == this->nodes.end()) {
        nodes.push_back(edge.head); // add the node if not found
        this->node_edges_map[nodes.end()-1] = std::vector<EdgeIt>(1,edges.end()-1); // add the corresponding edge
    }
    else
        this->node_edges_map[it].push_back(edges.end()-1); // add the corresponding edge iterator to the node map
}

std::string Graph::DisplayGraphCmdLine()
{
    std::stringstream ss;
    ss << "Graph - [Node -> Edge Nodes]\n";
    for(auto it=node_edges_map.begin(); it!= node_edges_map.end(); it++){
        ss << '\t' <<it->first->val << " -> ";
        for(auto it_n = it->second.begin(); it_n != it->second.end(); it_n++)
            ss << (*it_n)->tail.val << '\t';
        ss << '\n';
    }
    return ss.str();
}

