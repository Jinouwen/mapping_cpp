//
// Created by Ouwen Jin on 2021/11/21.
//

#ifndef MAPPING_RANDOMMAPPING_H
#define MAPPING_RANDOMMAPPING_H

#include <random>

#include "placementalgorithm.h"

class RandomMapping:public PlacementAlgorithm{
public:
    Placement do_mapping(Machine & machine, Network & network) override;
};


#endif //MAPPING_RANDOMMAPPING_H
