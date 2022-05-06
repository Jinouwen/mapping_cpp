//
// Created by Ouwen Jin on 2021/11/19.
//

#ifndef MAPPING_NETWORK_H
#define MAPPING_NETWORK_H
#include <vector>
#include <string>

struct Edge{
    int to;
    double weight;
    bool is_reverse;
    Edge(int, double, bool=false);
};
class Network {
public:
    int node_num;
    std::vector<Edge> *edges;

    Network(Network&& ) noexcept ;
    Network(Network& )=default;
    Network& operator=(const Network&)=default;
    explicit Network(int);
    ~Network();
    static Network make_random_net(int, int, int=0);
    static Network make_random_conv_net(int, int, int, int=0);
    static Network load_from_files(const std::string& config_filename);
    static Network expansion_net(Network & init_net, int out_node_num);
};



#endif //MAPPING_NETWORK_H
