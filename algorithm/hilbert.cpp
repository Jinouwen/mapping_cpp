//
// Created by Ouwen Jin on 2021/11/19.
//

#include "hilbert.h"
#include "iostream"
#define next(x,y) ((x + y) % 4)
#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3



void Hilbert::dfs(int & now_index, int & now_x, int & now_y, Pos * & mapping ,int now_direct, int now_size, int flag)
{
    if (now_size == 1){
        // std::cout << now_x << " " << now_y << " " << now_index << std::endl;
        mapping[now_index] = Pos(now_x, now_y);
        return;
    }
    int sub_size = now_size >> 1;
    dfs(now_index, now_x, now_y, mapping, next(now_direct, flag), sub_size, 4 - flag);
    now_x += direct_x[now_direct];
    now_y += direct_y[now_direct];
    now_index++;
    dfs(now_index, now_x, now_y, mapping, now_direct, sub_size, flag);
    now_x += direct_x[next(now_direct,flag)];
    now_y += direct_y[next(now_direct,flag)];
    now_index++;
    dfs(now_index, now_x, now_y, mapping, now_direct, sub_size, flag);
    now_x += direct_x[next(now_direct, flag + flag)];
    now_y += direct_y[next(now_direct, flag + flag)];
    now_index++;
    dfs(now_index, now_x, now_y, mapping, next(now_direct, 4 - flag), sub_size,4 - flag);
}

Placement Hilbert::do_mapping(Machine & machine, Network & network) {
    printf("mapping start, algorithm: hilbert curve\n");
    auto start = std::chrono::system_clock::now();
    Placement placement(machine, network);
    int now_index = 0;
    int now_x = 0;
    int now_y = 0;
    dfs(now_index, now_x, now_y, placement.mapping, UP, machine.size_x, 1);
    placement.build_index_from_mapping();
    printf("mapping done\n");
    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    printf("duration: %f\n",double (duration.count())/std::chrono::milliseconds::period::den);
    return placement;
}
