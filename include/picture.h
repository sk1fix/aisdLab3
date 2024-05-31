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
template<typename Ver, typename Distance = double>
class Graph {
private:
    struct Vertex {
        int _id;
        Ver value;
        vector<Edge> edges;
    };
    struct Edge {
        Distance distance;
        int _id;
    };
    vector<Vertex> graph;
public:
    bool has_vertex(int id) const {
        return find_if(graph.begin(), graph.end(), [id](const Vertex& v) { return v._id == id; }) != graph.end();
    }
    void add_vertex(int id, const Ver& value) {
        if (!has_vertex(id)) {
            graph.push_back(Vertex{ id, value, {} });
        }
    }
    bool remove_vertex(int id) {
        auto temp = find_if(graph.begin(), graph.end(),
            [id](const Vertex& v) { return v._id == id; });
        if (temp == graph.end()) return false;

        graph.erase(it);
        for (auto& vertex : graph) {
            vertex.edges.erase(
                remove_if(vertex.edges.begin(), vertex.edges.end(),
                    [id](const Edge& e) { return e._id == id; }),
                vertex.edges.end());
        }
        return true;
    }
    vector<int> vertices() const {
        vector<int> result;
        for (const auto& vertex : graph) {
            result.push_back(vertex._id);
        }
        return result;
    }
};