//
// Created by Ouwen Jin on 2021/11/21.
//

#include "randommapping.h"

Placement RandomMapping::do_mapping(Machine &machine, Network &network) {
    printf("mapping start, algorithm: random mapping\n");
    int node_num = network.node_num;
    std::default_random_engine e(1); // NOLINT(cert-msc51-cpp)
    Placement placement(machine, network);
    placement.mapping = new Pos[node_num];
    placement.index = new int[node_num];
    for(int i = 0; i < node_num; ++i){
        placement.mapping[i] = code2pos(i, machine.size_y);
    }
    std::shuffle(placement.mapping,placement.mapping + node_num, e);
    for(int i = 0; i < node_num; ++i){
        placement.index[pos2code(placement.mapping[i], machine.size_y)] = i;
    }
    printf("mapping_done\n");
    return placement;
}
