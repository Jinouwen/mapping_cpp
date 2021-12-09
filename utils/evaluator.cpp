//
// Created by Ouwen Jin on 2021/11/22.
//

#include "evaluator.h"
#include "cmath"
double Evaluator::weighted_length_total(const Placement & placement) {
    double now_cost = 0;
    std::vector<Edge> * & edges = placement.network.edges;
    int node_num = placement.network.node_num;
    int size_y = placement.machine.size_y;
    for (int i = 0; i < node_num; ++i){
        for (auto j:edges[i]){
            if (j.is_reverse) continue;
            int from = i;
            int to = j.to;
            Pos pos_u = placement.mapping[from];
            Pos pos_v = placement.mapping[to];
            int dis = get_manhattan_dis(pos_u, pos_v);
            now_cost += j.weight * dis;
        }
    }
    return now_cost;
}

double Evaluator::longest_path(const Placement &placement) {
    double longest = -1;
    std::vector<Edge> * & edges = placement.network.edges;
    int node_num = placement.network.node_num;
    int size_y = placement.machine.size_y;
    for (int i = 0; i < node_num; ++i){
        for (auto j:edges[i]){
            if (j.is_reverse) continue;
            int from = i;
            int to = j.to;
            Pos pos_u = placement.mapping[from];
            Pos pos_v = placement.mapping[to];
            int dis = get_manhattan_dis(pos_u, pos_v);
            longest = longest < dis ? dis : longest;
        }
    }
    return longest;
}

