#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <limits>
#include <functional>
#include <queue>

using namespace std;

namespace graph {

    template<typename Ver, typename Distance = double>
    class Graph {
    private:
        struct Edge {
            Distance distance;
            int to; 
        };
        struct Vertex {
            Ver value; 
            vector<Edge> edges; 
        };
        unordered_map<int, Vertex> graph;
    public:
        bool has_vertex(int id) const {
            return graph.find(id) != graph.end();
        }
        void add_vertex(int id, const Ver& value) {
            if (!has_vertex(id)) {
                graph[id] = Vertex{ value, {} };
            }
        }
        bool remove_vertex(int id) {
            auto it = graph.find(id);
            if (it == graph.end()) return false;
            graph.erase(it);
            for (auto& [_, vertex] : graph) {
                vertex.edges.erase(
                    remove_if(vertex.edges.begin(), vertex.edges.end(),
                        [id](const Edge& e) { return e.to == id; }),
                    vertex.edges.end());
            }
            return true;
        }
        vector<int> vertices() const {
            vector<int> result;
            for (const auto& [id, _] : graph) {
                result.push_back(id);
            }
            return result;
        }
        void add_edge(int from_id, int to_id, const Distance& d) {
            if (has_vertex(from_id) && has_vertex(to_id)) {
                graph[from_id].edges.push_back(Edge{ d, to_id });
            }
        }
        bool remove_edge(int from_id, int to_id) {
            auto it = graph.find(from_id);
            if (it == graph.end()) return false;

            auto& edges = it->second.edges;
            auto edge_it = remove_if(edges.begin(), edges.end(),
                [to_id](const Edge& e) { return e.to == to_id; });
            if (edge_it == edges.end()) return false;

            edges.erase(edge_it, edges.end());
            return true;
        }
        bool remove_edge(const Edge& e, int from_id) {
            return remove_edge(from_id, e.to);
        }
        bool has_edge(int from_id, int to_id) const {
            auto it = graph.find(from_id);
            if (it == graph.end()) return false;
            const auto& edges = it->second.edges;
            return any_of(edges.begin(), edges.end(),
                [to_id](const Edge& e) { return e.to == to_id; });
        }
        bool has_edge(const Edge& e, int from_id) const {
            return has_edge(from_id, e.to);
        }
        vector<Edge> edges(int vertex_id) const {
            auto it = graph.find(vertex_id);
            if (it != graph.end()) {
                return it->second.edges;
            }
            return {};
        }
        size_t order() const {
            return graph.size();
        }
        size_t degree(int vertex_id) const {
            auto it = graph.find(vertex_id);
            if (it != graph.end()) {
                return it->second.edges.size();
            }
            return 0;
        }
        vector<Edge> shortest_path(int from_id, int to_id) const {
            unordered_map<int, Distance> distances;
            unordered_map<int, int> predecessors;
            for (const auto& [id, _] : graph) {
                distances[id] = numeric_limits<Distance>::infinity();
            }
            distances[from_id] = 0;

            for (size_t i = 0; i < graph.size() - 1; ++i) {
                for (const auto& [id, vertex] : graph) {
                    for (const auto& edge : vertex.edges) {
                        if (distances[id] + edge.distance < distances[edge.to]) {
                            distances[edge.to] = distances[id] + edge.distance;
                            predecessors[edge.to] = id;
                        }
                    }
                }
            }
            for (const auto& [id, vertex] : graph) {
                for (const auto& edge : vertex.edges) {
                    if (distances[id] + edge.distance < distances[edge.to]) {
                        throw runtime_error("Graph contains a negative-weight cycle");
                    }
                }
            }
            vector<Edge> path;
            for (int v = to_id; v != from_id; v = predecessors[v]) {
                if (predecessors.find(v) == predecessors.end()) {
                    throw runtime_error("No path found");
                }
                int u = predecessors[v];
                auto it = find_if(graph.at(u).edges.begin(), graph.at(u).edges.end(),
                    [v](const Edge& e) { return e.to == v; });
                if (it != graph.at(u).edges.end()) {
                    path.push_back(*it);
                }
            }
            reverse(path.begin(), path.end());
            return path;
        }
        vector<int> walk(int start_id) const {
            vector<int> result;
            unordered_set<int> visited;
            function<void(int)> dfs = [&](int id) {
                if (visited.count(id)) return;
                visited.insert(id);
                result.push_back(id);
                for (const auto& edge : graph.at(id).edges) {
                    dfs(edge.to);
                }
            };
            dfs(start_id);
            return result;
        }
        int optimal_store() const {
            int best_id = -1;
            Distance min_max_distance = numeric_limits<Distance>::infinity();
            for (const auto& [id, _] : graph) {
                Distance max_distance = 0;
                for (const auto& [other_id, _] : graph) {
                    if (id != other_id) {
                        try {
                            auto path = shortest_path(id, other_id);
                            Distance total_distance = 0;
                            for (const auto& edge : path) {
                                total_distance += edge.distance;
                            }
                            max_distance = max(max_distance, total_distance);
                        }
                        catch (const runtime_error&) {
                            max_distance = numeric_limits<Distance>::infinity();
                        }
                    }
                }
                if (max_distance < min_max_distance) {
                    min_max_distance = max_distance;
                    best_id = id;
                }
            }
            return best_id;
        }
    };
}