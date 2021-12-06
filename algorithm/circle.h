//
// Created by Ouwen Jin on 2021/11/22.
//

#ifndef MAPPING_CIRCLE_H
#define MAPPING_CIRCLE_H

#include "placementalgorithm.h"

class Circle: public PlacementAlgorithm{
public:
    Placement do_mapping(Machine &, Network &) override;
};


#endif //MAPPING_CIRCLE_H
