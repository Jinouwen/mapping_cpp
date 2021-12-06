//
// Created by Ouwen Jin on 2021/11/22.
//

#include "circle.h"

inline int next(int x){ return (x + 1) % 4;}

Placement Circle::do_mapping(Machine & machine, Network & network) {
    printf("mapping start, algorithm: circle\n");
    Placement placement(machine, network);
    int direct_x[4] = {1, 0, -1, 0};
    int direct_y[4] = {0, 1, 0, -1};
    int & node_num = network.node_num;
    int now_x = 0;
    int now_y = 0;
    int now_dict = 0;
    for (int i = 0; i < node_num; ++i){
        placement.mapping[i] = Pos(now_x, now_y);
        now_x = now_x + direct_x[now_dict];
        now_y = now_y + direct_y[now_dict];
        if (now_x < 0 || now_x >= machine.size_x || now_y < 0 || now_y >= machine.size_y){
            now_x = now_x - direct_x[now_dict];
            now_y = now_y - direct_y[now_dict];
            now_dict = next(now_dict);
            now_x = now_x + direct_x[now_dict];
            now_y = now_y + direct_y[now_dict];
        }
    }
    placement.build_index_from_mapping();
    printf("mapping_done\n");
    return placement;
}
