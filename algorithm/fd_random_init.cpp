//
// Created by Ouwen Jin on 2022/4/26.
//

#include "fd_random_init.h"
#include "randommapping.h"
Placement FD_random_init::do_mapping(Machine & machine, Network &network) {
    RandomMapping random_mapping;
    ForceDirected forceDirected;
    Placement placement = random_mapping.do_mapping(machine,network);
    return forceDirected.do_mapping(placement);
}
