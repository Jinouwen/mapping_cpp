//
// Created by Ouwen Jin on 2021/11/22.
//

#ifndef MAPPING_EVALUATOR_H
#define MAPPING_EVALUATOR_H
#include <utility>

#include "placement.h"
#include "vector"
#include "placementalgorithm.h"


struct Task{
    Placement & placement;
    std::string name;
    Task(Placement & placement,std::string name):placement(placement), name(std::move(name)){};
};

class Evaluator {
public:
    std::string name;
    std::vector<Task> task_list;


    //evaluate methods
    static double weighted_length_total(const Placement &placement);
    static double longest_path(const Placement &placement);
    double get_energy_consumption(const Placement &placement) const;
    std::pair<double, double > get_latency(const Placement & placement) const;
    static std::pair<double, double> congestion(Placement &placement);


    explicit Evaluator(std::string name);
    void add_task(Placement & placement, std::string name);
    void evaluate();

private:
    //parameters
    double EN_r;
    double EN_w;
    double L_r;
    double L_w;




};



#endif //MAPPING_EVALUATOR_H
