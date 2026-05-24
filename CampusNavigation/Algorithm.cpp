#include "Algorithm.h"
#include "GraphException.h"
#include <queue>
#include <stack>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <climits>

namespace Graph {
namespace Algorithm {

void BFSv(LGraph& g, Vertex v, std::vector<bool>& visited) {
    std::queue<Vertex> q;
    q.push(v);
    visited[v] = true;

    while (!q.empty()) {
        Vertex current = q.front();
        q.pop();

        for (const auto& edge : g.List()[current].adj) {
            Vertex neighbor = edge.dest;
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
}

bool IsConnected(LGraph& graph) {
    if (graph.VertexCount() == 0) return true;

    std::vector<bool> visited(graph.VertexCount(), false);
    BFSv(graph, 0, visited);

    return std::all_of(visited.begin(), visited.end(), [](bool v) { return v; });
}

namespace {
    void DFSFindPaths(
        const LGraph& graph,
        Vertex current,
        Vertex target,
        std::vector<bool>& visited,
        std::vector<Vertex>& path,
        std::vector<std::vector<Vertex>>& all_paths,
        int current_time,
        int max_time,
        const std::vector<int>& vertex_weights)
    {
        // 添加当前顶点的时间
        current_time += vertex_weights[current];

        if (current_time > max_time || visited[current]) {
            return;
        }

        visited[current] = true;
        path.push_back(current);

        if (current == target) {
            all_paths.push_back(path);
        } else {
            for (const auto& edge : graph.List()[current].adj) {
                DFSFindPaths(graph, edge.dest, target, visited, path,
                           all_paths, current_time + edge.weight, max_time, vertex_weights);
            }
        }

        path.pop_back();
        visited[current] = false;
    }
}

PathResult GetCompleteShortestPath(const LGraph& graph,
                                 const std::string& start,
                                 const std::string& end) {
    PathResult result;
    const auto& vertex_map = graph.Map();

    if (vertex_map.find(start) == vertex_map.end() ||
        vertex_map.find(end) == vertex_map.end()) {
        throw GraphException("起点或终点不存在");
    }

    Vertex start_v = vertex_map.at(start);
    Vertex target = vertex_map.at(end);
    int n = graph.VertexCount();

    std::vector<int> dist(n, INT_MAX);
    std::vector<Vertex> prev(n, -1);
    std::priority_queue<std::pair<int, Vertex>,
                       std::vector<std::pair<int, Vertex>>,
                       std::greater<>> pq;

    dist[start_v] = 0;
    pq.push({0, start_v});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (u == target) break;
        if (d > dist[u]) continue;

        for (const auto& edge : graph.List()[u].adj) {
            int new_dist = d + edge.weight;
            if (new_dist < dist[edge.dest]) {
                dist[edge.dest] = new_dist;
                prev[edge.dest] = u;
                pq.push({new_dist, edge.dest});
            }
        }
    }

    // 重建路径
    std::vector<Vertex> path;
    for (Vertex v = target; v != -1; v = prev[v]) {
        path.push_back(v);
    }
    std::reverse(path.begin(), path.end());

    // 转换为顶点名称
    for (Vertex v : path) {
        result.vertices.push_back(graph.GetVertex(v).name);
    }

    // 计算总时间（边权重 + 所有顶点权重）
    result.total_time = dist[target];
    for (Vertex v : path) {
        result.total_time += graph.GetVertex(v).visitTime;
    }

    return result;
}

std::vector<PathResult> FindAllPathsWithinTime(
    const LGraph& graph,
    const std::string& start_name,
    const std::string& end_name,
    int max_time)
{
    const auto& vertex_map = graph.Map();
    if (vertex_map.find(start_name) == vertex_map.end() ||
        vertex_map.find(end_name) == vertex_map.end()) {
        throw GraphException("起点或终点不存在");
    }

    Vertex start = vertex_map.at(start_name);
    Vertex end = vertex_map.at(end_name);

    // 准备顶点权重数据
    std::vector<int> vertex_weights(graph.VertexCount(), 0);
    for (const auto& node : graph.List()) {
        Vertex v = vertex_map.at(node.data.name);
        vertex_weights[v] = node.data.visitTime;
    }

    std::vector<bool> visited(graph.VertexCount(), false);
    std::vector<Vertex> path;
    std::vector<std::vector<Vertex>> all_paths;

    DFSFindPaths(graph, start, end, visited, path, all_paths, 0, max_time, vertex_weights);

    // 转换为结果格式
    std::vector<PathResult> result;
    for (const auto& p : all_paths) {
        PathResult pr;
        int total_time = 0;

        for (size_t i = 0; i < p.size(); ++i) {
            Vertex v = p[i];
            pr.vertices.push_back(graph.GetVertex(v).name);
            total_time += graph.GetVertex(v).visitTime;

            if (i < p.size() - 1) {
                total_time += graph.GetEdge(graph.GetVertex(v).name,
                                       graph.GetVertex(p[i+1]).name);
            }
        }

        pr.total_time = total_time;
        result.push_back(pr);
    }

    return result;
}

int GetShortestPath(const LGraph& graph, const std::string& src, const std::string& dst) {
    const auto& vertex_map = graph.Map();
    if (vertex_map.find(src) == vertex_map.end() ||
        vertex_map.find(dst) == vertex_map.end()) {
        throw GraphException("顶点不存在");
    }

    Vertex start = vertex_map.at(src);
    Vertex target = vertex_map.at(dst);
    int n = graph.VertexCount();

    std::vector<int> dist(n, INT_MAX);
    std::vector<Vertex> prev(n, -1);

    using DistVertex = std::pair<int, Vertex>;
    std::priority_queue<DistVertex, std::vector<DistVertex>, std::greater<DistVertex>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (u == target) {
            std::vector<Vertex> path;
            for (Vertex v = u; v != -1; v = prev[v]) {
                path.push_back(v);
            }
            std::reverse(path.begin(), path.end());

            for (size_t i = 0; i < path.size(); ++i) {
                if (i > 0) std::cout << " -> ";
                std::cout << graph.GetVertex(path[i]).name;
            }
            std::cout << " (" << d << ")\n";
            return d;
        }

        if (d > dist[u]) continue;

        for (const auto& edge : graph.List()[u].adj) {
            int new_dist = d + edge.weight;
            if (new_dist < dist[edge.dest]) {
                dist[edge.dest] = new_dist;
                prev[edge.dest] = u;
                pq.push({new_dist, edge.dest});
            }
        }
    }

    std::cout << "路径不可达\n";
    return -1;
}

std::list<Vertex> FindCircuit(LGraph& graph, Vertex start) {
    std::stack<Vertex> stack;
    std::list<Vertex> circuit;

    auto& adj_list = const_cast<std::vector<HeadNode>&>(graph.List());
    std::vector<std::list<EdgeNode>::iterator> next_edge(adj_list.size());

    for (size_t i = 0; i < adj_list.size(); ++i) {
        next_edge[i] = adj_list[i].adj.begin();
    }

    Vertex current = start;
    stack.push(current);

    while (!stack.empty()) {
        if (next_edge[current] != adj_list[current].adj.end()) {
            Vertex next = next_edge[current]->dest;
            ++next_edge[current];

            adj_list[next].adj.remove_if([current](const EdgeNode& e) {
                return e.dest == current;
            });

            stack.push(current);
            current = next;
        } else {
            circuit.push_front(current);
            current = stack.top();
            stack.pop();
        }
    }

    return circuit;
}

bool HaveEulerPath(const LGraph& graph) {
    LGraph temp = graph;
    if (!IsConnected(temp)) {
        return false;
    }

    int oddDegreeCount = 0;
    for (const auto& node : graph.List()) {
        if (node.adj.size() % 2 != 0) {
            oddDegreeCount++;
            if (oddDegreeCount > 2) {
                return false;
            }
        }
    }

    return true;
}

std::list<Vertex> EulerCircle(const LGraph& graph) {
    if (!HaveEulerPath(graph)) {
        return {};
    }
    LGraph temp = graph;
    return FindCircuit(temp, 0);
}

int TopologicalShortestPath(const LGraph& graph, std::vector<std::string> path) {
    const auto& vertex_map = graph.Map();
    std::vector<Vertex> vertex_path;

    for (const auto& name : path) {
        if (vertex_map.find(name) == vertex_map.end()) {
            throw GraphException("顶点不存在: " + name);
        }
        vertex_path.push_back(vertex_map.at(name));
    }

    std::vector<int> dist(graph.VertexCount(), INT_MAX);
    dist[vertex_path[0]] = 0;

    for (size_t i = 0; i < vertex_path.size(); ++i) {
        Vertex u = vertex_path[i];
        if (dist[u] == INT_MAX) continue;

        for (const auto& edge : graph.List()[u].adj) {
            bool valid = std::any_of(vertex_path.begin() + i + 1, vertex_path.end(),
                [&edge](Vertex v) { return v == edge.dest; });

            if (valid && dist[edge.dest] > dist[u] + edge.weight) {
                dist[edge.dest] = dist[u] + edge.weight;
            }
        }
    }

    return dist[vertex_path.back()] == INT_MAX ? -1 : dist[vertex_path.back()];
}

std::vector<EdgeNode> MinimumSpanningTree(const LGraph& graph) {
    if (!IsConnected(const_cast<LGraph&>(graph))) {
        return {};
    }

    auto edges = graph.SortedEdges();
    DSU dsu(graph.VertexCount());
    std::vector<EdgeNode> mst;

    for (const auto& edge : edges) {
        if (!dsu.same(edge.from, edge.dest)) {
            dsu.unite(edge.from, edge.dest);
            mst.push_back(edge);

            if (mst.size() == static_cast<size_t>(graph.VertexCount() - 1)) {
                break;
            }
        }
    }

    return mst;
}

// algorithm.cpp
std::string GetAdjacentEdgesString(const LGraph& graph, const std::string& vertex_name) {
    const auto& vertex_map = graph.Map();
    auto it = vertex_map.find(vertex_name);
    if (it == vertex_map.end()) {
        return "顶点不存在";
    }

    std::ostringstream oss;
    const auto& adj = graph.List()[it->second].adj;

    if (adj.empty()) {
        return "N/A";
    }

    for (const auto& edge : adj) {
        oss << graph.GetVertex(edge.dest).name << "(" << edge.weight << ") ";
    }

    return oss.str();
}

std::string GetVerticesByTypeString(const LGraph& graph, const std::string& type) {
    std::ostringstream oss;
    bool found = false;

    for (const auto& node : graph.List()) {
        if (node.data.type == type) {
            oss << node.data.name << " ";
            found = true;
        }
    }

    return found ? oss.str() : "未找到该类型的顶点";
}

} // namespace Algorithm
} // namespace Graph
