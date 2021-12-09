//
// Created by Ouwen Jin on 2021/11/22.
//

#ifndef MAPPING_EVALUATOR_H
#define MAPPING_EVALUATOR_H
#include "placement.h"

class Evaluator {
public:
    static double weighted_length_total(const Placement &placement);
    static double longest_path(const Placement &placement);
};


#endif //MAPPING_EVALUATOR_H
