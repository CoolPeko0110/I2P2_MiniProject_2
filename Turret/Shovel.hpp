//
// Created by johnn on 2024/5/15.
//

#ifndef SHOVEL_HPP
#define SHOVEL_HPP
#include "Tool.hpp"

class Shovel: public Tool {
public:
    static const int Price;
    Shovel(float x, float y);
};
#endif //SHOVEL_HPP
