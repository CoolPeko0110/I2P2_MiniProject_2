#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "UI/Component/Slider.hpp"
#include "StageSelectScene.hpp"
#include <random>
#include <iostream>

extern int color;
extern float anstime;

void StageSelectScene::Initialize() {
    reload = 2000;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton* btn;
    //
    //
    btn = new Engine::ImageButton("play/black.png", "play/black.png", halfW - 200, halfH /2 + 200 , 400, 100);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this, 3));
    AddNewControlObject(btn);
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<> dist(1, 3);
    int rnd = dist(rng);
    AddNewObject(new Engine::Label("Custom", "pirulen.ttf", 48, halfW, halfH / 2 + 250, 255, 0, 0, 0, 0.5, 0.5));
    //
    switch(color) {
        case 0:
            AddNewObject(new Engine::Label("green", "pirulen.ttf", 48, halfW, halfH / 2 + 250, 255, 0, 0, 255, 0.5, 0.5));
            break;
        case 1:
            AddNewObject(new Engine::Label("red", "pirulen.ttf", 48, halfW, halfH / 2 + 250, 0, 0, 255, 255, 0.5, 0.5));
            break;
        case 2:
            AddNewObject(new Engine::Label("yellow", "pirulen.ttf", 48, halfW, halfH / 2 + 250, 0, 255, 0, 255, 0.5, 0.5));
            break;
        case 3:
            AddNewObject(new Engine::Label("purple", "pirulen.ttf", 48, halfW, halfH / 2 + 250, 255, 130, 71, 255, 0.5, 0.5));
            break;
        case 4:
            AddNewObject(new Engine::Label("blue", "pirulen.ttf", 48, halfW, halfH / 2 + 250, 255, 130, 171, 255, 0.5, 0.5));
            break;
        case 5:
            AddNewObject(new Engine::Label("pink", "pirulen.ttf", 48, halfW, halfH / 2 + 250, 255, 255, 0, 255, 0.5, 0.5));
            break;
        case 6:
            AddNewObject(new Engine::Label("white", "pirulen.ttf", 48, halfW, halfH / 2 + 250, 0, 0, 255, 255, 0.5, 0.5));
            break;
        default:
            break;
    }
    //
    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
}
void StageSelectScene::Update(float deltaTime) {
    std::cout<<reload<<std::endl;
    reload -= deltaTime;
}

void StageSelectScene::Terminate() {
	IScene::Terminate();
}
void StageSelectScene::PlayOnClick(int stage) {
    if(color < 6) {
        color++;
        anstime += 2000 - reload;
        std::cout<<anstime<<"\n";
        Engine::GameEngine::GetInstance().ChangeScene("stage-select");
    }
    else {
        anstime += 2000 - reload;
        std::cout<<anstime<<"\n";
        Engine::GameEngine::GetInstance().ChangeScene("start");
    }
}