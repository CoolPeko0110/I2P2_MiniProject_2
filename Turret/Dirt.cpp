//
// Created by johnn on 2024/5/15.
//

#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Dirt.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"

const int Dirt::Price = 1;
Dirt::Dirt(float x, float y) :
    Tool("play/place.png", "play/place.png", x, y, 0, Price) {
}