//
// Created by coolpeko on 6/12/24.
//

#include "FirstScene.hpp"
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
extern int firstwhichone;
void first_scene::Initialize() {
    reload = 5;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<> dist(1, 3);
    int rnd = dist(rng);
    firstwhichone = rnd;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    //black = new Engine::Image("scoreboard/black.png", w/2-200, h/2, 800, 812);
    //black->Draw();
    AddNewObject(new Engine::Label("Play", "pirulen.ttf", 48, w/2, h / 4 - 250, 255, 0, 0, 255, 0.5, 0.5));
}
void first_scene::Terminate() {
    IScene::Terminate();
}

void first_scene::Draw() const {
    //black->Draw();
    //firstblood->Draw();
}

void first_scene::Update(float deltaTime) {
    reload -= deltaTime;
    if(reload<=0) {
        Engine::GameEngine::GetInstance().ChangeScene("secondscene");
    }
}


