#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>
#include <locale>
#include <codecvt>
#include <windows.h>
#include <limits>
#include "LGraph.h"
#include "Algorithm.h"
#include "LocationInfo.h"
#include "GraphException.h"

using Graph::LGraph;
using Graph::VertInfo;
using namespace Graph::Algorithm;

namespace Utils
{
//强制转为UFT-8
std::string Wstring2UTF8(const std::wstring& wstr)
{
    return wstr.empty() ? "" : std::wstring_convert<
           std::codecvt_utf8<wchar_t>>().to_bytes(wstr);
}

std::wstring UTF82Wstring(const std::string& str)
{
    return str.empty() ? L"" : std::wstring_convert<
           std::codecvt_utf8<wchar_t>>().from_bytes(str);
}

void initConsoleEncoding()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };
    wcscpy_s(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void clearInput()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
}

const std::string nodes_path{ "C:\\Users\\32769\\Desktop\\cases\\random_sparse_10000\\nodes.csv" };
const std::string edges_path{ "C:\\Users\\32769\\Desktop\\cases\\random_sparse_10000\\edges.csv" };

struct edge
{
    std::string from;
    std::string to;
    int length;
};

void init(LGraph& graph);

std::vector<VertInfo> read_nodes(const std::string& nodes_path);

std::vector<edge> read_edges(const std::string& edges_path);

void store_nodes(const std::string& nodes_path, const LGraph& graph);

void store_edges(const std::string& edges_path, const LGraph& graph);

void show_all_nodes(const LGraph& graph);

void show_all_edges(const LGraph& graph);

int main()
{
    Utils::initConsoleEncoding();
    LGraph graph(false);
    init(graph);
    while (true)
    {
        int choice = 0;
        std::cout << "欢迎使用校园导航系统！" << std::endl
                  << "请选择您要进行的操作：" << std::endl
                  << "1.顶点相关操作" << std::endl
                  << "2.边相关操作" << std::endl
                  << "3.从文件中重新加载点与边" << std::endl
                  << "4.是否存在欧拉通路" << std::endl
                  << "5.求任意两点间的最短距离" << std::endl
                  << "6.求最小生成树" << std::endl
                  << "7.求解拓扑受限时的最短路径" << std::endl
                  << "8.求解顶点的邻接边" << std::endl
                  << "9.求解同类型顶点" << std::endl
                  << "10.规划从中北校区大门到闵行校区大门的游览路线" << std::endl
                  << "11.退出程序"<<std::endl
                  << "请输入操作前的数字：";
        std::cin >> choice;
        Utils::clearInput();
        if (choice == 1)
        {
            while (true)
            {
                std::cout << "\n顶点相关操作：" << std::endl
                          << "1.输出特定顶点信息" << std::endl
                          << "2.输出所有顶点信息" << std::endl
                          << "3.添加一个顶点" << std::endl
                          << "4.删除一个顶点" << std::endl
                          << "5.存储顶点到文件中" << std::endl
                          << "6.回到上一级菜单" << std::endl
                          << "请输入操作前的数字：";
                std::cin >> choice;
                Utils::clearInput();
                if (choice == 1)
                {
                    std::string name;
                    std::cout << "请输入顶点名称：";
                    std::getline(std::cin, name);
                    try
                    {
                        VertInfo v{ graph.GetVertex(name) };
                        std::cout << "顶点名称：" << v.name << std::endl
                                  << "顶点类型：" << v.type << std::endl
                                  << "建议游览时间：" << v.visitTime << std::endl;
                    }
                    catch (Graph::GraphException& e)
                    {
                        std::cout << "名称为 " << name << " 的顶点不存在" << std::endl;
                    }
                    std::cout << "按回车键继续...";
                    std::cin.get();
                }
                else if (choice == 2)
                {
                    std::cout << "顶点名称, 顶点类型, 建议游览时间：" << std::endl;
                    show_all_nodes(graph);

                    std::cout << "按回车键继续...";
                    std::cin.get();
                }
                else if (choice == 3)
                {
                    std::string name, type;
                    int visit_time;
                    std::cout << "请输入顶点名称：";
                    std::getline(std::cin, name);
                    std::cout << "请输入顶点类型：";
                    std::getline(std::cin, type);
                    std::cout << "请输入建议游览时间：";
                    std::cin >> visit_time;
                    Utils::clearInput();
                    try
                    {
                        graph.InsertVertex({ name, visit_time, type });
                        std::cout << "顶点添加成功！" << std::endl;
                    }
                    catch (Graph::GraphException& e)
                    {
                        std::cout << e.what() << std::endl;
                    }
                    std::cout << "按回车键继续...";
                    std::cin.get();
                }


                else if (choice == 4)
                {
                    std::string name;
                    std::cout << "请输入顶点名称：";
                    std::getline(std::cin, name);
                    try
                    {
                        graph.DeleteVertex({ name, 0, "" });
                        std::cout << "顶点删除成功！" << std::endl;
                    }
                    catch (Graph::GraphException& e)
                    {
                        std::cout << e.what() << std::endl;
                    }
                    std::cout << "按回车键继续...";
                    std::cin.get();
                }
                else if (choice == 5)
                {
                    std::cout << "正在将顶点存储到test_nodes.csv中..." << std::endl;
                    store_nodes("test_nodes.csv", graph);
                    std::cout << "存储成功！" << std::endl;
                    std::cout << "按回车键继续...";
                    std::cin.get();
                }
                else if (choice == 6)
                {
                    break; // 返回上一级菜单
                }
                else
                {
                    std::cout << "无效选择，请重新输入！" << std::endl;
                }
            }
        }
        else if (choice == 2)   // 边相关操作
        {
            while (true)
            {
                std::cout << "\n边相关操作：" << std::endl
                          << "1.输出特定边信息" << std::endl
                          << "2.输出所有边信息" << std::endl
                          << "3.添加一条边" << std::endl
                          << "4.删除一条边" << std::endl
                          << "5.存储边到文件中" << std::endl
                          << "6.更新边信息" << std::endl  // 新增选项
                          << "7.回到上一级菜单" << std::endl
                          << "请输入操作前的数字：";
                std::cin >> choice;
                Utils::clearInput();
                if (choice == 1)
                {
                    std::string from, to;
                    std::cout << "请输入边的起点与终点(空格分隔)：";
                    std::cin >> from >> to;
                    try
                    {
                        Graph::GElemSet w{ graph.GetEdge(from, to) };
                        std::cout << from << " <---> " << to << " 距离为：" << w << std::endl;
                    }
                    catch (Graph::GraphException& e)
                    {
                        std::cout << from << " <---> " << to << " 不存在！" << std::endl;
                    }
                    std::cout << "按回车键继续...";
                    std::cin.get();
                }
                else if (choice == 2)
                {
                    std::cout << "起点, 终点, 距离：" << std::endl;
                    show_all_edges(graph);
                    std::cout << "按回车键继续..."<< std::endl;
                    std::cin.get();
                }
                else if (choice == 3)
                {
                    std::string from, to;
                    int length;
                    std::cout << "请输入边的起点与终点(空格分隔)：";
                    std::cin >> from >> to;
                    std::cout << "请输入距离：";
                    std::cin >> length;
                    Utils::clearInput();
                    graph.InsertEdge(from, to, length);
                    std::cout << "边添加成功！" << std::endl;
                    std::cout << "按回车键继续..."<< std::endl;
                    std::cin.get();
                }
                else if (choice == 4)
                {
                    std::string from, to;
                    std::cout << "请输入边的起点与终点(空格分隔)：";
                    std::cin >> from >> to;
                    try
                    {
                        graph.DeleteEdge(from, to);
                        std::cout << "边删除成功！" << std::endl;
                    }
                    catch (Graph::GraphException& e)
                    {
                        std::cout << e.what() << std::endl;
                    }
                    std::cout << "按回车键继续...";
                    std::cin.get();
                }
                else if (choice == 5)
                {
                    std::cout << "正在将边存储到test_edges.csv文件中..." << std::endl;
                    store_edges("test_edges.csv", graph);
                    std::cout << "存储成功！" << std::endl;
                    std::cout << "按回车键继续...";
                    std::cin.get();
                }
                else if (choice == 6)
                {
                    std::string vertex_x_name, vertex_y_name;
                    int new_weight;
                    std::cout << "请输入要更新的边的起点与终点(空格分隔)：";
                    std::cin >> vertex_x_name >> vertex_y_name;
                    Utils::clearInput();
                    std::cout << "请输入新的距离：";
                    std::cin >> new_weight;
                    Utils::clearInput();
                    try
                    {
                        graph.UpdateEdge(vertex_x_name, vertex_y_name, new_weight);
                        std::cout << "边信息更新成功！" << std::endl;
                    }
                    catch (Graph::GraphException& e)
                    {
                        std::cout << e.what() << std::endl;
                    }
                    std::cout << "按回车键继续...";
                    std::cin.get();
                }
                else if (choice == 7)
                {
                    break; // 返回上一级菜单
                }
                else
                {
                    std::cout << "无效选择，请重新输入！" << std::endl;
                }
            }
        }

        else if (choice == 3)  // 从文件中重新加载点与边
        {
            init(graph);
            std::cout << "图已重新加载！" << std::endl;

            std::cout << "按回车键继续...";
            std::cin.get();
        }

        else if (choice == 4)   // 是否存在欧拉通路
        {
            auto res = HaveEulerPath(graph);
            std::cout << (res ? "存在欧拉通路" : "不存在欧拉通路") << std::endl;
        }

        else if (choice == 5)   // 求任意两点间的最短距离
        {
            std::string x, y;
            std::cout << "请输入边的起点与终点(空格分隔)：";
            std::cin >> x >> y;
            try
            {
                std::cout << x << "和" << y << "之间的最短距离为：" << GetShortestPath(graph, x, y) << std::endl;
            }
            catch (Graph::GraphException& e)
            {
                std::cout << "计算最短距离时出错: " << std::endl;
            }
            std::cout << "按回车键继续..."<< std::endl;
            std::cin.get();
        }
        else if (choice == 6)   // 求最小生成树
        {
            if (IsConnected(graph))
            {
                std::cout << "最小生成树的边如下：" << std::endl;
                auto res = MinimumSpanningTree(graph);
                int sum = 0;
                for (auto item : res)
                {
                    const auto& e = edge{ graph.List()[item.from].data.name,
                                          graph.List()[item.dest].data.name,
                                          item.weight };
                    std::cout << e.from << " -- " << e.to << " : " << e.length << std::endl;
                    sum += e.length;
                }
                std::cout << "总权重为: " << sum << std::endl;
            }
            else
            {
                std::cout << "图不连通，无法生成最小生成树" << std::endl;
            }
            std::cout << "按回车键继续..."<< std::endl;
            std::cin.get();
        }
        else if (choice == 7)   // 求解拓扑受限时的最短路径
        {
            int n;
            std::vector<std::string> list;
            std::cout << "请输入序列长度：";
            std::cin >> n;
            Utils::clearInput();
            std::cout << "请输入" << n << "个地点（每行一个）：" << std::endl;
            for (int i = 0; i < n; i++)
            {
                std::string x;
                std::cout << "地点 " << (i+1) << ": ";
                std::getline(std::cin, x);
                list.push_back(x);
            }
            try
            {
                int result = TopologicalShortestPath(graph, list);
                std::cout << "最短路径长度为: " << result << std::endl;
            }
            catch (Graph::GraphException& e)
            {
                std::cout << "计算拓扑受限最短路径时出错: " << e.what() << std::endl;
            }
            std::cout << "按回车键继续...";
            std::cin.get();
        }
        else if (choice == 8)
        {
            std::string name;
            std::cout << "请输入顶点名称：";
            std::getline(std::cin, name);

            std::cout << "邻接边: "
                      << GetAdjacentEdgesString(graph, name)
                      << std::endl;
        }
        else if (choice == 9)
        {
            std::string type;
            std::cout << "请输入顶点类型：";
            std::getline(std::cin, type);

            std::cout << "同类型顶点: "
                      << GetVerticesByTypeString(graph, type)
                      << std::endl;
        }
        //拓展功能
        else if (choice == 10)
        {
            std::string start = "中北校区大门";
            std::string end = "闵行校区大门";

            try
            {
                // 获取完整的最短路径信息
                auto shortest = GetCompleteShortestPath(graph, start, end);

                std::cout << "从中北校区大门到闵行校区大门的最短时间为: "
                          << shortest.total_time << " \n";
                std::cout << "路径: ";
                for (size_t i = 0; i < shortest.vertices.size(); ++i)
                {
                    if (i > 0) std::cout << " -> ";
                    std::cout << shortest.vertices[i];
                }
                std::cout << "\n\n";

                int desired_time;
                std::cout << "请输入期望的游览时间(不少于 " << shortest.total_time << "): ";
                std::cin >> desired_time;
                Utils::clearInput();

                if (desired_time < shortest.total_time)
                {
                    std::cout << "错误: 输入时间不能少于最短时间 " << shortest.total_time << " \n";
                }
                else
                {
                    auto paths = FindAllPathsWithinTime(graph, start, end, desired_time);

                    if (paths.empty())
                    {
                        std::cout << "没有找到满足时间限制的路径\n";
                    }
                    else
                    {
                        std::cout << "\n找到 " << paths.size() << " 条满足条件的路径:\n";

                        for (size_t i = 0; i < paths.size(); ++i)
                        {
                            std::cout << "\n路径 " << i+1 << "时间：" << paths[i].total_time << " \n";
                            for (size_t j = 0; j < paths[i].vertices.size(); ++j)
                            {
                                if (j > 0) std::cout << " -> ";
                                std::cout << paths[i].vertices[j];
                            }
                            std::cout << "\n";

                            // 详细时间 breakdown
                            std::cout << "  详细时间:\n";
                            int accumulated = 0;
                            for (size_t j = 0; j < paths[i].vertices.size(); ++j)
                            {
                                auto vertex = graph.GetVertex(paths[i].vertices[j]);
                                accumulated += vertex.visitTime;
                                std::cout << "    - " << vertex.name << ": +" << vertex.visitTime
                                          << "  (累计: " << accumulated << " )\n";

                                if (j < paths[i].vertices.size() - 1)
                                {
                                    int edge_time = graph.GetEdge(paths[i].vertices[j], paths[i].vertices[j+1]);
                                    accumulated += edge_time;
                                    std::cout << "    - " << paths[i].vertices[j] << " 到 "
                                              << paths[i].vertices[j+1] << ": +" << edge_time
                                              << "  (累计: " << accumulated << " )\n";
                                }
                            }
                        }
                    }
                }
            }
            catch (const Graph::GraphException& e)
            {
                std::cout << "错误: 未能找到目标起讫点"<< std::endl;
            }

            std::cout << "\n按回车键继续...";
            std::cin.get();
        }
        //结束
        else if (choice == 11)
        {
            std::cout << "感谢您的使用，再见！\n开发者：\\lky1433223/ \\Voltline/" << std::endl;
            break;
        }
        else
        {
            std::cout << "无效选择，请重新输入！" << std::endl;
        }
    }
    return 0;
}

/* 从文件中读取顶点信息 */
std::vector<VertInfo> read_nodes(const std::string& nodes_path)
{
    std::vector<VertInfo> nodes;
    std::ifstream fin(nodes_path, std::ios::in);

    if (fin)
    {
        std::string line;
        for (int i = 0; std::getline(fin, line); ++i)
        {
            if (i == 0) continue; // 跳过标题行
            if (line.empty()) continue;

            auto parse_field = [&](size_t& pos)
            {
                size_t end = line.find(',', pos);
                std::string field = line.substr(pos, end - pos);
                pos = end + (end != std::string::npos);
                return field;
            };

            size_t pos = 0;
            try
            {
                std::string name = parse_field(pos);
                std::string type = parse_field(pos);
                int visit_time = std::stoi(parse_field(pos));
                nodes.emplace_back(VertInfo{name, visit_time, type});
            }
            catch (...)
            {
                std::cerr << "警告: 解析失败的行: " << line << std::endl;
            }
        }
    }
    std::cout << "从 " << nodes_path << " 读取了 " << nodes.size() << " 个顶点" << std::endl;
    return nodes;
}

/* 从文件中读取边信息 */
std::vector<edge> read_edges(const std::string& edges_path)
{
    std::ifstream fin(edges_path);
    if (!fin.is_open())
    {
        std::cerr << "错误: 无法打开边文件: " << edges_path << std::endl;
        return {};
    }
    std::vector<edge> edges;
    std::string line;
    std::getline(fin, line);
    while (std::getline(fin, line))
    {
        if (line.empty() || line[0] == '#') continue;
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        if (pos1 == std::string::npos || pos2 == std::string::npos)
        {
            std::cerr << "警告: 跳过无效行: " << line << std::endl;
            continue;
        }
        std::string from = line.substr(0, pos1);
        std::string to = line.substr(pos1 + 1, pos2 - pos1 - 1);
        int length = std::stoi(line.substr(pos2 + 1));
        if (from.empty() || to.empty() || length <= 0)
        {
            std::cerr << "警告: 跳过无效边: " << line << std::endl;
            continue;
        }
        edges.push_back({from, to, length});
    }
    fin.close();
    std::cout << "从 " << edges_path << " 读取了 " << edges.size() << " 条边" << std::endl;
    return edges;
}

void init(LGraph& graph)
{
    auto load_data = [](auto& container, const auto& loader, const char* type)
    {
        try
        {
            container = loader();
            //std::cout << "成功读取 " << container.size() << " 个" << type << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "读取" << type << "错误: " << e.what() << std::endl;
        }
    };

    std::vector<VertInfo> nodes;
    std::vector<edge> edges;

    load_data(nodes, [] { return read_nodes(nodes_path); }, "顶点");
    load_data(edges, [] { return read_edges(edges_path); }, "边");

    for (const auto& v : nodes)
    {
        try
        {
            graph.InsertVertex(v);
        }
        catch (...)
        {
            std::cerr << "添加顶点失败: " << v.name << std::endl;
        }
    }

    for (const auto& e : edges)
    {
        try
        {
            graph.InsertEdge(e.from, e.to, e.length);
        }
        catch (...)
        {
            std::cerr << "添加边失败: " << e.from << "->" << e.to << std::endl;
        }
    }
}
/* 打印目前的所有顶点 */
void show_all_nodes(const LGraph& graph)
{
    const auto& graph_list = graph.List();
    for (const auto& v : graph_list)
    {
        const auto& data = v.data;
        std::cout << data.name << ", " << data.type << ", " << data.visitTime << std::endl;
    }
}

/* 打印目前的所有边 */
void show_all_edges(const LGraph& graph)
{
    const auto& graph_edges = graph.SortedEdges();
    const auto& graph_list = graph.List();
    for (const auto& en : graph_edges)
    {
        const auto& from_vertex = graph_list[en.from].data;
        const auto& to_vertex = graph_list[en.dest].data;
        std::cout << from_vertex.name << " -> " << to_vertex.name << " : " << en.weight << std::endl;
    }
}

/* 将顶点信息存储回文件中 */
void store_nodes(const std::string& nodes_path, const LGraph& graph)
{
    std::wofstream fout(nodes_path);
    if (!fout.is_open())
    {
        std::cerr << "无法打开节点文件进行写入: " << nodes_path << std::endl;
        return;
    }
    fout.imbue(std::locale(fout.getloc(), new std::codecvt_utf8<wchar_t>()));
    fout << L"name,type,visitTime" << std::endl;
    const auto& graph_list = graph.List();
    for (const auto& n : graph_list)
    {
        const auto& v = n.data;
        std::wstring w_name = Utils::UTF82Wstring(v.name);  // 修正调用方式
        std::wstring w_type = Utils::UTF82Wstring(v.type);  // 修正调用方式
        fout << w_name << L"," << w_type << L"," << v.visitTime << std::endl;
    }
    fout.close();
}

/* 将边信息存储回文件中 */
void store_edges(const std::string& edges_path, const LGraph& graph)
{
    std::wofstream fout(edges_path);
    if (!fout.is_open())
    {
        std::cerr << "无法打开边文件进行写入: " << edges_path << std::endl;
        return;
    }
    fout.imbue(std::locale(fout.getloc(), new std::codecvt_utf8<wchar_t>()));
    fout << L"from,to,weight" << std::endl;
    const auto& graph_edges = graph.SortedEdges();
    const auto& graph_list = graph.List();
    for (const auto& en : graph_edges)
    {
        std::wstring w_from = Utils::UTF82Wstring(graph_list[en.from].data.name);
        std::wstring w_to = Utils::UTF82Wstring(graph_list[en.dest].data.name);
        fout << w_from << L"," << w_to << L"," << en.weight << std::endl;
    }
    fout.close();
}
