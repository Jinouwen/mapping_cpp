//
// Created by Ouwen Jin on 2021/11/23.
//

#ifndef MAPPING_FORCE_DIRECTED_H
#define MAPPING_FORCE_DIRECTED_H

#include "placementalgorithm.h"
class ForceDirected: public PlacementAlgorithm{
public:
    Placement do_mapping(Machine &, Network &) override;
    static Placement do_mapping(Placement &);
};


#endif //MAPPING_FORCE_DIRECTED_H
