//
// Created by Ouwen Jin on 2021/11/19.
//

#include "network.h"
#include <random>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <cstring>
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
}

Network Network::load_from_files(const std::string &dir_path) {
    int node_max = 0;
    std::vector<std::pair<int, int> > data_ij;
    std::vector<double> data_w;
    std::string file_name = dir_path + "/dataij.bin";
    std::ifstream in_ij(file_name, std::ios::in | std::ios::binary);
    std::ifstream in_w(dir_path + "/dataw.bin", std::ios::in | std::ios::binary);
    int source_node, target_node;
    std::cout << "processing " << file_name << std::endl;
    if(!in_ij.is_open()){
        throw std::runtime_error("cant find data file");
    }
    int arr_ij[2] = {0};
    int arr_w[1] = {0};
    while(!in_ij.eof())
    {
        in_ij.read((char*) arr_ij, sizeof arr_ij);
        source_node = arr_ij[0];
        target_node = arr_ij[1];
        node_max = std::max(node_max, source_node);
        node_max = std::max(node_max, target_node);
        data_ij.emplace_back(std::pair<int, int> (source_node, target_node));
    }
    int weight;
    while(!in_w.eof())
    {
        in_w.read((char*) arr_w, sizeof  arr_w);
        weight = arr_w[0];
        data_w.push_back((double) weight);
    }
    std::cout << "find node num:" << node_max + 1 << std::endl;
    std::cout << "find edge num:" << data_ij.size() << std::endl;

    int node_num = int(std::sqrt(node_max + 1));
    if(node_num * node_num < node_max + 1)
        node_num += 1;
    auto net = Network( node_num * node_num);
    for(int i = 0; i < data_ij.size(); ++i){
        add_two_way_edges(net.edges[data_ij[i].first], net.edges[data_ij[i].second], data_ij[i].first, data_ij[i].second, data_w[i]);
    }
    return net;
}

Network Network::expansion_net(Network &init_net, int out_node_num) {
    if(init_net.node_num == out_node_num){
        return init_net;
    }
    auto net = Network(out_node_num);
    for (int i = 0; i < init_net.node_num; ++i)
        net.edges[i] = init_net.edges[i];
    return net;
}

Network::Network(Network && a) noexcept{
    std::cerr << "move construct!" << std::endl;
    node_num = a.node_num;
    edges = a.edges;
    a.edges = nullptr;
}


