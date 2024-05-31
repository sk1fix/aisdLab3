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
        V value;
        vector<Edge> edges;
    };
    struct Edge {
        Distance distance;
        int _id;
    };
    vector<Vertex> graph;
};