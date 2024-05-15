//
// Created by johnn on 2024/5/15.
//

#ifndef SAVINGSCENE_HPP
#define SAVINGSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"

class SavingScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    enum TileType {
        TILE_DIRT,
        TILE_FLOOR,
        TILE_OCCUPIED,
    };
public:
    static const int MapWidth, MapHeight;
    std::vector<std::vector<TileType>> mapState;
    explicit SavingScene() = default;
    void Initialize() override;
    void Terminate() override;
    void BackOnClick(int stage);
};
#endif //SAVINGSCENE_HPP
