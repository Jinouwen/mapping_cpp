//
// Created by Ouwen Jin on 2022/4/27.
//

#include <iomanip>
#include "visualize.h"
#include "iostream"
void Visualize::draw_mapping(Placement &placement) {
    Machine machine = placement.machine;
    auto x = machine.size_x;
    auto y = machine.size_y;
    std::cerr<<"--------------------------"<< std::endl;
    for (int i = 0; i < placement.network.node_num; ++i){
        std::cerr << i << ": " << placement.mapping[i].x << ' '<<placement.mapping[i].y << std::endl;
    }
    std::cerr<<"--------------------------"<< std::endl;
}

void Visualize::draw_index(Placement &placement) {
    Machine machine = placement.machine;
    auto x = machine.size_x;
    auto y = machine.size_y;
    std::cerr<<"--------------------------"<< std::endl;
    for (int i = 0; i < x; ++i){
        for (int j = 0; j < y; ++j){
            std::cerr << std::setw(4) << placement.index[pos2code(Pos(i,j),y)] ;
        }
        std::cerr << std::endl;
    };
    std::cerr<<"--------------------------"<< std::endl;

}
