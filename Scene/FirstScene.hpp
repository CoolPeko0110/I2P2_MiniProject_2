//
// Created by coolpeko on 6/12/24.
//

#ifndef FIRSTSCENE_HPP
#define FIRSTSCENE_HPP
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

class first_scene final : public Engine::IScene {
public:
    explicit first_scene() = default;
    void Terminate() override;
    void Initialize() override;
    void Update(float deltaTime) override;
    void Draw() const override;
    float reload = 5;
    Engine::Image* firstblood;
    Engine::Image* black;
};

#endif //FIRSTSCENE_HPP
