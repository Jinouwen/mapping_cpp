//
// Created by Ouwen Jin on 2021/11/19.
//

#include "network.h"
#include <random>

inline void add_two_way_edges(std::vector<Edge > & from_edges, std::vector<Edge > & to_edges, int from, int to, double weight){
    from_edges.emplace_back(to, weight, false);
    to_edges.emplace_back(from, weight, true);
}

Edge::Edge(int to, double weight, bool is_reverse):to(to),weight(weight),is_reverse(is_reverse){
}

Network::Network(int node_num):node_num(node_num){
    edges = new std::vector<Edge>[node_num];
}

Network Network::make_random_net(int layer_num, int node_per_layer, int method) {
    printf("Net: random weight full connect\n");
    std::default_random_engine e(1); // NOLINT(cert-msc51-cpp)
    std::uniform_real_distribution<double > u(0, 1);
    int node_num = layer_num * node_per_layer;
    Network net(node_num);
    auto &edges = net.edges;
    for(int i=0; i < layer_num-2; i++){
        for(int j=node_per_layer*i; j < node_per_layer*i+node_per_layer; j++){
            for(int k=node_per_layer*(i+1); k < node_per_layer*(i+1)+node_per_layer; k++){
                double weight=u(e);
                add_two_way_edges(edges[j], edges[k], j, k, method == 0 ? weight : 1); // two-way edges
            }
        }
    }
    return net;
    // Dependency compiler optimization, automatic return of references, extended life cycle
}

Network::~Network() {
    printf("destroy net\n");
    delete[] edges;
}

Network Network::make_random_conv_net(int layer_num, int node_per_layer, int kernal_size, int method) {

    printf("Net: random weight conv net\n");
    std::default_random_engine e(1); // NOLINT(cert-msc51-cpp)
    std::uniform_real_distribution<double > u(0, 1);
    int node_num = layer_num * node_per_layer;
    Network net(node_num);
    auto &edges = net.edges;
    for(int i=0; i < layer_num-2; i++){
        for(int j=node_per_layer*i; j < node_per_layer*i+node_per_layer; j++){
            int window_l = j - node_per_layer*i;
            int window_r = kernal_size + window_l > node_per_layer ? node_per_layer : kernal_size + window_l;
            for(int k=node_per_layer*(i+1) + window_l; k < node_per_layer*(i+1)+window_r; k++){
                double weight=u(e);
                add_two_way_edges(edges[j], edges[k], j, k, method == 0 ? weight : 1); // two-way edges
            }
        }
    }
    return net;
    // Dependency compiler optimization, automatic return of references, extended life cycle
}

