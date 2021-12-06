//
// Created by Ouwen Jin on 2021/11/19.
//

#ifndef MAPPING_NETWORK_H
#define MAPPING_NETWORK_H
#include <vector>
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
    Network(int);
    ~Network();
    static Network make_random_net(int, int, int=0);
};



#endif //MAPPING_NETWORK_H
