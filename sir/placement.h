//
// Created by Ouwen Jin on 2021/11/19.
//

#ifndef MAPPING_PLACEMENT_H
#define MAPPING_PLACEMENT_H

#include "machine.h"
#include "network.h"
#include "cstdlib"

struct Pos{
    int x,y;
    Pos (int x,int y):x(x),y(y){};
    Pos ():x(0),y(0){}
    bool operator == (const Pos &b) const{
        return x == b.x && y == b.y;
    }
};
inline int get_manhattan_dis(Pos u, Pos v){
    return abs(u.x - v.x) + abs(u.y - v.y);
}
inline int pos2code(Pos pos, int size_y){
    return pos.x * size_y + pos.y;
};
inline int pos2code(int x, int y, int size_y){
    return x * size_y + y;
};
inline Pos code2pos(int code, int size_y){
    return {code / size_y, code % size_y};
};
class Placement {
public:
    Placement(Machine &,Network &);
    ~Placement();

    Machine & machine;
    Network & network;
    int core_num;
    Pos * mapping;
    int * index;
    double time_spent{};


    inline int get_index(int code) const;
    inline int get_index(int x,int y) const;
    void build_index_from_mapping() const;
};


#endif //MAPPING_PLACEMENT_H
