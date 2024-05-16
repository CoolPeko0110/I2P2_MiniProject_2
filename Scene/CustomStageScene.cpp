//
// Created by johnn on 2024/5/15.
//

#include <allegro5/allegro.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <vector>
#include <queue>
#include <string>
#include <memory>
#include <iostream>

#include "Engine/AudioHelper.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "Enemy/Enemy.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "UI/Component/Label.hpp"
#include "Turret/Shovel.hpp"
#include "Turret/Dirt.hpp"
#include "CustomStageScene.hpp"
#include "PlayScene.hpp"

#include "Turret/TurretButton.hpp"
#include "Turret/Tool.hpp"


const int CustomStageScene::MapWidth = 20, CustomStageScene::MapHeight = 13;
const int CustomStageScene::BlockSize = 64;
const Engine::Point CustomStageScene::SpawnGridPoint = Engine::Point(-1, 0);
const Engine::Point CustomStageScene::EndGridPoint = Engine::Point(MapWidth, MapHeight - 1);
extern int NOWMAP;

Engine::Point CustomStageScene::GetClientSize() {
	return Engine::Point(MapWidth * BlockSize, MapHeight * BlockSize);
}
void CustomStageScene::Initialize() {
	undoactions.clear();
	doactions.clear();
	// TODO: [HACKATHON-3-BUG-FIXED] (1/5): There's a bug in this file, which crashes the game when you lose. Try to find it.
	// TODO: [HACKATHON-3-BUG-FIXED] (2/5): Find out the cheat code to test.
    // TODO: [HACKATHON-3-BUG-FIXED] (2/5): It should generate a Plane, and add 10000 to the money, but it doesn't work now.
	save = true;
	mapState.clear();
	// Add groups from bottom to top.
	AddNewObject(TileMapGroup = new Group());
	AddNewObject(GroundEffectGroup = new Group());
	AddNewObject(DebugIndicatorGroup = new Group());
	AddNewObject(TowerGroup = new Group());
	AddNewObject(EnemyGroup = new Group());
	AddNewObject(BulletGroup = new Group());
	AddNewObject(EffectGroup = new Group());
	// Should support buttons.
	AddNewControlObject(UIGroup = new Group());
	ReadMap();
	mapDistance = CalculateBFSDistance();
	ConstructUI();
	imgTarget = new Engine::Image("play/target.png", 0, 0);
	imgTarget->Visible = false;
	UIGroup->AddNewObject(imgTarget);
	//bgmId = AudioHelper::PlayBGM("play.ogg");
}
void CustomStageScene::Terminate() {
		//AudioHelper::StopBGM(bgmId);
		if(save) {
			for (int i = 0; i < MapHeight; i++) {
				for (int j = 0; j < MapWidth; j++) {
					tempMap[i][j] = mapState[i][j];
				}
			}
			std::string filename = std::string("Resource/RAMmap") + ".txt";
			std::ofstream fout(filename);
			for (int i = 0; i < MapHeight; i++) {
				for (int j = 0; j < MapWidth; j++) {
					if(tempMap[i][j] == TILE_FLOOR) fout<<'1';
					else fout<<'0';
				}
				if(i < MapHeight-1) fout<<'\n';
			}
			fout.close();
		}
		IScene::Terminate();
}
void CustomStageScene::Update(float deltaTime) {
	if (previewTool) {
		previewTool->Position = Engine::GameEngine::GetInstance().GetMousePosition();
		previewTool->Update(deltaTime);
	}
}
void CustomStageScene::Draw() const {
	IScene::Draw();
}
void CustomStageScene::OnMouseDown(int button, int mx, int my) {
	if ((button & 1) && !imgTarget->Visible && previewTool) {
		if(previewTool) {
			UIGroup->RemoveObject(previewTool->GetObjectIterator());
			previewTool = nullptr;
		}
	}
	IScene::OnMouseDown(button, mx, my);
}
void CustomStageScene::OnMouseMove(int mx, int my) {
	IScene::OnMouseMove(mx, my);
	const int x = mx / BlockSize;
	const int y = my / BlockSize;
	if (!previewTool || x < 0 || x >= MapWidth || y < 0 || y >= MapHeight) {
		imgTarget->Visible = false;
		return;
	}
	imgTarget->Visible = true;
	imgTarget->Position.x = x * BlockSize;
	imgTarget->Position.y = y * BlockSize;
}
void CustomStageScene::OnMouseUp(int button, int mx, int my) {
	IScene::OnMouseUp(button, mx, my);
	if (!imgTarget->Visible)
		return;
	const int x = mx / BlockSize;
	const int y = my / BlockSize;
	std::cout<<mapState[y][x]-TILE_DIRT<<"\n";
	if (button & 1) {
		if(!previewTool) return;
		if(!CheckSpaceValid(x, y)) {
			Engine::Sprite* sprite;
			GroundEffectGroup->AddNewObject(sprite = new DirtyEffect("play/target-invalid.png", 1, x * BlockSize + BlockSize / 2, y * BlockSize + BlockSize / 2));
			sprite->Rotation = 0;
			return;
		}
		if(previewTool) {
			if(!tooltype) {
				TileType tempTILE = mapState[y][x];
				std::cout<<tempTILE<<" "<<mapState[y][x]<<"\n";
				AudioHelper::PlayAudio("digging.wav");
				mapState[y][x] = TILE_FLOOR;
				if(tempTILE != mapState[y][x]) {
					std::multimap<int, int> tempACT;
					tempACT.insert(std::make_pair(y, x));
					undoactions.push_back(tempACT);
				}
				TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", x*BlockSize, y*BlockSize, BlockSize, BlockSize));
			}
			else {
				TileType tempTILE = mapState[y][x];
				std::cout<<tempTILE<<" "<<mapState[y][x]<<"\n";
				AudioHelper::PlayAudio("digging.wav");
				mapState[y][x] = TILE_DIRT;
				if(tempTILE != mapState[y][x]) {
					std::multimap<int, int> tempACT;
					tempACT.insert(std::make_pair(y, x));
					undoactions.push_back(tempACT);
				}
				TileMapGroup->AddNewObject(new Engine::Image("play/DIRT.png", x*BlockSize, y*BlockSize, BlockSize, BlockSize));
			}
			OnMouseMove(mx, my);
		}
	}
}
void CustomStageScene::ReadMap() {
	std::string filename = std::string("Resource/map") + std::to_string(NOWMAP) + ".txt";
	// Read map file.
	char c;
	std::vector<bool> mapData;
	std::ifstream fin(filename);
	while (fin >> c) {
		switch (c) {
		case '0': mapData.push_back(false); break;
		case '1': mapData.push_back(true); break;
		case '\n':
		case '\r':
			if (static_cast<int>(mapData.size()) / MapWidth != 0)
				throw std::ios_base::failure("Map data is corrupted.");
			break;
		default: throw std::ios_base::failure("Map data is corrupted.");
		}
	}
	fin.close();
	// Validate map data.
	if (static_cast<int>(mapData.size()) != MapWidth * MapHeight)
		throw std::ios_base::failure("Map data is corrupted.");
	// Store map in 2d array.
	mapState = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth));
	tempMap = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth));
	for (int i = 0; i < MapHeight; i++) {
		for (int j = 0; j < MapWidth; j++) {
			const int num = mapData[i * MapWidth + j];
			mapState[i][j] = num ? TILE_FLOOR : TILE_DIRT;
			tempMap[i][j] = TILE_DIRT;
			if (num)
				TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
			else
				TileMapGroup->AddNewObject(new Engine::Image("play/dirt.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
		}
	}
}
void CustomStageScene::ConstructUI() {
	// Background
	UIGroup->AddNewObject(new Engine::Image("play/sand.png", 1280, 0, 320, 832));
	// Text
	UIGroup->AddNewObject(new Engine::Label(std::string("CUSTOM"), "pirulen.ttf", 32, 1345, 0));
	TurretButton* btn;
	// Button 1
	btn = new TurretButton("play/floor.png", "play/dirt.png",
		Engine::Sprite("play/shovel.png", 1360, 186, 0, 0, 0, 0),
		Engine::Sprite("play/shovel.png", 1360, 186, 0, 0, 0, 0)
		, 1360, 186, 0);
	btn->SetOnClickCallback(std::bind(&CustomStageScene::UIBtnClicked, this, 0));
	UIGroup->AddNewControlObject(btn);
	// Button 2
	btn = new TurretButton("play/floor.png", "play/dirt.png",
		Engine::Sprite("play/place.png", 1456, 186, 0, 0, 0, 0),
		Engine::Sprite("play/place.png", 1456, 186, 0, 0, 0, 0)
		, 1456, 186, 0);
	btn->SetOnClickCallback(std::bind(&CustomStageScene::UIBtnClicked, this, 1));
	UIGroup->AddNewControlObject(btn);
	// Button 3
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int shift = 135 + 25;
	Engine::ImageButton *bn;
	//
	bn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", w-shift-100, h-shift-380, 200, 40);
	bn->SetOnClickCallback(std::bind(&CustomStageScene::UIBtnClicked, this, 4));
	AddNewControlObject(bn);
	AddNewObject(new Engine::Label("MAP 3", "pirulen.ttf", 30, w-shift, h-shift-360, 0, 0, 0, 255, 0.5, 0.5));
	//
	bn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", w-shift-100, h-shift-330, 200, 40);
	bn->SetOnClickCallback(std::bind(&CustomStageScene::UIBtnClicked, this, 5));
	AddNewControlObject(bn);
	AddNewObject(new Engine::Label("MAP 4", "pirulen.ttf", 30, w-shift, h-shift-310, 0, 0, 0, 255, 0.5, 0.5));
	//
	bn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", w-shift-100, h-shift-280, 200, 40);
	bn->SetOnClickCallback(std::bind(&CustomStageScene::UIBtnClicked, this, 6));
	AddNewControlObject(bn);
	AddNewObject(new Engine::Label("MAP 5", "pirulen.ttf", 30, w-shift, h-shift-260, 0, 0, 0, 255, 0.5, 0.5));
	//
	bn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", w-shift-100, h-shift-230, 200, 40);
	bn->SetOnClickCallback(std::bind(&CustomStageScene::UIBtnClicked, this, 7));
	AddNewControlObject(bn);
	AddNewObject(new Engine::Label("MAP 6", "pirulen.ttf", 30, w-shift, h-shift-210, 0, 0, 0, 255, 0.5, 0.5));
	//
	bn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", w-shift-100, h-shift-180, 200, 40);
	bn->SetOnClickCallback(std::bind(&CustomStageScene::UIBtnClicked, this, 8));
	AddNewControlObject(bn);
	AddNewObject(new Engine::Label("MAP 7", "pirulen.ttf", 30, w-shift, h-shift-160, 0, 0, 0, 255, 0.5, 0.5));
	//
	bn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", w-shift-100, h-shift, 200, 100);
	bn->SetOnClickCallback(std::bind(&CustomStageScene::UIBtnClicked, this, 2));
	AddNewControlObject(bn);
	AddNewObject(new Engine::Label("Back", "pirulen.ttf", 30, w-shift, h-shift+50, 0, 0, 0, 255, 0.5, 0.5));
	// Button 4
	bn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", w-shift-100, h-shift-110, 200, 100);
	bn->SetOnClickCallback(std::bind(&CustomStageScene::UIBtnClicked, this, 3));
	AddNewControlObject(bn);
	AddNewObject(new Engine::Label("Save", "pirulen.ttf", 30, w-shift, h-shift-60, 0, 0, 0, 255, 0.5, 0.5));
	// Button 5
	bn = new Engine::ImageButton("play/undo.png", "play/sureundo.png", w-shift-100, h-shift-580, 100, 50);
	bn->SetOnClickCallback(std::bind(&CustomStageScene::UIBtnClicked, this, 9));
	AddNewControlObject(bn);
	// Button 6
	bn = new Engine::ImageButton("play/do.png", "play/suredo.png", w-shift, h-shift-580, 100, 50);
	bn->SetOnClickCallback(std::bind(&CustomStageScene::UIBtnClicked, this, 10));
	AddNewControlObject(bn);
}

void CustomStageScene::UIBtnClicked(int id) {
	if (previewTool) {
		UIGroup->RemoveObject(previewTool->GetObjectIterator());
	}
	// TODO: [CUSTOM-TURRET]: On callback, create the turret.
	if (id == 0) {
		previewTool = new Shovel(0, 0);
		tooltype = 0;
	}
	else if (id == 1) {
		previewTool = new Dirt(0, 0);
		tooltype = 1;
	}
	else if (id == 2) {
		Engine::GameEngine::GetInstance().ChangeScene("stage-select");
	}
	else if (id == 3) {
		Engine::GameEngine::GetInstance().ChangeScene("save");
	}
	else if (id == 4) {
		if(NOWMAP == 3) return;
		NOWMAP = 3;
		Engine::GameEngine::GetInstance().ChangeScene("custom");
	}
	else if (id == 5) {
		if(NOWMAP == 4) return;
		NOWMAP = 4;
		Engine::GameEngine::GetInstance().ChangeScene("custom");
	}
	else if (id == 6) {
		if(NOWMAP == 5) return;
		NOWMAP = 5;
		Engine::GameEngine::GetInstance().ChangeScene("custom");
	}
	else if (id == 7) {
		if(NOWMAP == 6) return;
		NOWMAP = 6;
		Engine::GameEngine::GetInstance().ChangeScene("custom");
	}
	else if (id == 8) {
		if(NOWMAP == 7) return;
		NOWMAP = 7;
		Engine::GameEngine::GetInstance().ChangeScene("custom");
	}
	else if (id == 9) {
		if(!undoactions.empty()) {
			int y, x;
			std::multimap<int, int> it = undoactions.back();
			for(auto iter = it.begin();iter!=it.end();iter++) {
				y = iter->first;
				x = iter->second;
			}
			if(mapState[y][x] == TILE_FLOOR) {
				mapState[y][x] = TILE_DIRT;
				TileMapGroup->AddNewObject(new Engine::Image("play/dirt.png", x*BlockSize, y*BlockSize, BlockSize, BlockSize));
			}
			else {
				mapState[y][x] = TILE_FLOOR;
				TileMapGroup->AddNewObject(new Engine::Image("play/FLOOR.png", x*BlockSize, y*BlockSize, BlockSize, BlockSize));
			}
			doactions.push_back(it);
			undoactions.pop_back();
		}
	}
	else if (id == 10) {
		if(!doactions.empty()) {
			int y, x;
			std::multimap<int, int> it = doactions.back();
			for(auto iter = it.begin();iter!=it.end();iter++) {
				y = iter->first;
				x = iter->second;
			}
			if(mapState[y][x] == TILE_DIRT) {
				mapState[y][x] = TILE_FLOOR;
				TileMapGroup->AddNewObject(new Engine::Image("play/FLOOR.png", x*BlockSize, y*BlockSize, BlockSize, BlockSize));
			}
			else {
				mapState[y][x] = TILE_DIRT;
				TileMapGroup->AddNewObject(new Engine::Image("play/dirt.png", x*BlockSize, y*BlockSize, BlockSize, BlockSize));
			}
			undoactions.push_back(it);
			doactions.pop_back();
		}
	}
	if (!previewTool)
		return;
	previewTool->Position = Engine::GameEngine::GetInstance().GetMousePosition();
	previewTool->Tint = al_map_rgba(255, 255, 255, 200);
	previewTool->Enabled = false;
	previewTool->Preview = true;
	UIGroup->AddNewObject(previewTool);
	OnMouseMove(Engine::GameEngine::GetInstance().GetMousePosition().x, Engine::GameEngine::GetInstance().GetMousePosition().y);
}

bool CustomStageScene::CheckSpaceValid(int x, int y) {
	if (x < 0 || x >= MapWidth || y < 0 || y >= MapHeight)
		return false;
	auto map00 = mapState[y][x];
	mapState[y][x] = TILE_OCCUPIED;
	std::vector<std::vector<int>> map = CalculateBFSDistance();
	mapState[y][x] = map00;
	if (map[0][0] == -1)
		return false;
	for (auto& it : EnemyGroup->GetObjects()) {
		Engine::Point pnt;
		pnt.x = floor(it->Position.x / BlockSize);
		pnt.y = floor(it->Position.y / BlockSize);
		if (pnt.x < 0) pnt.x = 0;
		if (pnt.x >= MapWidth) pnt.x = MapWidth - 1;
		if (pnt.y < 0) pnt.y = 0;
		if (pnt.y >= MapHeight) pnt.y = MapHeight - 1;
		if (map[pnt.y][pnt.x] == -1)
			return false;
	}
	// All enemy have path to exit.
	//mapState[y][x] = TILE_OCCUPIED;
	mapDistance = map;
	for (auto& it : EnemyGroup->GetObjects())
		dynamic_cast<Enemy*>(it)->UpdatePath(mapDistance);
	return true;
}
std::vector<std::vector<int>> CustomStageScene::CalculateBFSDistance() {
	// Reverse BFS to find path.
	std::vector<std::vector<int>> map(MapHeight, std::vector<int>(std::vector<int>(MapWidth, -1)));
	std::queue<Engine::Point> que;
	std::vector<std::vector<int>> tag(MapHeight, std::vector<int>(std::vector<int>(MapWidth, -1)));
	for(int i=0;i<MapHeight;i++) {
		for(int j=0;j<MapWidth;j++) {
			if(i==MapHeight-1 && j==MapWidth-1) tag[i][j] = 1;
			else tag[i][j] = 0;
		}
	}
	// Push end point.
	// BFS from end point.
	if (mapState[MapHeight - 1][MapWidth - 1] != TILE_DIRT)
		return map;
	que.push(Engine::Point(MapWidth - 1, MapHeight - 1));
	map[MapHeight - 1][MapWidth - 1] = 0;
	while (!que.empty()) {
		Engine::Point p = que.front();
		que.pop();
		int temp = map[p.y][p.x]+1;
		// TODO: [BFS PathFinding] (1/1): Implement a BFS starting from the most right-bottom block in the map.
		//               For each step you should assign the corresponding distance to the most right-bottom block.
		//std::cout<<p.x<<" "<<p.y<<" "<<map[p.y][p.x]<<" "<<temp<<"\n"; (for debugging)
		if(p.y-1>=0 && mapState[p.y-1][p.x] == TILE_DIRT && !tag[p.y-1][p.x]) {
			que.push(Engine::Point(p.x, p.y-1));
			map[p.y-1][p.x] = temp;
		}
		if(p.y-1>=0) tag[p.y-1][p.x] = 1;
		if(p.x-1>=0 && mapState[p.y][p.x-1] == TILE_DIRT && !tag[p.y][p.x-1]) {
			que.push(Engine::Point(p.x-1, p.y));
			map[p.y][p.x-1] = temp;
		}
		if(p.x-1>=0) tag[p.y][p.x-1] = 1;
		if(p.y+1 < MapHeight && mapState[p.y+1][p.x] == TILE_DIRT && !tag[p.y+1][p.x]) {
			que.push(Engine::Point(p.x, p.y+1));
			map[p.y+1][p.x] = temp;
		}
		if(p.y+1<MapHeight) tag[p.y+1][p.x] = 1;
		if(p.x+1 < MapWidth && mapState[p.y][p.x+1] == TILE_DIRT && !tag[p.y][p.x+1]) {
			que.push(Engine::Point(p.x+1, p.y));
			map[p.y][p.x+1] = temp;
		}
		if(p.x+1<MapWidth) tag[p.y][p.x+1] = 1;
		//               mapState[y][x] is TILE_DIRT if it is empty.
	}
	return map;
}
