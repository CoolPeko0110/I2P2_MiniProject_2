//
// Created by coolpeko on 6/12/24.
//

#ifndef SECONDSCENE_HPP
#define SECONDSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>

#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"

class Turret;
class Tool;
namespace Engine {
    class Group;
    class Image;
    class Label;
    class Sprite;
}

class second_scene final : public Engine::IScene {
public:
    explicit second_scene() = default;
    void Terminate() override;
    void Initialize() override;
    void Update(float deltaTime) override;
    void Draw() const override;
    float reload = 0.1;
    Engine::Image* secondblood;
    Engine::Image* black;
};


#endif //SECONDSCENE_HPP
