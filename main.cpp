// [main.cpp]
// This is the entry point of your game.
// You can register your scenes here, and start the game.
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Scene/LoseScene.hpp"
#include "Scene/PlayScene.hpp"
#include "Scene/ScoreBoardScene.hpp"
#include "Scene/StageSelectScene.hpp"
#include "Scene/WinScene.hpp"
#include "Scene/StartScene.h"
#include "Scene/SettingsScene.hpp"
#include "Scene/CustomStageScene.hpp"
#include "Scene/SavingScene.hpp"

int SCORE;
int NOWMAP = 3;

int main(int argc, char **argv) {
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();

    // TODO: [HACKATHON-1-SCENE] (3/4): Register Scenes here
	game.AddNewScene("stage-select", new StageSelectScene());
	game.AddNewScene("start", new StartScene());
	game.AddNewScene("scoreboard", new ScoreBoardScene());
	game.AddNewScene("custom", new CustomStageScene());
	game.AddNewScene("save", new SavingScene());
	//
	game.AddNewScene("settings", new SettingsScene());
	game.AddNewScene("play", new PlayScene());
	game.AddNewScene("lose", new LoseScene());
	game.AddNewScene("win", new WinScene());

    // TODO: [HACKATHON-1-SCENE] (4/4): Change the start scene
	//game.Start("stage-select", 60, 1600, 832);
	game.Start("start", 60, 1600, 832);
	//
	return 0;
}
