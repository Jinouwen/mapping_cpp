//
// Created by Ouwen Jin on 2021/11/21.
//

#ifndef MAPPING_PLACEMENTALGORITHM_H
#define MAPPING_PLACEMENTALGORITHM_H

#include "placement.h"
#include "machine.h"
#include "network.h"

class PlacementAlgorithm {
public:
    virtual Placement do_mapping(Machine &, Network &) = 0;
};


#endif //MAPPING_PLACEMENTALGORITHM_H
