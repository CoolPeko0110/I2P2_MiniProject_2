//
// Created by coolpeko on 6/13/24.
//

#ifndef QUETIONSCENE_HPP
#define QUETIONSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>

#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"


class question_scene final : public Engine::IScene {
public:
    explicit question_scene() = default;
    void Terminate() override;
    void Initialize() override;
    void Update(float deltaTime) override;
    void PlayOnClick(int stage);
    int final_time;
    float reload = 1000000;
};

#endif //QUETIONSCENE_HPP
