#pragma once
#include<picture.h>
using namespace graph;
int main()
{
    graph::Graph<string> g;
    g.add_vertex(1, "A");
    g.add_vertex(2, "B");
    g.add_vertex(3, "C");
    g.add_edge(1, 2, 1.0);
    g.add_edge(2, 3, 2.0);
    g.add_edge(1, 3, 4.0);

    auto vertices = g.vertices();
    cout << "Vertices: ";
    for (const auto& v : vertices) {
        cout << v << " ";
    }
    cout << endl;

    auto edges = g.edges(1);
    cout << "Edges from vertex 1: ";
    for (const auto& e : edges) {
        cout << "(" << e.to << ", " << e.distance << ") ";
    }
    cout << endl;

    auto path = g.shortest_path(1, 3);
    cout << "Shortest path from 1 to 3: ";
    for (const auto& e : path) {
        cout << "(" << e.to << ", " << e.distance << ") ";
    }
    cout << endl;

    int optimal_store_id = g.optimal_store();
    cout << "Optimal store location: " << optimal_store_id << endl;

    return 0;
}