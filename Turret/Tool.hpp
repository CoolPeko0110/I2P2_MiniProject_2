//
// Created by johnn on 2024/5/15.
//

#ifndef TOOL_HPP
#define TOOL_HPP
#include <allegro5/base.h>
#include <list>
#include <string>

#include "Engine/Sprite.hpp"

class Enemy;
class PlayScene;

class Tool: public Engine::Sprite {
protected:
    int price;
    float coolDown;
    Sprite imgBase;
    PlayScene* getPlayScene();

public:
    bool Enabled = true;
    bool Preview = false;
    Tool(std::string imgBase, std::string imgTurret, float x, float y, float radius, int price);
    void Update(float deltaTime) override;
    void Draw() const override;
    int GetPrice() const;
};
#endif //TOOL_HPP
