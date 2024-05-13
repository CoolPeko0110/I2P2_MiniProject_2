//
// Created by johnn on 2024/5/13.
//

#ifndef SHAPATURRET_HPP
#define SHAPATURRET_HPP
#include "Turret.hpp"

class ShapaTurret: public Turret {
public:
    static const int Price;
    ShapaTurret(float x, float y);
    void CreateBullet() override;
};

#endif //SHAPATURRET_HPP
