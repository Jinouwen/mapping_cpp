//
// Created by Ouwen Jin on 2021/11/19.
//

#include "hilbert.h"
#include "iostream"
#include "chrono"
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

void Hilbert::dfs(int &now_index, Pos now_pos, Pos start_pos, Pos end_pos, int size_x, int size_y, Pos *&pos_list) {
    //std::cerr << now_pos.x << " " << now_pos.y << "  :  " << now_pos.x + size_x - 1 << " " << now_pos.y + size_y - 1 << std::endl;
    if (size_x == 1 && size_y ==1){
        pos_list[now_index] = now_pos;
        return;
    }
    if (size_x >= size_y * 1.5 && start_pos.x != end_pos.x){
        int mid_x0 = (start_pos.x + end_pos.x)/2;
        int mid_x1 = mid_x0 + 1;
        if (start_pos.x > end_pos.x)
            std::swap(mid_x0, mid_x1);
        int mid_y;
        if ((std::abs(mid_x0 - start_pos.x) + 1) % 2 == 0)
            mid_y = start_pos.y;
        else
            mid_y = now_pos.y == start_pos.y ? now_pos.y + size_y - 1: now_pos.y;
        dfs(now_index, Pos(std::min(start_pos.x, mid_x0), now_pos.y), start_pos, Pos(mid_x0, mid_y),
            std::abs(mid_x0 - start_pos.x) + 1, size_y, pos_list);
        now_index++;
        dfs(now_index, Pos(std::min(end_pos.x, mid_x1), now_pos.y), Pos(mid_x1,mid_y), end_pos,
            std::abs(mid_x1 - end_pos.x) +1,size_y,pos_list);
    }
    else if (size_y >= size_x * 1.5 && start_pos.y != end_pos.y){
        int mid_y0 = (start_pos.y + end_pos.y)/2;
        int mid_y1 = mid_y0 + 1;
        if (start_pos.y > end_pos.y)
            std::swap(mid_y0, mid_y1);
        int mid_x;
        if ((std::abs(mid_y0 - start_pos.y) + 1) % 2 == 0)
            mid_x = start_pos.x;
        else
            mid_x = now_pos.x == start_pos.x ? now_pos.x + size_x - 1 : now_pos.x;
        dfs(now_index, Pos(now_pos.x, std::min(start_pos.y, mid_y0)), start_pos, Pos(mid_x, mid_y0),
            size_x, std::abs(mid_y0 - start_pos.y) + 1, pos_list);
        now_index++;
        dfs(now_index, Pos(now_pos.x, std::min(end_pos.y, mid_y1)), Pos(mid_x, mid_y1), end_pos,
            size_x, std::abs(mid_y1 - end_pos.y) + 1,pos_list);
    }
    else if (start_pos.x == end_pos.x || start_pos.y == end_pos.y){
        int x0 = start_pos.x;
        int x1 = end_pos.x;
        int y0 = start_pos.y;
        int y1 = end_pos.y;
        if (x0 == x1){
            x1 = now_pos.x == x0 ? now_pos.x + size_x - 1 : now_pos.x;
        }
        else{
            y1 = now_pos.y == y0 ? now_pos.y + size_y - 1 : now_pos.y;
        }
        int mid_x0 = (x0 + x1) / 2;
        int mid_x1 = mid_x0 + 1;
        int mid_y0 = (y0 + y1) / 2;
        int mid_y1 = mid_y0 + 1;
        if (x0 > x1){
            std::swap(mid_x0, mid_x1);
        }
        if(y0 > y1){
            std::swap(mid_y0, mid_y1);
        }
        int x0_flag = std::abs(mid_x0 - x0 + 1) % 2;
        int x1_flag = std::abs(mid_x1 - x1 + 1) % 2;
        int y0_flag = std::abs(mid_y0 - y0 + 1) % 2;
        int y1_flag = std::abs(mid_y1 - y1 + 1) % 2;
        if (x0_flag == 1 && x1_flag == 0){
            int mid_x0_ = x0 + x1 - mid_x1;
            int mid_x1_ = x0 + x1 - mid_x0;
            mid_x0 = mid_x0_;
            mid_x1 = mid_x1_;
            x0_flag = std::abs(mid_x0 - x0 + 1) % 2;
            x1_flag = std::abs(mid_x1 - x1 + 1) % 2;
        }
        if (y0_flag == 1 && y1_flag == 0){
            int mid_y0_ = y0 + y1 - mid_y1;
            int mid_y1_ = y0 + y1 - mid_y0;
            mid_y0 = mid_y0_;
            mid_y1 = mid_y1_;
            y0_flag = std::abs(mid_y0 - y0 + 1) % 2;
            y1_flag = std::abs(mid_y1 - y1 + 1) % 2;
        }
        int len_x0 = std::abs(mid_x0 - x0) + 1;
        int len_y0 = std::abs(mid_x0 - x0) + 1;
        int len_x1 = std::abs(x1 - mid_x1) + 1;
        int len_y1 = std::abs(y1 - mid_y1) + 1;
        if (start_pos.x == end_pos.x){
            if (x0_flag == 1 && x1_flag == 1 && y0_flag == 0 && y1_flag == 0){
                mid_y0 -= 1;
                mid_y1 -= 1;
                y0_flag = std::abs(mid_y0 - start_pos.y) % 2;
                y1_flag = std::abs(mid_y1 - end_pos.y) % 2;
            }
            int now_pos_x0 = std::min(x0, mid_x0);
            int now_pos_x1 = std::min(x1, mid_x1);
            int now_pos_y0 = std::min(y0, mid_y0);
            int now_pos_y1 = std::min(y1, mid_y1);
            dfs(now_index,Pos(now_pos_x0, now_pos_y0), start_pos, Pos(mid_x0, y0),
                len_x0, len_y0, pos_list);
            now_index++;
            int mid_x2 = x1_flag + y0_flag == 1 ? x1 : mid_x1;
            dfs(now_index, Pos(now_pos_x1, now_pos_y0), Pos(mid_x1, y0), Pos(mid_x2, mid_y0),
                len_x1, len_y0, pos_list);
            now_index++;
            dfs(now_index, Pos(now_pos_x1, now_pos_y1), Pos(mid_x2, mid_y1), Pos(mid_x1, y1),
                len_x1, len_y1, pos_list);
            now_index++;
            dfs(now_index, Pos(now_pos_x0, now_pos_y1), Pos(mid_x0,y1), Pos(x0, y1),
                len_x0, len_y1, pos_list);
        }
        else{
            if (y0_flag == 1 && y1_flag == 1 && x0_flag == 0 && x1_flag == 0){
                mid_x0 -= 1;
                mid_x1 -= 1;
                x0_flag = std::abs(mid_x0 - start_pos.x) % 2;
                x1_flag = std::abs(mid_x1 - end_pos.x) % 2;
            }
            int now_pos_x0 = std::min(x0, mid_x0);
            int now_pos_x1 = std::min(x1, mid_x1);
            int now_pos_y0 = std::min(y0, mid_y0);
            int now_pos_y1 = std::min(y1, mid_y1);
            dfs(now_index, Pos(now_pos_x0, now_pos_y0), start_pos, Pos(x0, mid_y0),
                len_x0, len_y0, pos_list);
            now_index++;
            int mid_y2 = x0_flag + y1_flag == 1 ? y1 : mid_y1;
            dfs(now_index, Pos(now_pos_x0, now_pos_y1), Pos(x0, mid_y1), Pos(mid_x0, mid_y2),
                len_x0, len_y1, pos_list);
            now_index++;
            dfs(now_index, Pos(now_pos_x1, now_pos_y1), Pos(mid_x1, mid_y2), Pos(x1, mid_y1),
                len_x1, len_y1,pos_list);
            now_index++;
            dfs(now_index, Pos(now_pos_x1, now_pos_y0), Pos(x1, mid_y0), Pos(x1, y0),
                len_x1, len_y0, pos_list);
        }
    }
    else{
        auto &x0 = start_pos.x;
        auto &y0 = start_pos.y;
        auto &x1 = end_pos.x;
        auto &y1 = end_pos.y;
        int len_x = std::abs(x1 - x0) + 1;
        int len_y = std::abs(y1 - y0) + 1;
        int x_flag = len_x % 2;
        int y_flag = len_y % 2;
       // if (x_flag == 0 && y_flag == 0)
       //     std::cerr << "error while divide into 3*3" << std::endl;
        int dx0 = len_x / 3;
        int dy0 = len_y / 3;
        if (dx0 % 2 == 0) {
            if (x_flag == 0)
                dx0 -= 1;
            else
                dx0 = len_x - 2 * dx0;
        }
        if (dy0 % 2 == 0){
            if (y_flag == 0)
                dy0 -= 1;
            else
                dy0 = len_y - 2 * dy0;
        }
        int one_x = x1 > x0 ? 1 : -1;
        int one_y = y1 > y0 ? 1 : -1;
        dx0 *= one_x;
        dy0 *= one_y;
        int mid_x0 = x0 + dx0 - one_x;
        int mid_x1 = mid_x0 + one_x;
        int mid_x3 = x1 - dx0 + one_x;
        int mid_x2 = mid_x3 - one_x;

        int mid_y0 = y0 + dy0 - one_y;
        int mid_y1 = mid_y0 + one_y;
        int mid_y3 = y1 - dy0 + one_y;
        int mid_y2 = mid_y3 - one_y;
        int len_x0 = std::abs(mid_x0 - x0) + 1;
        int len_x1 = std::abs(mid_x2 - mid_x1) + 1;
        int len_x2 = std::abs(x1 - mid_x3) + 1;
        int len_y0 = std::abs(mid_y0 - y0) + 1;
        int len_y1 = std::abs(mid_y2 - mid_y1) + 1;
        int len_y2 = std::abs(y1 - mid_y3) + 1;
        int now_pos_x0 = std::min(x0, mid_x0);
        int now_pos_x1 = std::min(mid_x1, mid_x2);
        int now_pos_x2 = std::min(mid_x3, x1);
        int now_pos_y0 = std::min(y0, mid_y0);
        int now_pos_y1 = std::min(mid_y1, mid_y2);
        int now_pos_y2 = std::min(mid_y3, y1);
        auto pos00 = Pos(now_pos_x0, now_pos_y0);
        auto pos01 = Pos(now_pos_x0, now_pos_y1);
        auto pos02 = Pos(now_pos_x0, now_pos_y2);
        auto pos10 = Pos(now_pos_x1, now_pos_y0);
        auto pos11 = Pos(now_pos_x1, now_pos_y1);
        auto pos12 = Pos(now_pos_x1, now_pos_y2);
        auto pos20 = Pos(now_pos_x2, now_pos_y0);
        auto pos21 = Pos(now_pos_x2, now_pos_y1);
        auto pos22 = Pos(now_pos_x2, now_pos_y2);
        if (len_x > len_y){
            dfs(now_index, pos00, Pos(x0, y0)        , Pos(mid_x0, mid_y0), len_x0, len_y0, pos_list);
            now_index++;
            dfs(now_index, pos01, Pos(mid_x0, mid_y1), Pos(x0, mid_y2)    , len_x0, len_y1, pos_list);
            now_index++;
            dfs(now_index, pos02, Pos(x0, mid_y3)    , Pos(mid_x0, y1)    , len_x0, len_y2, pos_list);
            now_index++;
            dfs(now_index, pos12, Pos(mid_x1, y1)    , Pos(mid_x2, mid_y3), len_x1, len_y2, pos_list);
            now_index++;
            dfs(now_index, pos11, Pos(mid_x2, mid_y2), Pos(mid_x1, mid_y1), len_x1, len_y1, pos_list);
            now_index++;
            dfs(now_index, pos10, Pos(mid_x1, mid_y0), Pos(mid_x2, y0)    , len_x1, len_y0, pos_list);
            now_index++;
            dfs(now_index, pos20, Pos(mid_x3, y0)    , Pos(x1,mid_y0)     , len_x2, len_y0, pos_list);
            now_index++;
            dfs(now_index, pos21, Pos(x1, mid_y1)    , Pos(mid_x3, mid_y2), len_x2, len_y1, pos_list);
            now_index++;
            dfs(now_index, pos22, Pos(mid_x3, mid_y3), Pos(x1,y1)         , len_x2, len_y2, pos_list);
        }
        else{
            dfs(now_index, pos00, Pos(x0, y0)        , Pos(mid_x0, mid_y0), len_x0, len_y0, pos_list);
            now_index++;
            dfs(now_index, pos10, Pos(mid_x1, mid_y0), Pos(mid_x2, y0)    , len_x1, len_y0, pos_list);
            now_index++;
            dfs(now_index, pos20, Pos(mid_x3, y0)    , Pos(x1,mid_y0)     , len_x2, len_y0, pos_list);
            now_index++;
            dfs(now_index, pos21, Pos(x1, mid_y1)    , Pos(mid_x3, mid_y2), len_x2, len_y1, pos_list);
            now_index++;
            dfs(now_index, pos11, Pos(mid_x2, mid_y2), Pos(mid_x1, mid_y1), len_x1, len_y1, pos_list);
            now_index++;
            dfs(now_index, pos01, Pos(mid_x0, mid_y1), Pos(x0, mid_y2)    , len_x0, len_y1, pos_list);
            now_index++;
            dfs(now_index, pos02, Pos(x0, mid_y3)    , Pos(mid_x0, y1)    , len_x0, len_y2, pos_list);
            now_index++;
            dfs(now_index, pos12, Pos(mid_x1, y1)    , Pos(mid_x2, mid_y3), len_x1, len_y2, pos_list);
            now_index++;
            dfs(now_index, pos22, Pos(mid_x3, mid_y3), Pos(x1,y1)         , len_x2, len_y2, pos_list);

        }



    }
}

Placement Hilbert::do_mapping(Machine & machine, Network & network) {
    printf("mapping start, algorithm: hilbert curve\n");
    if (network.node_num > machine.core_num)
        throw std::runtime_error("core num less than node num");
    auto start = std::chrono::system_clock::now();
    Placement placement(machine, network);
    int now_index = 0;
    auto end_pos = Pos(machine.size_x - 1, 0);

    if (machine.size_x % 2 == 1 && machine.size_y % 2 == 0)
        end_pos = Pos(0, machine.size_y -1);
    dfs(now_index, Pos(0,0), Pos(0,0), end_pos, machine.size_x, machine.size_y, placement.mapping);
    placement.build_index_from_mapping();
    printf("mapping done\n");
    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    printf("duration: %f\n",double (duration.count())/std::chrono::milliseconds::period::den);
    return placement;
}


Placement Hilbert::do_mapping_old(Machine & machine, Network & network) {
    printf("mapping start, algorithm: hilbert curve\n");
    auto start = std::chrono::system_clock::now();
    Placement placement(machine, network);
    int now_index = 0;
    int now_x = 0;
    int now_y = 0;
    auto pos_list = new Pos[machine.core_num];

    dfs(now_index, now_x, now_y, placement.mapping, UP, machine.size_x, 1);
    placement.build_index_from_mapping();
    printf("mapping done\n");
    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    printf("duration: %f\n",double (duration.count())/std::chrono::milliseconds::period::den);
    return placement;
}
void Hilbert::test() {
    int size = 16;
    int size_x = size;
    int size_y = size;
    int core_num = size_x * size_y;
    auto pos_list = new Pos[core_num];
    int now_index = 0;
    int index[size_x][size_y];
    dfs(now_index,Pos(0,0), Pos(0, 0), Pos(size_x - 1, 0), size_x, size_y, pos_list);

    for (int i = 0; i < core_num; ++i){
        index[pos_list[i].x][pos_list[i].y] = i;
    }
    for (int i = 0; i < size_x; ++i){
        for (int j = 0; j < size_y; ++j){
            printf("%5d", index[i][j]);
        }
        printf("\n");
        std::fflush(stdout);
    }

    Machine machine(size_x);
    Network network(size);
    Placement placement(machine, network);
    now_index = 0;
    int now_x = 0;
    int now_y = 0;
    dfs(now_index, now_x, now_y, placement.mapping, UP, machine.size_x, 1);

    for (int i = 0; i < core_num; ++i){
        index[placement.mapping[i].x][placement.mapping[i].y] = i;
    }
    for (int i = 0; i < size_x; ++i){
        for (int j = 0; j < size_y; ++j){
            printf("%5d", index[i][j]);
        }
        printf("\n");
    }
}

int main_(){
    Hilbert hilbert;
    auto machine = Machine(60,60);
    auto net = Network::load_from_files("/tmp/pycharm_project_498/snntoolbox_applications/models/inceptionV3");
    auto placement = hilbert.do_mapping(machine, net);
    for (int i = 0; i < machine.size_x; ++i) {
        for (int j = 0; j < machine.size_y; ++j) {
            printf("%5d", placement.index[pos2code(i, j, machine.size_y)]);
            fflush(stdout);
        }
        printf("\n");
        fflush(stdout);
    }

}


