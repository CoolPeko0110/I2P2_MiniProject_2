//
// Created by johnn on 2024/5/15.
//

#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Shovel.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"

const int Shovel::Price = 1;
Shovel::Shovel(float x, float y) :
    Tool("play/shovel.png", "play/shovel.png", x, y, 0, Price) {
}
