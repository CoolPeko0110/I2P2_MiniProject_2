#ifndef STAGESELECTSCENE_HPP
#define STAGESELECTSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"

class StageSelectScene final : public Engine::IScene {
public:
	explicit StageSelectScene() = default;
	void Initialize() override;
	void Terminate() override;
	void PlayOnClick(int stage);
	void Update(float deltaTime) override;
	float reload = 1000000;
};

#endif // STAGESELECTSCENE_HPP
