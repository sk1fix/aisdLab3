#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <algorithm>
#include <limits>
#include <functional>
#include <queue>

using namespace std;

template<typename Ver, typename Distance = double, typename V>
class Graph {
private:
    struct Edge {
        Distance distance;
        V _id; 
    };

    struct Vertex {
        V _id; 
        Ver value;
        vector<Edge> edges; 
    };

    unordered_map<V, Vertex> graph;

public:
    bool has_vertex(V id) const {
        return graph.find(id) != graph.end();
    }
    void add_vertex(V id, const Ver& value) {
        if (!has_vertex(id)) {
            graph[id] = Vertex{ id, value, {} };
        }
    }
    bool remove_vertex(V id) {
        auto it = graph.find(id);
        if (it == graph.end()) return false;
        graph.erase(it);
        for (auto& [_, vertex] : graph) {
            vertex.edges.erase(
                remove_if(vertex.edges.begin(), vertex.edges.end(),
                    [id](const Edge& e) { return e._id == id; }),
                vertex.edges.end());
        }
        return true;
    }
    vector<V> vertices() const {
        vector<V> result;
        for (const auto& [id, _] : graph) {
            result.push_back(id);
        }
        return result;
    }
    void add_edge(V from_id, V to_id, const Distance& d) {
        if (has_vertex(from_id) && has_vertex(to_id)) {
            graph[from_id].edges.push_back(Edge{ d, to_id });
        }
    }
    bool remove_edge(V from_id, V to_id) {
        auto it = graph.find(from_id);
        if (it == graph.end()) return false;

        auto& edges = it->second.edges;
        auto edge_it = remove_if(edges.begin(), edges.end(),
            [to_id](const Edge& e) { return e._id == to_id; });
        if (edge_it == edges.end()) return false;

        edges.erase(edge_it, edges.end());
        return true;
    }
    bool remove_edge(const Edge& e, V from_id) {
        return remove_edge(from_id, e._id);
    }
    bool has_edge(V from_id, V to_id) const {
        auto it = graph.find(from_id);
        if (it == graph.end()) return false;

        const auto& edges = it->second.edges;
        return any_of(edges.begin(), edges.end(),
            [to_id](const Edge& e) { return e._id == to_id; });
    }
    bool has_edge(const Edge& e, V from_id) const {
        return has_edge(from_id, e._id);
    }
};