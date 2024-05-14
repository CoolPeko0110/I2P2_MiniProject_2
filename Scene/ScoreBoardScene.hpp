//
// Created by johnn on 2024/5/12.
//

#ifndef SCOREBOARDSCENE_HPP
#define SCOREBOARDSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <memory>
#include <map>
#include "Engine/IScene.hpp"

class ScoreBoardScene final : public Engine::IScene {
private:
    int page = 0;
    int numofpage;
    std::multimap<int, std::multimap<std::string, std::string>> Score;
    std::multimap<int, std::string> Date;

public:

    explicit ScoreBoardScene() = default;

    void Initialize() override;

    void Terminate() override;

    void BackOnClick(int stage);

    void NextOnClick(int stage);

    void PrevOnClick(int stage);
};

#endif //SCOREBOARDSCENE_HPP