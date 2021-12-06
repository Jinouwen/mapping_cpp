//
// Created by Ouwen Jin on 2021/11/22.
//

#include "zigzag.h"

Placement ZigZag::do_mapping(Machine & machine, Network & network) {
    printf("mapping start, algorithm: zig\n");
    Placement placement(machine, network);
    int node_num = machine.core_num;
    int size_x = machine.size_x;
    int size_y = machine.size_y;
    bool flag = true;
    int now_index = 0;
    for (int i = 0; i < size_x; ++i){
        if (flag){
            for (int j = 0; j < size_y; ++j, ++now_index){
                placement.mapping[now_index] = Pos(i,j);
            }
        }
        else{
            for (int j = size_y - 1; j >= 0; --j, ++now_index){
                placement.mapping[now_index] = Pos(i,j);
            }

        }
        flag = !flag;
    }
    for (int i = 0; i < node_num; ++i){
        placement.index[pos2code(placement.mapping[i],machine.size_y)] = i;
    }
    printf("mapping_done\n");
    return placement;
}
