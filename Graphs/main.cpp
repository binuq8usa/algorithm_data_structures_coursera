#include <iostream>
#include "graph.h"

using namespace std;

int main()
{
    Node a(1);
    Node b(2);
    Edge e(a,b);
    Graph g;
    g.AddEdge(e);
    e.head = Node(2);
    e.tail = Node(3);
    g.AddEdge(e);
    std::cout << g.DisplayGraphCmdLine() << std::endl;
    cout << "Hello World!" << endl;
    return 0;
}
