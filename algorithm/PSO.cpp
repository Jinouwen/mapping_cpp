//
// Created by Ouwen Jin on 2022/1/6.
//

#include "PSO.h"
#include <iostream>
#include <chrono>


inline static Placement build_placement_from_particle(Machine & machine, Network & network,const int * encode_particle, bool if_build_index=false){
    int dimension = machine.core_num;
    bool used[dimension];
    for (int i = 0; i < dimension; ++i){
        used[i] = false;
    }
    Placement placement(machine, network);
    for(long long i = dimension -1; i >= 0; --i){
        int now_rank = encode_particle[i];
        int now_pos = -1;
        while(now_rank) {
            now_pos++;
            while (used[now_pos])
                now_pos++;
            now_rank--;
        }
        used[now_pos] = true;
        placement.mapping[i] = code2pos(now_pos, machine.size_y);
    }
    if(if_build_index)
        placement.build_index_from_mapping();
    return placement;
}

static double cal_fitness(Machine & machine, Network & network, int* encode_particle){
    auto placement = build_placement_from_particle(machine, network, encode_particle);
    double now_cost = 0;
    std::vector<Edge> * & edges = placement.network.edges;
    int node_num = placement.network.node_num;
    for (int i = 0; i < node_num; ++i){
        for (auto j:edges[i]){
            if (j.is_reverse) continue;
            int from = i;
            int to = j.to;
            Pos pos_u = placement.mapping[from];
            Pos pos_v = placement.mapping[to];
            int dis = get_manhattan_dis(pos_u, pos_v);
            now_cost += j.weight * dis;
        }
    }
    return now_cost;


}

Placement PSO::do_mapping(Machine & machine, Network & network) {
    printf("mapping start, algorithm: PSO\n");
    auto start = std::chrono::system_clock::now();
    double W = 0.9;
    double C1 = 2;
    double C2 = 2;
    int particle_num = network.node_num * 2;
    int iteration_num = network.node_num * 2;
    int dimension = machine.core_num;
    printf("particle num:%d dimension:%d\n", particle_num, dimension);
    //----------initialize----------------------------------------------
    std::default_random_engine e(1); // NOLINT(cert-msc51-cpp)
    auto particles = new int * [particle_num];
    auto velocities = new double * [particle_num];
    auto pbest = new int * [particle_num];
    double pbest_fitness[particle_num];
    for (int i = 0; i < particle_num; ++i){
        particles[i] = new int[dimension];
        velocities[i] = new double[dimension];
        pbest[i] = new int[dimension];
    }
    std::uniform_int_distribution<int> u_int(1,dimension * 20);
    std::uniform_real_distribution<double> u_real_NP1(-1, 1);
    for (int i = 0; i < particle_num; ++i){
        for (int j = 0; j < dimension; ++j){
            particles[i][j] = u_int(e) % (j + 1) + 1;
            velocities[i][j] = u_real_NP1(e);
        }
    }
    double fitness[particle_num];
    double gbest_fitness = INFINITY;
    int gbest_id;
    int gbest[dimension];
    for (int i = 0; i < particle_num; ++i){
        //printf("%d\n",i);
        fitness[i] = cal_fitness(machine, network, particles[i]);
        if (fitness[i] < gbest_fitness){
            gbest_fitness = fitness[i];
            gbest_id = i;
        }
        for (int j = 0; j < dimension; ++j){
            pbest[i][j] = particles[i][j];
        }
        pbest_fitness[i] = fitness[i];
    }
    for (int j = 0; j < dimension; ++j){
        gbest[j] = particles[gbest_id][j];
    }
    //--------------optimization---------------------------------------------
    std::uniform_real_distribution<double> u_real_01 (0,1);
    for (int iteration_count = 0; iteration_count < iteration_num; ++iteration_count){
        printf("%d\n",iteration_count);
        //---------------------------------update particle----------------------------------
        for (int i = 0; i < dimension; i++){
            int d_max = i + 1;
            int d_min = 1;
            for (int j = 0; j < particle_num; j++){
                velocities[j][i] = W * velocities[j][i]
                           + C1 * u_real_01(e) * (pbest[j][i] - particles[j][i])
                           + C2 * u_real_01(e) * (gbest[i] - particles[j][i]);
                particles[j][i] = int(round(particles[j][i] + velocities[j][i]) + 0.1);
                particles[j][i] = particles[j][i] <= d_max ? particles[j][i] : d_max;
                particles[j][i] = particles[j][i] >= d_min ? particles[j][i] : d_min;
            }
        }
        //--------------------------------evaluate & update g/p best--------------------------------
        for (int i = 0; i < particle_num; ++i){
            fitness[i] = cal_fitness(machine, network, particles[i]);
            if(fitness[i] < pbest_fitness[i]){
                pbest_fitness[i] = fitness[i];
                for (int j = 0; j < dimension; ++j){
                    pbest[i][j] = particles[i][j];
                }
                if(fitness[i] < gbest_fitness){
                    gbest_fitness = fitness[i];
                    for (int j = 0; j < dimension; ++j){
                        gbest[j] = particles[i][j];
                    }
                }
            }
        }
    }
    //------------get output placement----------------------------
    auto placement = build_placement_from_particle(machine, network, gbest, true);
    //------------release memory----------------------------------
    for (int i = 0; i < particle_num; ++i) {
        delete[] particles[i];
        delete[] velocities[i];
        delete[] pbest[i];
    }

    delete[] particles;
    delete[] velocities;
    delete[] pbest;

    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    printf("duration: %f\n",double (duration.count())/std::chrono::milliseconds::period::den);
    return placement;
}
