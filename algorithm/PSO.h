//
// Created by Ouwen Jin on 2022/1/6.
//

#ifndef MAPPING_PSO_H
#define MAPPING_PSO_H

#include "placementalgorithm.h"
#include <random>

class PSO :public PlacementAlgorithm{
public:
    Placement do_mapping(Machine &, Network &) override;

};


#endif //MAPPING_PSO_H
