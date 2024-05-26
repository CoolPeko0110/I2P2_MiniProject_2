#include <functional>
#include <string>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <fstream>
#include <iostream>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "Engine/Point.hpp"
#include "WinScene.hpp"

#include <ctime>
extern int SCORE;

void WinScene::Initialize() {
	ticks = 0;
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int halfW = w / 2;
	int halfH = h / 2;
	NAME.clear();
	AddNewObject(new Engine::Image("win/outline.png", halfW, halfH-246, 100, 60, 0.5, 0.5));
	AddNewObject(new Engine::Image("win/benjamin-sad.png", halfW, halfH, 0, 0, 0.5, 0.5));
	AddNewObject(new Engine::Label("You Win!", "pirulen.ttf", 48, halfW, halfH / 4 -10, 255, 255, 255, 255, 0.5, 0.5));
	Engine::ImageButton* btn;
	btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
	btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
	AddNewControlObject(btn);
	AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
	bgmId = AudioHelper::PlayAudio("win.wav");
	AddNewObject(new Engine::Label(NAME, "pirulen.ttf", 48, halfW, halfH * 3 / 2 - 454, 255, 255, 255, 255, 0.5, 0.5));
}
void WinScene::Terminate() {
	time_t now = time(0);
	tm *ltm = localtime(&now);
	std::string date = std::to_string(1+ltm->tm_mon) + "/" + std::to_string(ltm->tm_mday) + "_" + std::to_string(ltm->tm_hour) + ":" + std::to_string(ltm->tm_min);
	char str1[100];
	std::string temp = std::to_string(SCORE);
	int i;
	for(i = 0; temp[i]<='9' && temp[i]>='0'; i++) {
		str1[i] = temp[i];
	}
	str1[i] = '\n';
	str1[i+1] = 0;
	//std::cout<<str1; (for debugging)
	std::string filename = "Resource/scoreboard.txt";
	std::fstream out(filename, std::ios::app);
	if(NAME == "\0") NAME = "WHORU\0";
	out<<NAME.c_str()<<" "<<str1<<" "<<date.c_str()<<"\n";
	out.close();
	//std::cout<<NAME.c_str()<<str1<<"\n";
	IScene::Terminate();
	AudioHelper::StopBGM(bgmId);
}
void WinScene::Update(float deltaTime) {
	ticks += deltaTime;
	if (ticks > 4 && ticks < 100 &&
		dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId == 2) {
		ticks = 100;
		bgmId = AudioHelper::PlayBGM("happy.ogg");
	}
}
void WinScene::BackOnClick(int stage) {
	// Change to select scene.
	keyStrokes.clear();
	Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
void WinScene::OnKeyDown(int keyCode) {
	IScene::OnKeyDown(keyCode);
	if(keyCode<=ALLEGRO_KEY_Z && keyCode>=ALLEGRO_KEY_A) {
		keyStrokes.push_back(keyCode-ALLEGRO_KEY_A+'A');
	}
	else if(keyCode>=ALLEGRO_KEY_0 && keyCode<=ALLEGRO_KEY_9) {
		keyStrokes.push_back(keyCode-ALLEGRO_KEY_0+'0');
	}
	else if(keyCode == ALLEGRO_KEY_BACKSPACE) {
		if(!keyStrokes.empty()) keyStrokes.pop_back();
	}
	NAME.clear();
	int count = 0;
	for(auto iter = keyStrokes.begin();iter!=keyStrokes.end();iter++) {
		//std::cout<<*iter; (for debugging)
		NAME += *iter;
		count++;
	}
	//std::cout<<"\n"; (for debugging)
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int halfW = w / 2;
	int halfH = h / 2;
	IScene::Terminate();
	ticks = 0;
	AddNewObject(new Engine::Image("win/outline.png", halfW, halfH-246, 100+count*50, 60, 0.5, 0.5));
	AddNewObject(new Engine::Image("win/benjamin-sad.png", halfW, halfH, 0, 0, 0.5, 0.5));
	AddNewObject(new Engine::Label("You Win!", "pirulen.ttf", 48, halfW, halfH / 4 -10, 255, 255, 255, 255, 0.5, 0.5));
	Engine::ImageButton* btn;
	btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
	btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
	AddNewControlObject(btn);
	AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
	AddNewObject(new Engine::Label(NAME, "pirulen.ttf", 48, halfW, halfH * 3 / 2 - 454, 255, 255, 255, 255, 0.5, 0.5));
}


