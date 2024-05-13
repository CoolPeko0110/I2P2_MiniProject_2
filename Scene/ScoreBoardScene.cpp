//
// Created by johnn on 2024/5/12.
//

#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <map>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "UI/Component/Slider.hpp"
#include "ScoreBoardScene.hpp"


void ScoreBoardScene::Initialize() {
    page = 0;
    numofpage = 0;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 3 / 2 - 50,
                                  400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreBoardScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));
    //
    btn = new Engine::ImageButton("scoreboard/next-page.png", "scoreboard/next-page-floor.png", halfW + 400, halfH * 3 / 2 - 50,
                                  100, 100);
    btn->SetOnClickCallback(std::bind(&ScoreBoardScene::NextOnClick, this, 2));
    AddNewControlObject(btn);

    btn = new Engine::ImageButton("scoreboard/prev-page.png", "scoreboard/prev-page-floor.png", halfW + 250, halfH * 3 / 2 - 50,
                                  100, 100);
    btn->SetOnClickCallback(std::bind(&ScoreBoardScene::PrevOnClick, this, 2));
    AddNewControlObject(btn);

    std::string filename = "Resource/scoreboard.txt";
    std::ifstream fin(filename);
    int i;
    for( i=0 ; !fin.eof() ; i++ ) {
        std::string name, scorestr;
        fin>>name>>scorestr;
        int score = std::atoi(scorestr.c_str());
        if(name!="\0") Score.insert(std::pair<int, std::string>(score, name));
    }
    numofpage = (i-1)/5 - (((i-1)%5)?0:1);
    int count = 5;
    for(auto iter = Score.rbegin(); count-- && iter!=Score.rend(); iter++) {
        AddNewObject(new Engine::Label(iter->second+" "+std::to_string(iter->first), "pirulen.ttf", 48, halfW, halfH * 3 / 2 - 520 + 100 * (4-count), 255, 255, 255, 255, 0.5, 0.5));
    }
    fin.close();
    //
}

void ScoreBoardScene::Terminate() {
    Score.clear();
    IScene::Terminate();
}

void ScoreBoardScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("start");
}

void ScoreBoardScene::NextOnClick(int stage) {
    if(page==numofpage) return;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    page++;
    Engine::ImageButton *btn;
    for(int i=0;i<5;i++) {
        btn = new Engine::ImageButton("scoreboard/black.png", "scoreboard/black.png", halfW - 200, halfH * 3 / 2 - 550 + 100 * (i), 1000, 100);
        AddNewControlObject(btn);
    }
    int count = 0, num = 0;
    for(auto iter = Score.rbegin(); count <= page && iter!=Score.rend(); iter++, num++) {
        if(num == 5) {
            num = 0;
            count++;
        }
        if(count == page) {
            if(iter->second != "\0") AddNewObject(new Engine::Label(iter->second+" "+std::to_string(iter->first), "pirulen.ttf", 48, halfW, halfH * 3 / 2 - 520 + 100 * (num), 255, 255, 255, 255, 0.5, 0.5));
        }
        else if(count < page){
            if(iter->second != "\0") AddNewObject(new Engine::Label(iter->second+" "+std::to_string(iter->first), "pirulen.ttf", 48, halfW, halfH * 3 / 2 - 520 + 100 * (num), 0, 0, 0, 255, 0.5, 0.5));
        }
        else {
            break;
        }
    }
}

void ScoreBoardScene::PrevOnClick(int stage) {
    if(!page) return;
    page--;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    int count = 0, num = 0;
    Engine::ImageButton *btn;
    for(int i=0;i<5;i++) {
        btn = new Engine::ImageButton("scoreboard/black.png", "scoreboard/black.png", halfW - 200, halfH * 3 / 2 - 550 + 100 * (i), 1000, 100);
        AddNewControlObject(btn);
    }
    num = 0;
    for(auto iter = Score.rbegin(); count <= page && iter!=Score.rend(); iter++, num++) {
        if(num == 5) {
            num = 0;
            count++;
        }
        if(count == page) {
            if(iter->second != "\0") AddNewObject(new Engine::Label(iter->second+" "+std::to_string(iter->first), "pirulen.ttf", 48, halfW, halfH * 3 / 2 - 520 + 100 * (num), 255, 255, 255, 255, 0.5, 0.5));
        }
        else if(count < page){
            if(iter->second != "\0") AddNewObject(new Engine::Label(iter->second+" "+std::to_string(iter->first), "pirulen.ttf", 48, halfW, halfH * 3 / 2 - 520 + 100 * (num), 0, 0, 0, 255, 0.5, 0.5));
        }
        else {
            break;
        }
    }
}



