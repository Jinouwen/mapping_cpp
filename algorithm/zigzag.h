//
// Created by Ouwen Jin on 2021/11/22.
//

#ifndef MAPPING_ZIGZAG_H
#define MAPPING_ZIGZAG_H
#include "placementalgorithm.h"

class ZigZag:public PlacementAlgorithm{
public:
    Placement do_mapping(Machine &, Network &) override;

};


#endif //MAPPING_ZIGZAG_H
