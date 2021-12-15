#include <iostream>
#include "sir/machine.h"
#include "sir/network.h"
#include "algorithm/randommapping.h"
#include "algorithm/zigzag.h"
#include "algorithm/circle.h"
#include "utils/evaluator.h"
#include "algorithm/hilbert.h"
#include "algorithm/force_directed.h"

#define DEFAULT_METHOD 0
#define LATENCY_METHOD 1
#define DEFAULT_METHOD_NEW 2
#define LATENCY_METHOD_NEW 3
void test1(){

    int size = 128;
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

    cost_random = Evaluator::weighted_length_total(placement_random);
    cost_zigzag = Evaluator::weighted_length_total(placement_zigzag);
    cost_circle = Evaluator::weighted_length_total(placement_circle);
    cost_hilbert = Evaluator::weighted_length_total(placement_hilbert);
   // cost_FD = Evaluator::evaluate(placement_FD);
    cost_FD_hilbert = Evaluator::weighted_length_total(placement_FD_hilbert);
    std::cout << "hilbert:" << cost_hilbert << std::endl;
    std::cout << "random:" << cost_random << std::endl;
    std::cout << "zigzag:" << cost_zigzag << std::endl;
    std::cout << "circle:" << cost_circle << std::endl;
   // std::cout << "FD:" << cost_FD << std::endl;
    std::cout << "hilbert + FD:" << cost_FD_hilbert << std::endl;

    double longest_random, longest_zigzag, longest_circle, longest_hilbert, longest_FD, longest_FD_hilbert;
    longest_random = Evaluator::longest_path(placement_random);
    longest_zigzag = Evaluator::longest_path(placement_zigzag);
    longest_circle = Evaluator::longest_path(placement_circle);
    longest_hilbert = Evaluator::longest_path(placement_hilbert);
    // longest_FD = Evaluator::longest_path(placement_FD);
    longest_FD_hilbert = Evaluator::longest_path(placement_FD_hilbert);
    std::cout << "hilbert longest:" << longest_hilbert << std::endl;
    std::cout << "random longest:" << longest_random << std::endl;
    std::cout << "zigzag longest:" << longest_zigzag << std::endl;
    std::cout << "circle longest:" << longest_circle << std::endl;
    // std::cout << "FD:" << longest_FD << std::endl;
    std::cout << "hilbert + FD longest:" << longest_FD_hilbert << std::endl;

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
void test3(int _size = 0, int method = 0){
    std::cout << std::endl << std::endl << std::endl;
    int size = _size ? _size : 256;
    Machine machine(size);
    int core_num = size * size;
    printf("core num:%d\n", core_num);
    int layer_num, node_per_layer;
    node_per_layer = size;
    layer_num = core_num / node_per_layer;
    auto net = Network::make_random_conv_net(layer_num, node_per_layer,4);
    printf("layer num: %d, node per layer: %d\n",layer_num, node_per_layer);
    Hilbert hilbert;
    RandomMapping random_mapping;
    ZigZag zig_zag;
    Circle circle;
    Placement placement_hilbert = hilbert.do_mapping(machine, net);
    Placement placement_random = random_mapping.do_mapping(machine, net);
    Placement placement_zigzag = zig_zag.do_mapping(machine, net);
    Placement placement_circle = circle.do_mapping(machine, net);
    Placement placement_FD_hilbert_cost = ForceDirected::do_mapping(placement_hilbert, DEFAULT_METHOD);
    Placement placement_FD_cost = ForceDirected::do_mapping(placement_random, DEFAULT_METHOD);
    Placement placement_FD_longest = ForceDirected::do_mapping(placement_random, LATENCY_METHOD_NEW);
    Placement placement_FD_hilbert_longest = ForceDirected::do_mapping(placement_hilbert, LATENCY_METHOD_NEW);
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

    double cost_random, cost_zigzag, cost_circle, cost_hilbert, cost_FD_cost, cost_FD_hilbert_cost, cost_FD_hilbert_longest, cost_FD_longest;

    cost_random = Evaluator::weighted_length_total(placement_random);
    cost_zigzag = Evaluator::weighted_length_total(placement_zigzag);
    cost_circle = Evaluator::weighted_length_total(placement_circle);
    cost_hilbert = Evaluator::weighted_length_total(placement_hilbert);
    cost_FD_cost = Evaluator::weighted_length_total(placement_FD_cost);
    cost_FD_longest = Evaluator::weighted_length_total(placement_FD_longest);
    cost_FD_hilbert_cost = Evaluator::weighted_length_total(placement_FD_hilbert_cost);
    cost_FD_hilbert_longest = Evaluator::weighted_length_total(placement_FD_hilbert_longest);
    std::cout << "hilbert cost:" << cost_hilbert << std::endl;
    std::cout << "random cost:" << cost_random << std::endl;
    std::cout << "zigzag cost:" << cost_zigzag << std::endl;
    std::cout << "circle cost:" << cost_circle << std::endl;
    std::cout << "FD_cost cost:" << cost_FD_cost << std::endl;
    std::cout << "FD_longest cost" << cost_FD_longest << std::endl;
    std::cout << "hilbert_FD_cost cost:" << cost_FD_hilbert_cost << std::endl;
    std::cout << "hilbert_FD_longest cost:" << cost_FD_hilbert_longest << std::endl;


    double longest_random, longest_zigzag, longest_circle, longest_hilbert, longest_FD_cost, longest_FD_hilbert_cost, longest_FD_longest, longest_FD_hilbert_longest;
    longest_random = Evaluator::longest_path(placement_random);
    longest_zigzag = Evaluator::longest_path(placement_zigzag);
    longest_circle = Evaluator::longest_path(placement_circle);
    longest_hilbert = Evaluator::longest_path(placement_hilbert);
    longest_FD_cost = Evaluator::longest_path(placement_FD_cost);
    longest_FD_hilbert_cost = Evaluator::longest_path(placement_FD_hilbert_cost);
    longest_FD_longest = Evaluator::longest_path(placement_FD_longest);
    longest_FD_hilbert_longest = Evaluator::longest_path(placement_FD_hilbert_longest);

    std::cout << "hilbert longest:" << longest_hilbert << std::endl;
    std::cout << "random longest:" << longest_random << std::endl;
    std::cout << "zigzag longest:" << longest_zigzag << std::endl;
    std::cout << "circle longest:" << longest_circle << std::endl;
    std::cout << "FD_cost longest:" << longest_FD_cost << std::endl;
    std::cout << "FD_longest longest" << longest_FD_longest << std::endl;
    std::cout << "hilbert_FD_cost longest:" << longest_FD_hilbert_cost << std::endl;
    std::cout << "hilbert_FD_longest longest:" << longest_FD_hilbert_longest << std::endl;
}
void fun(double array [][4], int n){
    for (int i = 0; i < n; ++i){
        for (int j = 0; j < 4; ++j)
            printf("%f ", array[i][j]);
    }

}
int getnum(const Network & net){
    int n = net.node_num;
    int ans = 0;
    for(int i = 0; i < n; ++i){
        ans += net.edges[i].size(); // NOLINT(cppcoreguidelines-narrowing-conversions)
    }
    return ans;
}
void test4(int method){
    int test_list[6] = {32, 64, 128, 256, 512, 1024};
    for(int i : test_list)
        test3(i, method);
}

void test5(){
    int size = 128;
    Machine machine(size);
    int core_num = size * size;
    printf("core num:%d\n", core_num);
    int layer_num, node_per_layer;
    node_per_layer = size;
    layer_num = core_num / node_per_layer;
    auto net = Network::make_random_conv_net(layer_num, node_per_layer, 4);
    printf("layer num: %d, node per layer: %d\n",layer_num, node_per_layer);
    RandomMapping random_mapping;
    Hilbert hilbert;
    auto placement_random = random_mapping.do_mapping(machine, net);
    auto placement_hilbert = hilbert.do_mapping(machine, net);
    auto placement_FD_cost = ForceDirected::do_mapping(placement_hilbert, DEFAULT_METHOD_NEW);
    auto placement_FD_latency = ForceDirected::do_mapping(placement_hilbert, LATENCY_METHOD);

    auto cost_hilbert = Evaluator::weighted_length_total(placement_hilbert);
    auto cost_random = Evaluator::weighted_length_total(placement_random);
    auto cost_FD_cost = Evaluator::weighted_length_total(placement_FD_cost);
    auto cost_FD_latency = Evaluator::weighted_length_total(placement_FD_latency);
    auto longest_hilbert = Evaluator::longest_path(placement_hilbert);
    auto longest_random = Evaluator::longest_path(placement_random);
    auto longest_FD_cost = Evaluator::longest_path(placement_FD_cost);
    auto longest_FD_latency = Evaluator::longest_path(placement_FD_latency);


    std::cout << "random cost:" << cost_random << std::endl;
    std::cout << "hilbert cost:" << cost_hilbert << std::endl;
    std::cout << "FD_cost cost:" << cost_FD_cost << std::endl;
    std::cout << "FD_latency cost:" << cost_FD_latency << std::endl;
    std::cout << "random longest path:" << longest_random << std::endl;
    std::cout << "hilbert longest path:" << longest_hilbert << std::endl;
    std::cout << "FD_cost longest path:" << longest_FD_cost << std::endl;
    std::cout << "FD_latency longest path:" << longest_FD_latency << std::endl;
}

void net_test(){
    int node0 = 0;
    int node1 = 0;
    auto net0 = Network::make_random_net(256 * 256 / 4, 4);
    auto net1 = Network::make_random_conv_net(256, 256, 4);
    printf("%d %d",getnum(net0), getnum(net1));
}
int main() {
    test4(LATENCY_METHOD);
}
