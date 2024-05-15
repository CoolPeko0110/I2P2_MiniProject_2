//
// Created by johnn on 2024/5/15.
//

#ifndef DIRT_HPP
#define DIRT_HPP
#include "Tool.hpp"

class Dirt: public Tool {
public:
    static const int Price;
    Dirt(float x, float y);
};
#endif //DIRT_HPP
