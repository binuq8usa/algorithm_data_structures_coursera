#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <string>
#include <sstream>

struct Node{
    int val;
    Node(){}
    Node(int value):val(value){}
    void operator()(const Node& rhs){
        this->val = rhs.val;
    }
    bool operator==(const Node& rhs){
        return this->val == rhs.val;
    }

    ~Node(){}
};

struct Edge{
    Node head;
    Node tail;
    explicit Edge(Node a, Node b):head(a), tail(b){}
    ~Edge(){}
};

typedef std::vector<Node>::iterator NodeIt;
typedef std::vector<Edge>::iterator EdgeIt;

class Graph{
private:
    std::vector<Node> nodes;
    std::vector<Edge> edges;
    std::map<NodeIt,std::vector<EdgeIt>> node_edges_map;

public:
    Graph(){}
    virtual ~Graph(){
        node_edges_map.clear();
        nodes.clear();
        edges.clear();
    }

    size_t num_edges() const{
        return edges.size();
    }

    Node GetVertex(int id) const{
        if(id > 0 && id < nodes.size()) return nodes[id];
        else return Node();
    }
    void AddEdge(Edge a);
    std::string DisplayGraphCmdLine();
};

#endif // GRAPH_H
