//
// Created by Ouwen Jin on 2022/4/26.
//

#ifndef MAPPING_FD_RANDOM_INIT_H
#define MAPPING_FD_RANDOM_INIT_H
#include "force_directed.h"

class FD_random_init: ForceDirected{
    Placement do_mapping(Machine &, Network &) override;
};


#endif //MAPPING_FD_RANDOM_INIT_H
