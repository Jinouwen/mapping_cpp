//
// Created by Ouwen Jin on 2021/11/19.
//

#ifndef MAPPING_HILBERT_H
#define MAPPING_HILBERT_H

#include "placementalgorithm.h"

static int direct_x[4] = {1,0,-1,0};
static int direct_y[4] = {0,-1,0,1};
class Hilbert: public PlacementAlgorithm{
public:
    Placement do_mapping(Machine &, Network &) override;
    Placement do_mapping_old(Machine &, Network &);
    void test();
private:
    void dfs(int & now_index, int & now_x, int & now_y, Pos * & mapping ,int now_direct, int now_size, int flag);
    void dfs(int & now_index, Pos now_pos, Pos start_pos, Pos end_pos, int size_x, int size_y, Pos * & pos_list);

};


#endif //MAPPING_HILBERT_H
