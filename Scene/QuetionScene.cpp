//
// Created by coolpeko on 6/13/24.
//

#include "QuetionScene.hpp"
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
#include "Turret/LaserTurret.hpp"
#include "Turret/MachineGunTurret.hpp"
#include "Turret/MissileTurret.hpp"
#include "Turret/ShapaTurret.hpp"
#include "Turret/Shovel.hpp"
#include "UI/Animation/Plane.hpp"
#include "Enemy/PlaneEnemy.hpp"
#include "PlayScene.hpp"

#include "WinScene.hpp"
#include "Enemy/PretenderEnemy.hpp"
#include "Engine/Resources.hpp"
#include "Enemy/SoldierEnemy.hpp"
#include "Enemy/TankEnemy.hpp"
#include "Turret/TurretButton.hpp"
#include "Turret/Tool.hpp"
#include <random>
extern int whichone;
extern int firstwhichone;
void question_scene::Initialize() {
    reload = 1000000;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton* btn;
    //
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 - 100, 400, 100);
    btn->SetOnClickCallback(std::bind(&question_scene::PlayOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Coin", "pirulen.ttf", 48, halfW, halfH / 2 - 50, 0, 0, 0, 255, 0.5, 0.5));
    //
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 + 100, 400, 100);
    btn->SetOnClickCallback(std::bind(&question_scene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Chocolate", "pirulen.ttf", 48, halfW, halfH / 2 + 150, 0, 0, 0, 255, 0.5, 0.5));
    //
}
void question_scene::Terminate() {
    IScene::Terminate();
}

void question_scene::Update(float deltaTime) {
    reload -= deltaTime;
}
void question_scene::PlayOnClick(int stage) {
    if(stage == 1) {
        final_time = 1000000 - reload;
        std::string filename = "Resource/answer.txt";
        std::fstream out(filename, std::ios::app);
        if(firstwhichone == 1) {
            out<<"Adult ";
        }
        else {
            out<<"Child ";
        }
        if(whichone == 1) {
            out<<"chocolate || chocolate time:"<<std::to_string(final_time)<<"\n";
        }
        else {
            out<<"coin || chocolate time:"<<std::to_string(final_time)<<"\n";
        }
        out.close();
        Engine::GameEngine::GetInstance().ChangeScene("start");
    }
    else {
        final_time = 1000000 - reload;
        std::string filename = "Resource/answer.txt";
        std::fstream out(filename, std::ios::app);
        if(firstwhichone == 1) {
            out<<"Adult ";
        }
        else {
            out<<"Child ";
        }
        if(whichone == 1) {
            out<<"chocolate || coin time:"<<std::to_string(final_time)<<"\n";
        }
        else {
            out<<"coin || coin time:"<<std::to_string(final_time)<<"\n";
        }
        out.close();
        Engine::GameEngine::GetInstance().ChangeScene("start");
    }
}