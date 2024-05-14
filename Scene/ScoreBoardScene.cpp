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
    if(!page) page = 0;
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
        std::string name, scorestr, date;
        fin>>name>>scorestr>>date;
        int score = std::atoi(scorestr.c_str());
        if(name!="\0") {
            Score.insert(std::pair<int, std::string>(score, name));
            Date.insert(std::pair<int, std::string>(score, date));
        }
    }
    for(auto iter = Score.rbegin(), it = Date.rbegin();iter!=Score.rend();iter++, it++) {
        std::cout<<iter->second<<" "<<iter->first<<" "<<it->second<<"\n";
    }
    numofpage = (i-1)/5 - (((i-1)%5)?0:1);
    int count = 0, num = 0;
    for(auto iter = Score.rbegin(), it = Date.rbegin(); count <= page && iter!=Score.rend(); iter++, num++, it++) {
        if(num == 5) {
            num = 0;
            count++;
        }
        if(count == page) {
            if(iter->second != "\0") AddNewObject(new Engine::Label(iter->second+" "+std::to_string(iter->first)+" "+it->second, "pirulen.ttf", 48, halfW, halfH * 3 / 2 - 520 + 100 * (num), 255, 255, 255, 255, 0.5, 0.5));
        }
        else if(count < page){
            if(iter->second != "\0") AddNewObject(new Engine::Label(iter->second+" "+std::to_string(iter->first)+" "+it->second, "pirulen.ttf", 48, halfW, halfH * 3 / 2 - 520 + 100 * (num), 0, 0, 0, 255, 0.5, 0.5));
        }
        else {
            break;
        }
    }
    fin.close();
    //
}

void ScoreBoardScene::Terminate() {
    Score.clear();
    IScene::Terminate();
}

void ScoreBoardScene::BackOnClick(int stage) {
    page = 0;
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

void ScoreBoardScene::NextOnClick(int stage) {
    if(page==numofpage) return;
    page++;
    Engine::GameEngine::GetInstance().ChangeScene("scoreboard");
}

void ScoreBoardScene::PrevOnClick(int stage) {
    if(!page) return;
    page--;
    Engine::GameEngine::GetInstance().ChangeScene("scoreboard");
}