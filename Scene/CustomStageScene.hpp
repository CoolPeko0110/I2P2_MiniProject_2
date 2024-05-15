//
// Created by johnn on 2024/5/15.
//

#ifndef CUSTOMSTAGESCENE_HPP
#define CUSTOMSTAGESCENE_HPP
#include <allegro5/allegro_audio.h>
#include <list>
#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"

class Tool;
namespace Engine {
	class Group;
	class Image;
	class Label;
	class Sprite;
}  // namespace Engine

class CustomStageScene final : public Engine::IScene {
private:
	enum TileType {
		TILE_DIRT,
		TILE_FLOOR,
		TILE_OCCUPIED,
	};
	ALLEGRO_SAMPLE_ID bgmId;
public:
	static const std::vector<Engine::Point> directions;
	static const int MapWidth, MapHeight;
	static const int BlockSize;
	static const Engine::Point SpawnGridPoint;
	static const Engine::Point EndGridPoint;
	int MapId;
	// Map tiles.
	Group* TileMapGroup;
	Group* GroundEffectGroup;
	Group* DebugIndicatorGroup;
	Group* BulletGroup;
	Group* TowerGroup;
	Group* EnemyGroup;
	Group* EffectGroup;
	Group* UIGroup;
	Engine::Image* imgTarget;
	Engine::Sprite* dangerIndicator;
	std::vector<std::vector<TileType>> mapState;
	std::vector<std::vector<TileType>> tempMap;
	std::list<std::vector<int>> tempEnemy;
	std::vector<std::vector<int>> mapDistance;
	std::list<std::multimap<int, int>> undoactions;
	std::list<std::multimap<int, int>> doactions;
	Tool* previewTool;
	bool tooltype;
	bool save;
	static Engine::Point GetClientSize();
	explicit CustomStageScene() = default;
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Draw() const override;
	void OnMouseDown(int button, int mx, int my) override;
	void OnMouseMove(int mx, int my) override;
	void OnMouseUp(int button, int mx, int my) override;
	void ReadMap();
	void ConstructUI();
	void UIBtnClicked(int id);
	bool CheckSpaceValid(int x, int y);
	std::vector<std::vector<int>> CalculateBFSDistance();
};
#endif //CUSTOMSTAGESCENE_HPP
