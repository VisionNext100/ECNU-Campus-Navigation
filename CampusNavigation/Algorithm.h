//
// Created by 刘凯源 on 24-10-13.
//

#ifndef CAMPUSNAVIGATION_ALGORITHM_H
#define CAMPUSNAVIGATION_ALGORITHM_H

#include "LGraph.h"

namespace Graph{
    namespace Algorithm{
        //拓展功能
        struct PathResult {
        std::vector<std::string> vertices;
        int total_time; // 包括所有顶点和边的权重
    };

    PathResult GetCompleteShortestPath(const LGraph& graph,
                                     const std::string& start,
                                     const std::string& end);

    std::vector<PathResult> FindAllPathsWithinTime(
        const LGraph& graph,
        const std::string& start,
        const std::string& end,
        int max_time);
        //结束

        class DSU{
            private:
                std::vector<int> parent, rank;
            public:
                explicit DSU(int n) : parent(n), rank(n, 0){
                    for (int i = 0; i < n; ++i) parent[i] = i;
                }

            int find(int x){
                if (parent[x] != x) parent[x] = find(parent[x]);
                return parent[x];
            }

            void unite(int x, int y){
                int rx = find(x), ry = find(y);
                if (rx == ry) return;

                if (rank[rx] < rank[ry])    parent[rx] = ry;
                else if (rank[rx] > rank[ry])   parent[ry] = rx;
                else{
                    parent[ry] = rx;
                    rank[rx]++;
                }
            }

            bool same(int x, int y){
                return find(x) == find(y);
            }
        };

        std::list<Vertex> EulerCircle(const LGraph& graph); //计算欧拉回路
        bool HaveEulerPath(const LGraph& graph); //判断是否存在欧拉回路
        bool IsConnected(LGraph& graph); //判断图是否联通
        int GetShortestPath(const LGraph& graph, const std::string& vertex_name_x,const std::string& vertex_name_y); //计算单源最短路径
        int TopologicalShortestPath(const LGraph& graph, std::vector<std::string> path); //计算拓扑受限的最短路径
        std::vector<EdgeNode> MinimumSpanningTree(const LGraph& graph); //计算最小生成树
        std::string GetAdjacentEdgesString(const LGraph& graph, const std::string& vertex_name);
        std::string GetVerticesByTypeString(const LGraph& graph, const std::string& type);
    }
}
#endif //CAMPUSNAVIGATION_ALGORITHM_H
