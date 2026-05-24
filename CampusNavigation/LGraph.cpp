#include "LGraph.h"
#include "GraphException.h"
#include <algorithm>

namespace Graph
{
LGraph::LGraph(bool directed)
    : n_verts(0), m_edges(0), directed(directed), ver_list() {}

bool LGraph::exist_vertex(const std::string& name) const
{
    return vertex_map.count(name) > 0;
}

bool LGraph::exist_edge(const std::string& vertex_x_name, const std::string& vertex_y_name) const
{
    if (!exist_vertex(vertex_x_name) || !exist_vertex(vertex_y_name))
        return false;

    const auto& x = vertex_map.at(vertex_x_name);
    const auto& y = vertex_map.at(vertex_y_name);

    return std::any_of(ver_list[x].adj.begin(), ver_list[x].adj.end(),
        [y, this](const EdgeNode& e) {
            return e.dest == y || (!directed && e.from == y);
        });
}

void LGraph::InsertVertex(const LocationInfo& vertex_info)
{
    if (exist_vertex(vertex_info.name)) {
        ver_list[vertex_map[vertex_info.name]].data = vertex_info;
        return;
    }

    if (deleted.erase(vertex_info.name)) {}

    vertex_map[vertex_info.name] = n_verts++;
    ver_list.emplace_back(vertex_info);
}

void LGraph::DeleteVertex(const LocationInfo& vertex_info)
{
    auto it = vertex_map.find(vertex_info.name);
    if (it == vertex_map.end()) {
        std::cerr << "顶点不存在: " << vertex_info.name << '\n';
        return;
    }

    Vertex v = it->second;

    for (auto& head : ver_list) {
        head.adj.remove_if([v](const EdgeNode& e) {
            return e.from == v || e.dest == v;
        });
    }

    ver_list.erase(ver_list.begin() + v);
    vertex_map.erase(it);
    deleted.insert(vertex_info.name);
    n_verts--;

    for (auto& [name, idx] : vertex_map) {
        if (idx > v) idx--;
    }

    for (auto& head : ver_list) {
        for (auto& edge : head.adj) {
            if (edge.from > v) edge.from--;
            if (edge.dest > v) edge.dest--;
        }
    }
}

void LGraph::UpdateVertex(const LocationInfo& old_info, LocationInfo& new_info)
{
    if (!exist_vertex(old_info.name)) {
        std::cerr << "顶点不存在: " << old_info.name << '\n';
        return;
    }
    ver_list[vertex_map[old_info.name]].data = new_info;
}

VertInfo LGraph::GetVertex(const std::string& name) const
{
    auto it = vertex_map.find(name);
    if (it == vertex_map.end()) {
        std::cerr << "顶点不存在: " << name << '\n';
        return {};
    }
    return ver_list[it->second].data;
}

VertInfo LGraph::GetVertex(const Vertex vertex) const
{
    if (vertex >= n_verts) {
        std::cerr << "顶点ID无效\n";
        return {};
    }
    return ver_list[vertex].data;
}

void LGraph::InsertEdge(const std::string& vertex_x_name, const std::string& vertex_y_name, GElemSet weight)
{
    if (!exist_vertex(vertex_x_name) || !exist_vertex(vertex_y_name)) {
        std::cerr << "顶点不存在\n";
        return;
    }

    Vertex x = vertex_map[vertex_x_name];
    Vertex y = vertex_map[vertex_y_name];

    if (exist_edge(vertex_x_name, vertex_y_name)) {
        std::cerr << "边已存在\n";
        return;
    }

    ver_list[x].adj.emplace_back(x, y, weight);
    if (!directed) {
        ver_list[y].adj.emplace_back(y, x, weight);
    }
    m_edges++;
}

void LGraph::DeleteEdge(const std::string& vertex_x_name, const std::string& vertex_y_name)
{
    if (!exist_vertex(vertex_x_name) || !exist_vertex(vertex_y_name)) {
        std::cerr << "顶点不存在\n";
        return;
    }
    DeleteEdge(vertex_map[vertex_x_name], vertex_map[vertex_y_name]);
}

void LGraph::DeleteEdge(Vertex vertex_x, Vertex vertex_y)
{
    auto remove_edge = [this](Vertex u, Vertex v) {
        ver_list[u].adj.remove_if([v](const EdgeNode& e) {
            return e.dest == v;
        });
    };

    remove_edge(vertex_x, vertex_y);
    if (!directed) {
        remove_edge(vertex_y, vertex_x);
    }
    m_edges--;
}

void LGraph::UpdateEdge(const std::string& vertex_x_name, const std::string& vertex_y_name, GElemSet new_weight)
{
    DeleteEdge(vertex_x_name, vertex_y_name);
    InsertEdge(vertex_x_name, vertex_y_name, new_weight);
}

GElemSet LGraph::GetEdge(const std::string& vertex_x_name, const std::string& vertex_y_name) const
{
    if (!exist_vertex(vertex_x_name) || !exist_vertex(vertex_y_name)) {
        std::cerr << "顶点不存在\n";
        return {};
    }

    Vertex x = vertex_map.at(vertex_x_name);
    Vertex y = vertex_map.at(vertex_y_name);

    for (const auto& edge : ver_list[x].adj) {
        if (edge.dest == y) {
            return edge.weight;
        }
    }

    std::cerr << "边不存在\n";
    return {};
}

std::vector<EdgeNode> LGraph::SortedEdges(std::function<bool(const GElemSet&, const GElemSet&)> cmp) const
{
    std::vector<EdgeNode> result;

    for (Vertex u = 0; u < n_verts; ++u) {
        for (const auto& edge : ver_list[u].adj) {
            if (directed || u < edge.dest) {
                result.push_back(edge);
            }
        }
    }

    std::sort(result.begin(), result.end(), [&cmp](const EdgeNode& a, const EdgeNode& b) {
        return cmp(a.weight, b.weight);
    });

    return result;
}
} // namespace Graph
