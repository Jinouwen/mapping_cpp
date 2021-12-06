#include <iostream>
#include "sir/machine.h"
#include "sir/network.h"
#include "algorithm/randommapping.h"
#include "algorithm/zigzag.h"
#include "algorithm/circle.h"
#include "utils/evaluator.h"
#include "algorithm/hilbert.h"
#include "algorithm/force_directed.h"
void test1(){

    int size = 512;
    Machine machine(size);
    int core_num = size * size;
    printf("core num:%d\n", core_num);
    int layer_num, node_per_layer;
    node_per_layer = size;
    layer_num = core_num / node_per_layer;
    auto net = Network::make_random_net(layer_num, node_per_layer);
    printf("layer num: %d, node per layer: %d\n",layer_num, node_per_layer);
    Hilbert hilbert;
    RandomMapping random_mapping;
    ZigZag zig_zag;
    Circle circle;
    Placement placement_hilbert = hilbert.do_mapping(machine, net);
    Placement placement_FD_hilbert = ForceDirected::do_mapping(placement_hilbert);
    Placement placement_random = random_mapping.do_mapping(machine, net);
    Placement placement_zigzag = zig_zag.do_mapping(machine, net);
    Placement placement_circle = circle.do_mapping(machine, net);
    //Placement placement_FD = ForceDirected::do_mapping(placement_random);
/*
    int now_code = 0;
    for (int i = 0; i < size; ++i){
        for (int j = 0; j < size; ++j){
            printf("%d ", placement_hilbert.index[now_code]);
            now_code++;
        }
        printf("\n");
    }
*/

    double cost_random, cost_zigzag, cost_circle, cost_hilbert, cost_FD, cost_FD_hilbert;

    cost_random = Evaluator::evaluate(placement_random);
    cost_zigzag = Evaluator::evaluate(placement_zigzag);
    cost_circle = Evaluator::evaluate(placement_circle);
    cost_hilbert = Evaluator::evaluate(placement_hilbert);
   // cost_FD = Evaluator::evaluate(placement_FD);
    cost_FD_hilbert = Evaluator::evaluate(placement_FD_hilbert);
    std::cout << "hilbert:" << cost_hilbert << std::endl;
    std::cout << "random:" << cost_random << std::endl;
    std::cout << "zigzag:" << cost_zigzag << std::endl;
    std::cout << "circle:" << cost_circle << std::endl;
   // std::cout << "FD:" << cost_FD << std::endl;
    std::cout << "hilbert + FD:" << cost_FD_hilbert << std::endl;
}

void test2(){
    int size = 8;
    Machine machine(size);
    auto net = Network::make_random_net(size, size);
    Hilbert hilbert;
    RandomMapping random_mapping;
    ForceDirected force_directed;
    Placement placement_hilbert = hilbert.do_mapping(machine, net);
    Placement placement_fd = ForceDirected::do_mapping(placement_hilbert);
    int a = 1;
}
void fun(double array [][4], int n){

    for (int i = 0; i < n; ++i){
        for (int j = 0; j < 4; ++j)
            printf("%f ", array[i][j]);
    }

}
void test3(int n){
    auto array = new double [n][4];
    for (int i = 0; i < n; ++i){
        for (int j = 0; j < 4; ++j)
            array[i][j] = j;
    }
    fun(array, n);
}
int main() {
    test1();
    return 0;
}
