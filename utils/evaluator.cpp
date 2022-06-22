//
// Created by Ouwen Jin on 2021/11/22.
//

#include "evaluator.h"

#include <utility>
#include "cmath"
#include <cstdio>
#include <iostream>
#include "visualize.h"
Evaluator::Evaluator(std::string name):
        name(std::move(name)){
    EN_r = 1;
    EN_w = 0.1;
    L_r = 1;
    L_w = 0.01;
}
double Evaluator::get_energy_consumption(const Placement &placement) const {
    double energy_consumption = 0;
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
            energy_consumption += j.weight * (dis * (EN_w + EN_r) + EN_w) ;
        }
    }
    return energy_consumption;

}

std::pair<double, double> Evaluator::get_latency(const Placement &placement) const {
    double longest = -1;
    std::vector<Edge> * & edges = placement.network.edges;
    int node_num = placement.network.node_num;
    int size_y = placement.machine.size_y;
    double sum = 0;
    double total_num = 0;
    for (int i = 0; i < node_num; ++i){
        for (auto j:edges[i]){
            if (j.is_reverse) continue;
            int from = i;
            int to = j.to;
            Pos pos_u = placement.mapping[from];
            Pos pos_v = placement.mapping[to];
            int dis = get_manhattan_dis(pos_u, pos_v);
            double latency = dis * (L_w + L_r) + L_w;
            longest = longest < latency ? latency : longest;
            sum += latency;
            total_num += 1;
        }
    }
    return {sum/total_num, longest};

}
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


std::pair<double,double> Evaluator::congestion(Placement &placement) {
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


void Evaluator::evaluate() {
    std::cerr << "evaluating..." << std::endl;
    std::string filename =name+"_result.txt";
    double energy_consumption = 0;
    double average_latency = 0;
    double maximum_latency = 0;
    std::pair<double, double > congestion_result(0,0);
    std::pair<double, double > latency(0, 0);
    FILE *fp=freopen(filename.c_str(),"w", stdout);

    for (auto & task : task_list){
        std::cerr << task.name << std::endl;
        name = task.name;
        Placement & placement = task.placement;
        energy_consumption = get_energy_consumption(placement);
        latency = get_latency(placement);
        congestion_result = congestion(placement);
        std::cout << std::endl;
        std::cout << "algorithm: " << name << std::endl;
        std::cout << "Energy consumption: "<< std::endl << energy_consumption << std::endl;
        std::cout << "Average latency: "<< std::endl << latency.first << std::endl;
        std::cout << "Maximum latency: "<< std::endl << latency.second << std::endl;
        std::cout << "Average congestion: "<< std::endl << congestion_result.first << std::endl;
        std::cout << "Maximum congestion: "<< std::endl << congestion_result.second << std::endl;
        std::cout << "Mapping time spent: "<< std::endl << placement.time_spent << " seconds"<< std::endl;
    }
    fflush(fp);
    fclose(fp);
    freopen("/dev/tty","w",stdout);
    std::cerr<<"evaluate done. Result saved in ./" << filename << std::endl;
}

void Evaluator::add_task(Placement &placement, std::string task_name) {
    task_list.emplace_back(Task(placement, std::move(task_name)));
}



