#include "TitleState.h"
#include "Game.h"
#include "InputManager.h"

TitleState::TitleState() {

}

TitleState::~TitleState() {

}

void TitleState::LoadAssets() {
	music.Open("audio/stageState.ogg");
	bg.Open("img/penguin.png");
	bg.Open("img/cubngun.png");
	bg.Open("img/healthBar.png");
	bg.Open("img/penguinbullet.png");
	bg.Open("img/penguindeath.png");
	bg.Open("img/alien.png");
	bg.Open("img/aliendeath.png");
	bg.Open("img/minion.png");
	bg.Open("img/miniondeath.png");
	bg.Open("img/minionbullet2.png");
	bg.Open("img/penguinface.png");
	bg.Open("img/ocean.jpg");
	bg.Open("img/title.jpg");
}

void TitleState::Update() {
	if(InputManager::GetInstance().KeyPress(SDLK_SPACE)) {
		quitRequested = true;
		Game::GetInstance().RemoveState();
		Game::GetInstance().AddState(new StageState());
	}
}

void TitleState::Render() {
	bg.Render(0, 0);
}

bool TitleState::QuitRequested() {
	return quitRequested;
}

bool TitleState::Is(std::string type) {
	return (type == "Title");
}

TitleState* TitleState::get() {
	return this;
}

