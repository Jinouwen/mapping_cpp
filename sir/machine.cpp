//
// Created by Ouwen Jin on 2021/11/19.
//

#include "machine.h"

Machine::Machine(int _size_x,int _size_y):size_x(_size_x),size_y(_size_y) {
    if (size_y == -1)
        size_y = size_x;
    core_num = size_x * size_y;
}
