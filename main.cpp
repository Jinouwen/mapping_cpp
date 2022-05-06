#include <iostream>
#include <utility>
#include "sir/machine.h"
#include "sir/network.h"
#include "algorithm/randommapping.h"
#include "algorithm/zigzag.h"
#include "algorithm/circle.h"
#include "utils/evaluator.h"
#include "algorithm/hilbert.h"
#include "algorithm/force_directed.h"
#include "algorithm/PSO.h"

#define DEFAULT_METHOD 0
#define LATENCY_METHOD 1
#define DEFAULT_METHOD_NEW 2
#define LATENCY_METHOD_NEW 3
void PSO_test(){
    int size = 16;
    Machine machine(size);
    auto net = Network::make_random_net(size, size);
    Hilbert hilbert;
    RandomMapping random_mapping;
    ForceDirected force_directed;
    PSO pso;
    Placement placement_random = random_mapping.do_mapping(machine, net);
    Placement placement_hilbert = hilbert.do_mapping(machine, net);
    Placement placement_fd = force_directed.do_mapping(placement_hilbert);
    Placement placement_pso = pso.do_mapping(machine, net);
    auto cost_random = Evaluator::weighted_length_total(placement_random);
    auto cost_fd = Evaluator::weighted_length_total(placement_fd);
    auto cost_pso = Evaluator::weighted_length_total(placement_pso);
    printf("random cost: %lf\n", cost_random);
    printf("fd cost: %lf\n", cost_fd);
    printf("pso cost: %lf\n", cost_pso);
}

void test2(){
    int size = 8;
    Machine machine(size);
    auto net = Network::make_random_net(size, size);
    Hilbert hilbert;
    RandomMapping random_mapping;
    ForceDirected force_directed;
    Placement placement_hilbert = hilbert.do_mapping(machine, net);
    Placement placement_fd = force_directed.do_mapping(placement_hilbert);
    int a = 1;
}

void test3_old(int _size = 0, int method = 0){
    std::cout << std::endl << std::endl << std::endl;
    int size = _size ? _size : 256;
    Machine machine(size);
    int core_num = size * size;
    printf("core num:%d\n", core_num);
    int layer_num, node_per_layer;
    node_per_layer = size;
    layer_num = core_num / node_per_layer;
    auto net = Network::make_random_conv_net(layer_num, node_per_layer, 4);
    printf("layer num: %d, node per layer: %d\n",layer_num, node_per_layer);
    Hilbert hilbert;
    RandomMapping random_mapping;
    ZigZag zig_zag;
    Circle circle;
    PSO pso;
    ForceDirected force_directed;

    Placement placement_hilbert = hilbert.do_mapping(machine, net);
    Placement placement_random = random_mapping.do_mapping(machine, net);
    Placement placement_zigzag = zig_zag.do_mapping(machine, net);
    Placement placement_circle = circle.do_mapping(machine, net);
    Placement placement_FD_hilbert_cost = force_directed.do_mapping(placement_hilbert, DEFAULT_METHOD);
    Placement placement_FD_cost = force_directed.do_mapping(placement_random, DEFAULT_METHOD);
    Placement placement_FD_longest = force_directed.do_mapping(placement_random, LATENCY_METHOD_NEW);
    Placement placement_FD_hilbert_longest = force_directed.do_mapping(placement_hilbert, LATENCY_METHOD_NEW);
    Placement placement_pso = pso.do_mapping(machine, net);

    double cost_random, cost_zigzag, cost_circle, cost_hilbert, cost_FD_cost,
            cost_FD_hilbert_cost, cost_FD_hilbert_longest, cost_FD_longest, cost_pso;


    cost_random = Evaluator::weighted_length_total(placement_random);
    cost_zigzag = Evaluator::weighted_length_total(placement_zigzag);
    cost_circle = Evaluator::weighted_length_total(placement_circle);
    cost_hilbert = Evaluator::weighted_length_total(placement_hilbert);
    cost_FD_cost = Evaluator::weighted_length_total(placement_FD_cost);
    cost_FD_longest = Evaluator::weighted_length_total(placement_FD_longest);
    cost_FD_hilbert_cost = Evaluator::weighted_length_total(placement_FD_hilbert_cost);
    cost_FD_hilbert_longest = Evaluator::weighted_length_total(placement_FD_hilbert_longest);
    cost_pso = Evaluator::weighted_length_total(placement_pso);
    std::cout << "----------------------cost---------------------" << std::endl;
    std::cout << "hilbert cost:" << cost_hilbert << std::endl;
    std::cout << "random cost:" << cost_random << std::endl;
    std::cout << "zigzag cost:" << cost_zigzag << std::endl;
    std::cout << "circle cost:" << cost_circle << std::endl;
    std::cout << "FD_cost cost:" << cost_FD_cost << std::endl;
    std::cout << "FD_longest cost" << cost_FD_longest << std::endl;
    std::cout << "hilbert_FD_cost cost:" << cost_FD_hilbert_cost << std::endl;
    std::cout << "hilbert_FD_longest cost:" << cost_FD_hilbert_longest << std::endl;
    std::cout << "pso cost:" << cost_pso << std::endl;

    double longest_random, longest_zigzag, longest_circle, longest_hilbert,
            longest_FD_cost, longest_FD_hilbert_cost, longest_FD_longest, longest_FD_hilbert_longest,
            longest_pso;
    longest_random = Evaluator::longest_path(placement_random);
    longest_zigzag = Evaluator::longest_path(placement_zigzag);
    longest_circle = Evaluator::longest_path(placement_circle);
    longest_hilbert = Evaluator::longest_path(placement_hilbert);
    longest_FD_cost = Evaluator::longest_path(placement_FD_cost);
    longest_FD_hilbert_cost = Evaluator::longest_path(placement_FD_hilbert_cost);
    longest_FD_longest = Evaluator::longest_path(placement_FD_longest);
    longest_FD_hilbert_longest = Evaluator::longest_path(placement_FD_hilbert_longest);
    longest_pso = Evaluator::longest_path(placement_pso);
    std::cout << "------------------------longest spiking path-----------------------" << std::endl;
    std::cout << "hilbert longest:" << longest_hilbert << std::endl;
    std::cout << "random longest:" << longest_random << std::endl;
    std::cout << "zigzag longest:" << longest_zigzag << std::endl;
    std::cout << "circle longest:" << longest_circle << std::endl;
    std::cout << "FD_cost longest:" << longest_FD_cost << std::endl;
    std::cout << "FD_longest longest" << longest_FD_longest << std::endl;
    std::cout << "hilbert_FD_cost longest:" << longest_FD_hilbert_cost << std::endl;
    std::cout << "hilbert_FD_longest longest:" << longest_FD_hilbert_longest << std::endl;
    std::cout << "pso longest:" << longest_pso <<std::endl;




    std::pair<double, double > congestion_random, congestion_zigzag, congestion_circle,
            congestion_FD_hilbert_cost, congestion_FD_cost, congestion_FD_longest,
            congestion_FD_hilbert_longest, congestion_pso;

    congestion_random = Evaluator::congestion(placement_random);
    congestion_zigzag = Evaluator::congestion(placement_zigzag);
    congestion_circle = Evaluator::congestion(placement_circle);
    congestion_FD_hilbert_cost = Evaluator::congestion(placement_FD_hilbert_cost);
    congestion_FD_cost = Evaluator::congestion(placement_FD_cost);
    congestion_FD_hilbert_longest = Evaluator::congestion(placement_FD_hilbert_longest);
    congestion_FD_longest = Evaluator::congestion(placement_FD_longest);
    congestion_pso = Evaluator::congestion(placement_pso);
    std::cout << "----------------------------congestion----------------------------------" << std::endl;
    std::cout << "random average congestion: "<< congestion_random.first<< std::endl;
    std::cout << "zigzag average congestion: "<< congestion_zigzag.first<< std::endl;
    std::cout << "circle average congestion: "<< congestion_circle.first<< std::endl;
    std::cout << "FD_hilbert_cost average congestion: "<< congestion_FD_hilbert_cost.first<< std::endl;
    std::cout << "FD_cost average congestion: "<< congestion_FD_cost.first<< std::endl;
    std::cout << "FD_longest average congestion: "<< congestion_FD_longest.first<< std::endl;
    std::cout << "FD_hilbert_longest average congestion: "<< congestion_FD_hilbert_longest.first<< std::endl;
    std::cout << "pso average congestion:" << congestion_pso.first << std::endl;
    std::cout << "random max congestion:" <<  congestion_random.second<< std::endl;
    std::cout << "zigzag max congestion:" <<  congestion_zigzag.second<< std::endl;
    std::cout << "circle max congestion:" <<  congestion_circle.second<< std::endl;
    std::cout << "FD_hilbert_cost max congestion:" <<  congestion_FD_hilbert_cost.second<< std::endl;
    std::cout << "FD_cost max congestion:" <<  congestion_FD_cost.second<< std::endl;
    std::cout << "FD_longest max congestion:" <<  congestion_FD_longest.second<< std::endl;
    std::cout << "FD_hilbert_longest max congestion:" <<  congestion_FD_hilbert_longest.second<< std::endl;
    std::cout << "pso max congestion" << congestion_pso.second << std::endl;

    std::cout << "*******************************************************************" << std::endl << std::endl;



}
void major_test(Machine &machine, Network &net, std::string name,int opt = 0){
    /* opt
     * 1: dont run PSO
     */
    std::cerr << std::endl << std::endl << std::endl;
    Hilbert hilbert;
    RandomMapping random_mapping;
    ZigZag zig_zag;
    Circle circle;
    PSO pso;
    ForceDirected force_directed;
    Evaluator evaluator(name);

    std::cerr << "mapping: " <<name<< std::endl;

    Placement placement_hilbert = hilbert.do_mapping(machine, net);
    Placement placement_random = random_mapping.do_mapping(machine, net);
    Placement placement_zigzag = zig_zag.do_mapping(machine, net);
    Placement placement_circle = circle.do_mapping(machine, net);
    Placement placement_FD_hilbert_cost = force_directed.do_mapping(placement_hilbert, DEFAULT_METHOD);
    Placement placement_FD_cost = force_directed.do_mapping(placement_random, DEFAULT_METHOD);
    Placement placement_FD_longest = force_directed.do_mapping(placement_random, LATENCY_METHOD_NEW);
    Placement placement_FD_hilbert_longest = force_directed.do_mapping(placement_hilbert, LATENCY_METHOD_NEW);

    evaluator.add_task(placement_hilbert, "Hilbert");
    evaluator.add_task(placement_random, "Random mapping");
    evaluator.add_task(placement_zigzag, "Zig-zag");
    evaluator.add_task(placement_circle, "Circle");
    evaluator.add_task(placement_FD_hilbert_cost, "Hilbert_FD_default");
    evaluator.add_task(placement_FD_cost, "FD_default");
    evaluator.add_task(placement_FD_longest, "FD_latency");
    evaluator.add_task(placement_FD_hilbert_longest, "Hilbert_FD_latency");
    if (! (opt&1)){
        Placement placement_pso = pso.do_mapping(machine, net);
        evaluator.add_task(placement_pso, "PSO");
    }

    evaluator.evaluate();

}
void test3_old2(Machine &machine, Network &net){
    std::cout << std::endl << std::endl << std::endl;
    Hilbert hilbert;
    RandomMapping random_mapping;
    ZigZag zig_zag;
    Circle circle;
    PSO pso;
    ForceDirected force_directed;

    Placement placement_hilbert = hilbert.do_mapping(machine, net);
    Placement placement_random = random_mapping.do_mapping(machine, net);
    Placement placement_zigzag = zig_zag.do_mapping(machine, net);
    Placement placement_circle = circle.do_mapping(machine, net);
    Placement placement_FD_hilbert_cost = force_directed.do_mapping(placement_hilbert, DEFAULT_METHOD);
    Placement placement_FD_cost = force_directed.do_mapping(placement_random, DEFAULT_METHOD);
    Placement placement_FD_longest = force_directed.do_mapping(placement_random, LATENCY_METHOD_NEW);
    Placement placement_FD_hilbert_longest = force_directed.do_mapping(placement_hilbert, LATENCY_METHOD_NEW);
    Placement placement_pso = pso.do_mapping(machine, net);

    double cost_random, cost_zigzag, cost_circle, cost_hilbert, cost_FD_cost,
           cost_FD_hilbert_cost, cost_FD_hilbert_longest, cost_FD_longest, cost_pso;


    cost_random = Evaluator::weighted_length_total(placement_random);
    cost_zigzag = Evaluator::weighted_length_total(placement_zigzag);
    cost_circle = Evaluator::weighted_length_total(placement_circle);
    cost_hilbert = Evaluator::weighted_length_total(placement_hilbert);
    cost_FD_cost = Evaluator::weighted_length_total(placement_FD_cost);
    cost_FD_longest = Evaluator::weighted_length_total(placement_FD_longest);
    cost_FD_hilbert_cost = Evaluator::weighted_length_total(placement_FD_hilbert_cost);
    cost_FD_hilbert_longest = Evaluator::weighted_length_total(placement_FD_hilbert_longest);
    cost_pso = Evaluator::weighted_length_total(placement_pso);
    std::cout << "----------------------cost---------------------" << std::endl;
    std::cout << "hilbert cost:" << cost_hilbert << std::endl;
    std::cout << "random cost:" << cost_random << std::endl;
    std::cout << "zigzag cost:" << cost_zigzag << std::endl;
    std::cout << "circle cost:" << cost_circle << std::endl;
    std::cout << "FD_cost cost:" << cost_FD_cost << std::endl;
    std::cout << "FD_longest cost" << cost_FD_longest << std::endl;
    std::cout << "hilbert_FD_cost cost:" << cost_FD_hilbert_cost << std::endl;
    std::cout << "hilbert_FD_longest cost:" << cost_FD_hilbert_longest << std::endl;
    std::cout << "pso cost:" << cost_pso << std::endl;

    double longest_random, longest_zigzag, longest_circle, longest_hilbert,
           longest_FD_cost, longest_FD_hilbert_cost, longest_FD_longest, longest_FD_hilbert_longest,
           longest_pso;
    longest_random = Evaluator::longest_path(placement_random);
    longest_zigzag = Evaluator::longest_path(placement_zigzag);
    longest_circle = Evaluator::longest_path(placement_circle);
    longest_hilbert = Evaluator::longest_path(placement_hilbert);
    longest_FD_cost = Evaluator::longest_path(placement_FD_cost);
    longest_FD_hilbert_cost = Evaluator::longest_path(placement_FD_hilbert_cost);
    longest_FD_longest = Evaluator::longest_path(placement_FD_longest);
    longest_FD_hilbert_longest = Evaluator::longest_path(placement_FD_hilbert_longest);
    longest_pso = Evaluator::longest_path(placement_pso);
    std::cout << "------------------------longest spiking path-----------------------" << std::endl;
    std::cout << "hilbert longest:" << longest_hilbert << std::endl;
    std::cout << "random longest:" << longest_random << std::endl;
    std::cout << "zigzag longest:" << longest_zigzag << std::endl;
    std::cout << "circle longest:" << longest_circle << std::endl;
    std::cout << "FD_cost longest:" << longest_FD_cost << std::endl;
    std::cout << "FD_longest longest" << longest_FD_longest << std::endl;
    std::cout << "hilbert_FD_cost longest:" << longest_FD_hilbert_cost << std::endl;
    std::cout << "hilbert_FD_longest longest:" << longest_FD_hilbert_longest << std::endl;
    std::cout << "pso longest:" << longest_pso <<std::endl;




    std::pair<double, double > congestion_random, congestion_zigzag, congestion_circle,
                               congestion_FD_hilbert_cost, congestion_FD_cost, congestion_FD_longest,
                               congestion_FD_hilbert_longest, congestion_pso;

    congestion_random = Evaluator::congestion(placement_random);
    congestion_zigzag = Evaluator::congestion(placement_zigzag);
    congestion_circle = Evaluator::congestion(placement_circle);
    congestion_FD_hilbert_cost = Evaluator::congestion(placement_FD_hilbert_cost);
    congestion_FD_cost = Evaluator::congestion(placement_FD_cost);
    congestion_FD_hilbert_longest = Evaluator::congestion(placement_FD_hilbert_longest);
    congestion_FD_longest = Evaluator::congestion(placement_FD_longest);
    congestion_pso = Evaluator::congestion(placement_pso);
    std::cout << "----------------------------congestion----------------------------------" << std::endl;
    std::cout << "random average congestion: "<< congestion_random.first<< std::endl;
    std::cout << "zigzag average congestion: "<< congestion_zigzag.first<< std::endl;
    std::cout << "circle average congestion: "<< congestion_circle.first<< std::endl;
    std::cout << "FD_hilbert_cost average congestion: "<< congestion_FD_hilbert_cost.first<< std::endl;
    std::cout << "FD_cost average congestion: "<< congestion_FD_cost.first<< std::endl;
    std::cout << "FD_longest average congestion: "<< congestion_FD_longest.first<< std::endl;
    std::cout << "FD_hilbert_longest average congestion: "<< congestion_FD_hilbert_longest.first<< std::endl;
    std::cout << "pso average congestion:" << congestion_pso.first << std::endl;
    std::cout << "random max congestion:" <<  congestion_random.second<< std::endl;
    std::cout << "zigzag max congestion:" <<  congestion_zigzag.second<< std::endl;
    std::cout << "circle max congestion:" <<  congestion_circle.second<< std::endl;
    std::cout << "FD_hilbert_cost max congestion:" <<  congestion_FD_hilbert_cost.second<< std::endl;
    std::cout << "FD_cost max congestion:" <<  congestion_FD_cost.second<< std::endl;
    std::cout << "FD_longest max congestion:" <<  congestion_FD_longest.second<< std::endl;
    std::cout << "FD_hilbert_longest max congestion:" <<  congestion_FD_hilbert_longest.second<< std::endl;
    std::cout << "pso max congestion" << congestion_pso.second << std::endl;

    std::cout << "*******************************************************************" << std::endl << std::endl;



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
    int test_list[6] = {2, 4, 8, 16, 32, 64};
    for(int i : test_list)
        test3_old(i, method);
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
    ForceDirected force_directed;

    auto placement_random = random_mapping.do_mapping(machine, net);
    auto placement_hilbert = hilbert.do_mapping(machine, net);
    auto placement_FD_cost = force_directed.do_mapping(placement_hilbert, DEFAULT_METHOD_NEW);
    auto placement_FD_latency = force_directed.do_mapping(placement_hilbert, LATENCY_METHOD);

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

void test6(){

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
    ForceDirected force_directed;
    auto placement_random = random_mapping.do_mapping(machine, net);
    auto placement_hilbert = hilbert.do_mapping(machine, net);
    auto placement_FD_cost = force_directed.do_mapping(placement_hilbert, DEFAULT_METHOD_NEW);
    auto placement_FD_latency = force_directed.do_mapping(placement_hilbert, LATENCY_METHOD_NEW);

    auto cost_hilbert = Evaluator::weighted_length_total(placement_hilbert);
    auto cost_random = Evaluator::weighted_length_total(placement_random);
    auto cost_FD_cost = Evaluator::weighted_length_total(placement_FD_cost);
    auto cost_FD_latency = Evaluator::weighted_length_total(placement_FD_latency);
    auto longest_hilbert = Evaluator::longest_path(placement_hilbert);
    auto longest_random = Evaluator::longest_path(placement_random);
    auto longest_FD_cost = Evaluator::longest_path(placement_FD_cost);
    auto longest_FD_latency = Evaluator::longest_path(placement_FD_latency);
    auto congestion_hilbert = Evaluator::congestion(placement_hilbert);
    auto congestion_random = Evaluator::congestion(placement_random);
    auto congestion_FD_cost = Evaluator::congestion(placement_FD_cost);
    auto congestion_FD_latency = Evaluator::congestion(placement_FD_latency);

    std::cout << "random cost:" << cost_random << std::endl;
    std::cout << "hilbert cost:" << cost_hilbert << std::endl;
    std::cout << "FD_cost cost:" << cost_FD_cost << std::endl;
    std::cout << "FD_latency cost:" << cost_FD_latency << std::endl;
    std::cout << "random longest path:" << longest_random << std::endl;
    std::cout << "hilbert longest path:" << longest_hilbert << std::endl;
    std::cout << "FD_cost longest path:" << longest_FD_cost << std::endl;
    std::cout << "FD_latency longest path:" << longest_FD_latency << std::endl;
    std::cout << "random congestion:" << congestion_random.first << std::endl;
    std::cout << "hilbert congestion:" << congestion_hilbert.first << std::endl;
    std::cout << "FD_cost congestion:" << congestion_FD_cost.first << std::endl;
    std::cout << "FD_latency congestion" << congestion_FD_latency.first << std::endl;
}
void code_test(){
    auto net = Network::load_from_files("/tmp/pycharm_project_498/snntoolbox_applications/models/inceptionV3");

    std::cout << "node_num:" << net.node_num << std::endl;
}
void inceptionV3(){
    auto machine = Machine(60);
    auto net = Network::load_from_files("/tmp/pycharm_project_498/snntoolbox_applications/models/inceptionV3");
    test3_old2(machine, net);
}

void test_evaluator(){
    Machine machine(4);
    Network net = Network::make_random_conv_net(4,4,4);
    major_test(machine,net,"test");
}
void full_connection(){
    int test_list[3] = {256,512,1024};
    for(int i : test_list){
        Machine machine(i);
        auto network = Network::make_random_net(i,i,0);
        major_test(machine,network,"DNN_"+std::to_string(i)+"_"+ std::to_string(i),1);
    }

}
int main() {

    full_connection();
    return 0;
}
