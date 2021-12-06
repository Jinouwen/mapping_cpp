//
// Created by Ouwen Jin on 2021/11/19.
//

#include "placement.h"

Placement::Placement(Machine & machine1, Network & network1):machine(machine1),network(network1),core_num(machine1.core_num){
    mapping = new Pos[core_num];
    index = new int[core_num];

}

Placement::~Placement() {
    delete [] mapping;
    delete [] index;
}

int Placement::get_index(int code) const {
    return index[code];
}

int Placement::get_index(int x, int y) const {
    int code = pos2code(x, y, machine.size_y);
    return index[code];
}

void Placement::build_index_from_mapping() const {
    for (int i = 0; i < machine.core_num; ++i){
        index[pos2code(mapping[i], machine.size_y)] = i;
    }
}

