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


std::pair<double,double> Evaluator::congestion(const Placement &placement) {
    #define get_right(x,y) ((x)*size_y+(y)+size_total)
    #define get_up(x,y) ((x)*(size_y-1)+(y))
    auto & machine = placement.machine;
    auto & edges = placement.network.edges;
    auto & mapping = placement.mapping;
    auto & node_num = placement.network.node_num;
    int route_num = 2 * machine.size_x * machine.size_y - machine.size_x - machine.size_y;
    auto * route = new double[route_num]();
    int size_y = machine.size_y;
    int size_x = machine.size_x;
    int size_total = (size_y - 1) * size_x;
    for (int i = 0; i < node_num; ++i){
        for (auto j: edges[i]){
            if (j.is_reverse) continue;
            int from = i;
            int to = j.to;
            Pos pos_u = mapping[from];
            Pos pos_v = mapping[to];
            int x_min = std::min(pos_u.x, pos_v.x);
            int x_max = std::max(pos_u.x, pos_v.x);
            int y_min = std::min(pos_u.y, pos_v.y);
            int y_max = std::max(pos_u.y, pos_v.y);
            double right_base = y_max - y_min + 1;
            double up_base = x_max - x_min + 1;
            for (int xx = x_min; xx < x_max; ++xx){
                for (int yy = y_min; yy <= y_max; ++yy){
                    route[get_right(xx,yy)] += j.weight / right_base;
                }
            }
            for (int yy = y_min; yy < y_max; ++yy){
                for (int xx = x_min; xx <= x_max; ++xx){
                    route[get_up(xx,yy)] += j.weight / up_base;
                }
            }
        }
    }
    double sum = 0;
    double max_congestion;
    for (int i = 0; i < route_num; ++i){
        sum += route[i];
        max_congestion = std::max(max_congestion, route[i]);
    }
    double average = sum / route_num;
    return {average, max_congestion};
}

