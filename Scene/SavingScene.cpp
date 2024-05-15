//
// Created by johnn on 2024/5/15.
//

#include <functional>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "SavingScene.hpp"
#include "PlayScene.hpp"
#include "Engine/Point.hpp"
#include <fstream>

#include "CustomStageScene.hpp"

extern int NOWMAP;
const int SavingScene::MapWidth = 20, SavingScene::MapHeight = 13;

void SavingScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    AddNewObject(new Engine::Label("you want to save in map "+std::to_string(NOWMAP)+"?", "pirulen.ttf", 48, halfW, halfH / 4 + 10, 255, 255, 255, 255, 0.5, 0.5));
    Engine::ImageButton* btn;
    //
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH -200, 400, 80);
    btn->SetOnClickCallback(std::bind(&SavingScene::BackOnClick, this, 0));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("MAP 3", "pirulen.ttf", 48, halfW, halfH -160, 0, 0, 0, 255, 0.5, 0.5));
    //
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH-100 , 400, 80);
    btn->SetOnClickCallback(std::bind(&SavingScene::BackOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("MAP 4", "pirulen.ttf", 48, halfW, halfH-60 , 0, 0, 0, 255, 0.5, 0.5));
    //
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH , 400, 80);
    btn->SetOnClickCallback(std::bind(&SavingScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("MAP 5", "pirulen.ttf", 48, halfW, halfH+40 , 0, 0, 0, 255, 0.5, 0.5));
    //
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH +100, 400, 80);
    btn->SetOnClickCallback(std::bind(&SavingScene::BackOnClick, this, 3));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("MAP 6", "pirulen.ttf", 48, halfW, halfH +140, 0, 0, 0, 255, 0.5, 0.5));
    //
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH+200 , 400, 80);
    btn->SetOnClickCallback(std::bind(&SavingScene::BackOnClick, this, 4));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("MAP 7", "pirulen.ttf", 48, halfW, halfH+240, 0, 0, 0, 255, 0.5, 0.5));
    //
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW + 250, halfH +100, 400, 80);
    btn->SetOnClickCallback(std::bind(&SavingScene::BackOnClick, this, 5));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("YES & PLAY", "pirulen.ttf", 48, halfW + 450, halfH +140 , 0, 0, 0, 255, 0.5, 0.5));
    //
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW + 250, halfH +200, 400, 80);
    btn->SetOnClickCallback(std::bind(&SavingScene::BackOnClick, this, 6));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("NO & BACK", "pirulen.ttf", 48, halfW + 450, halfH +240, 0, 0, 0, 255, 0.5, 0.5));
    //
}
void SavingScene::Terminate() {
    IScene::Terminate();
}
void SavingScene::BackOnClick(int stage) {
    // Change to select scene.
    if(stage < 5) {
        NOWMAP = stage+3;
        Engine::GameEngine::GetInstance().ChangeScene("save");
    }
    else if (stage == 5) {
        std::string filename = std::string("Resource/RAMmap") + ".txt";
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
        for (int i = 0; i < MapHeight; i++) {
            for (int j = 0; j < MapWidth; j++) {
                const int num = mapData[i * MapWidth + j];
                mapState[i][j] = num ? TILE_FLOOR : TILE_DIRT;
            }
        }
        filename = std::string("Resource/map") + std::to_string(NOWMAP) + ".txt";
        std::ofstream fout(filename);
        for (int i = 0; i < MapHeight; i++) {
            for (int j = 0; j < MapWidth; j++) {
                if(mapState[i][j] == TILE_FLOOR) fout<<'1';
                else fout<<'0';
            }
            if(i < MapHeight-1) fout<<'\n';
        }
        fout.close();
        PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
        scene->MapId = NOWMAP;
        Engine::GameEngine::GetInstance().ChangeScene("play");
    }
    else {
        Engine::GameEngine::GetInstance().ChangeScene("stage-select");
    }
}