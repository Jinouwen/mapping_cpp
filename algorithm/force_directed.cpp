//
// Created by Ouwen Jin on 2021/11/23.
//
// TODO: parallelize
#include "force_directed.h"
#include "randommapping.h"
#include "queue"
#include "evaluator.h"
#include "chrono"
#define sign(x) ( ((x) <0 )? -1 : ((x)> 0) )
#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3
#define DEFAULT_METHOD 0
#define eps 1e-6


int count;

inline void add_force(Pos pos_u, Pos pos_v, int index_u, int index_v, double weight, double forces[][4], int method=DEFAULT_METHOD){
    //if (method == DEFAULT_METHOD) {
    count ++;
        if (pos_u.x != pos_v.x) {
            int sign = sign(pos_v.x - pos_u.x);
            forces[index_u][RIGHT] += weight * sign;
            forces[index_u][LEFT] -= weight * sign;
            forces[index_v][RIGHT] -= weight * sign;
            forces[index_v][LEFT] += weight * sign;
        } else {
            forces[index_u][RIGHT] -= weight;
            forces[index_u][LEFT] -= weight;
            forces[index_v][RIGHT] -= weight;
            forces[index_v][LEFT] -= weight;
            if (abs(pos_u.y - pos_v.y) == 1){
                if (pos_v.y > pos_u.y){
                    forces[index_u][UP] -= weight;
                    forces[index_v][DOWN] -= weight;
                }
                else{
                    forces[index_u][DOWN] -= weight;
                    forces[index_v][UP] -= weight;
                }
            }
        }
        if (pos_u.y != pos_v.y) {
            int sign = sign(pos_v.y - pos_u.y);
            forces[index_u][UP] += weight * sign;
            forces[index_u][DOWN] -= weight * sign;
            forces[index_v][UP] -= weight * sign;
            forces[index_v][DOWN] += weight * sign;
        } else {
            forces[index_u][UP] -= weight;
            forces[index_u][DOWN] -= weight;
            forces[index_v][UP] -= weight;
            forces[index_v][DOWN] -= weight;
            if (abs(pos_u.x - pos_v.x) == 1){
                if (pos_v.x > pos_u.x){
                    forces[index_u][RIGHT] -= weight;
                    forces[index_v][LEFT] -= weight;
                }
                else{
                    forces[index_u][LEFT] -= weight;
                    forces[index_v][RIGHT] -= weight;
                }
            }
        }
    //}
}

void print_mapping(int * index, int size_x, int size_y){
    for(int i = 0; i < size_x; ++i){
        for(int j = 0; j < size_y; ++j)
            printf("%2d ",index[i * size_y + j]);
        printf("\n");
    }
    printf("**********************\n");

}
Placement ForceDirected::do_mapping(Machine & machine, Network & network) {
    printf("mapping start, algorithm: force directed\n");
    RandomMapping randomMapping;
    Placement placement = randomMapping.do_mapping(machine, network);
    printf("mapping_done\n");
    return do_mapping(placement);
}

struct SwapPair{
    Pos pos_u,pos_v;
    int index_u{0},index_v{0};
    int direct_u{0}, direct_v{0};
    double force{0};
    SwapPair() = default;
};


Placement ForceDirected::do_mapping(Placement & init_placement) {
    printf("mapping start, algorithm: force directed\n");
    auto start = std::chrono::system_clock::now();
    double percentage = 0.5;
    count = 0;
    Placement placement(init_placement.machine, init_placement.network);
    memcpy(placement.mapping, init_placement.mapping, sizeof(Pos) * placement.core_num);
    memcpy(placement.index, init_placement.index, sizeof(int) * placement.core_num);
    int core_num = placement.machine.core_num;
    auto forces = new double[core_num][4];
    memset(forces,0,sizeof(double) * core_num * 4);
    for (int i = 0; i < core_num; ++i){
        for (auto j: placement.network.edges[i]){
            if(j.is_reverse) continue;
            add_force(placement.mapping[i], placement.mapping[j.to], i, j.to, j.weight, forces);
        }
    }
    int size_x = placement.machine.size_x;
    int size_y = placement.machine.size_y;
    int pair_num = (size_x - 1) * size_y + (size_y - 1) * size_x;
    int now_pair_num = 0;
    auto pairs = new SwapPair[pair_num + 1];
    pairs[pair_num].force = -1;
    auto connect_pairs = new int[core_num][4];
    for(int i = 0; i < core_num; ++i)
        for(int j = 0; j < 4; ++j)
            connect_pairs[i][j] = pair_num;
    for(int i = 0; i < size_x - 1; ++i){
        for (int j = 0; j < size_y; ++j){
            pairs[now_pair_num].pos_u = Pos(i, j);
            pairs[now_pair_num].pos_v = Pos(i + 1, j);
            pairs[now_pair_num].direct_u = RIGHT;
            pairs[now_pair_num].direct_v = LEFT;
            pairs[now_pair_num].index_u = placement.index[pos2code(pairs[now_pair_num].pos_u,size_y)];
            pairs[now_pair_num].index_v = placement.index[pos2code(pairs[now_pair_num].pos_v,size_y)];
            connect_pairs[pos2code(pairs[now_pair_num].pos_u, size_y)][RIGHT] = now_pair_num;
            connect_pairs[pos2code(pairs[now_pair_num].pos_v, size_y)][LEFT] = now_pair_num;
            now_pair_num++;
        }
    }
    for (int i = 0; i < size_x; ++i){
        for (int j = 0; j < size_y - 1; ++j){
            pairs[now_pair_num].pos_u = Pos(i, j);
            pairs[now_pair_num].pos_v = Pos(i, j + 1);
            pairs[now_pair_num].direct_u = UP;
            pairs[now_pair_num].direct_v = DOWN;
            pairs[now_pair_num].index_u = placement.index[pos2code(pairs[now_pair_num].pos_u,size_y)];
            pairs[now_pair_num].index_v = placement.index[pos2code(pairs[now_pair_num].pos_v,size_y)];
            connect_pairs[pos2code(pairs[now_pair_num].pos_u, size_y)][UP] = now_pair_num;
            connect_pairs[pos2code(pairs[now_pair_num].pos_v, size_y)][DOWN] = now_pair_num;
            now_pair_num++;
        }
    }
    int time_step = 1;
    auto in_queue = new int[pair_num + 1];
    int * queue[2];
    queue[0] = new int[pair_num + 1];
    queue[1] = new int[pair_num + 1];
    int queue_num[2];
    int now = time_step & 1;
    int next = now ^ 1;
    queue_num[now] = 0;
    for (int i = 0; i < now_pair_num; ++i){
        pairs[i].force = forces[pairs[i].index_u][pairs[i].direct_u] + forces[pairs[i].index_v][pairs[i].direct_v];
        if (pairs[i].force > eps)
            queue[now][queue_num[now]++] = i;
        in_queue[i] = time_step;
    }
    bool loop_condition = queue_num[now] > 0;
    //double last = 0;

    while(loop_condition){
        now = time_step & 1;
        next = now ^ 1;
        time_step++;
        in_queue[pair_num]=time_step;
        queue_num[next] = 0;
        std::sort(queue[now],queue[now] + queue_num[now],[& pairs] (int a, int b){return pairs[a].force > pairs[b].force;});
        int top = std::max(1, int(queue_num[now] * percentage));
        //printf("%d \n", queue_num[now]);
        for (int i = 0; i < top; ++i){
            SwapPair & pair = pairs[queue[now][i]];
            pair.index_u = placement.index[pos2code(pair.pos_u, size_y)];
            pair.index_v = placement.index[pos2code(pair.pos_v, size_y)];
            if(forces[pair.index_u][pair.direct_u] + forces[pair.index_v][pair.direct_v] > pair.force - eps){
                //do swap
                SwapPair old_copy(pair);
                //printf("%f %f \n",last - Evaluator::evaluate(placement), forces[pair.index_u][pair.direct_u] + forces[pair.index_v][pair.direct_v]);
                //last = Evaluator::evaluate(placement);

                //printf("swapping%d %d\n",old_copy.index_u,old_copy.index_v);
                //print_mapping(placement.index, size_x, size_y);
                //printf("%f ", pair.force);


                for(auto j: placement.network.edges[old_copy.index_u]){
                    add_force(placement.mapping[old_copy.index_u], placement.mapping[j.to], old_copy.index_u, j.to, -j.weight, forces);
                }
                for(auto j: placement.network.edges[old_copy.index_v]){
                    if( j.to != old_copy.index_u)
                        add_force(placement.mapping[old_copy.index_v], placement.mapping[j.to], old_copy.index_v, j.to, -j.weight, forces);
                }
                placement.mapping[old_copy.index_u] = old_copy.pos_v;
                placement.mapping[old_copy.index_v] = old_copy.pos_u;
                placement.index[pos2code(old_copy.pos_u, size_y)] = old_copy.index_v;
                placement.index[pos2code(old_copy.pos_v, size_y)] = old_copy.index_u;
                pair.index_v = old_copy.index_u;
                pair.index_u = old_copy.index_v;
                for(auto j: placement.network.edges[old_copy.index_u]){
                    add_force(placement.mapping[old_copy.index_u], placement.mapping[j.to],  old_copy.index_u, j.to, j.weight, forces);
                    for(int k = 0; k < 4; ++k){
                        int code = pos2code(placement.mapping[j.to], size_y);
                        if(in_queue[connect_pairs[code][k]] != time_step){
                            queue[next][queue_num[next]++] = connect_pairs[code][k];
                            in_queue[connect_pairs[code][k]] = time_step;
                        }
                    }
                }
                for(auto j: placement.network.edges[old_copy.index_v]){
                    if(j.to != old_copy.index_u) {
                        add_force(placement.mapping[old_copy.index_v], placement.mapping[j.to],  old_copy.index_v, j.to, j.weight,
                                  forces);
                        for(int k = 0; k < 4; ++k){
                            int code = pos2code(placement.mapping[j.to], size_y);
                            if(in_queue[connect_pairs[code][k]] != time_step){
                                queue[next][queue_num[next]++] = connect_pairs[code][k];
                                in_queue[connect_pairs[code][k]] = time_step;
                            }
                        }
                    }
                }
                for(int k = 0; k < 4; ++k) {
                    int code = pos2code(old_copy.pos_u, size_y);
                    if (in_queue[connect_pairs[code][k]] != time_step) {
                        queue[next][queue_num[next]++] = connect_pairs[code][k];
                        in_queue[connect_pairs[code][k]] = time_step;
                    }
                }
                for(int k = 0; k < 4; ++k) {
                    int code = pos2code(old_copy.pos_v, size_y);
                    if (in_queue[connect_pairs[code][k]] != time_step) {
                        queue[next][queue_num[next]++] = connect_pairs[code][k];
                        in_queue[connect_pairs[code][k]] = time_step;
                    }
                }
            }
        }
        for(int i = top; i < queue_num[now]; ++i){
            if(in_queue[queue[now][i]] != time_step) {
                queue[next][queue_num[next]++] = queue[now][i];
                in_queue[queue[now][i]] = time_step;
            }
        }
        int t_num = 0;
        for(int i = 0; i < queue_num[next]; ++i){
            SwapPair & pair = pairs[queue[next][i]];
            pair.index_u = placement.index[pos2code(pair.pos_u, size_y)];
            pair.index_v = placement.index[pos2code(pair.pos_v, size_y)];
            pair.force = forces[pair.index_u][pair.direct_u] + forces[pair.index_v][pair.direct_v];
            if(pair.force > eps){
                queue[next][t_num++] = queue[next][i];
            }
        }
        queue_num[next] = t_num;
        if(t_num == 0)
            loop_condition = false;
    }
    delete[] forces;
    delete[] pairs;
    delete[] in_queue;
    delete[] queue[0];
    delete[] queue[1];
    delete[] connect_pairs;
    printf("mapping_done count:%d\n",count);
    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    printf("duration: %f\n",double (duration.count())/std::chrono::milliseconds::period::den);
    return placement;
}
