#include "TitleState.h"
#include "Game.h"
#include "InputManager.h"

TitleState::TitleState() {
	bg2.Open("img/title2.png");
	bgTimer = Timer(500);
	bgTimer.Start();
	bgBool = true;
}

TitleState::~TitleState() {

}

void TitleState::LoadAssets() {
	music.Open("audio/stageState.ogg");
	bg.Open("img/LancelotIdleLeft.png");
	bg.Open("img/LancelotIdleRight.png");
	bg.Open("img/LancelotRunningLeft.png");
	bg.Open("img/LancelotRunningRight.png");
	bg.Open("img/LancelotHiddenLeft.png");
	bg.Open("img/LancelotHiddenRight.png");
	bg.Open("img/LancelotRunningLeftInv.png");
	bg.Open("img/LancelotRunningRightInv.png");
	bg.Open("img/notattack.png");
	bg.Open("img/healthBar.png");
	bg.Open("img/stealthBar.png");
	bg.Open("img/NotfredoIdleLeft.png");
	bg.Open("img/NotfredoIdleRight.png");
	bg.Open("img/NotfredoRunningLeft.png");
	bg.Open("img/NotfredoRunningRight.png");
	bg.Open("img/tileset3d2.png");
	bg.Open("img/title1.png");
}

void TitleState::Update() {
	if(InputManager::GetInstance().KeyPress(SDLK_SPACE)) {
		quitRequested = true;
		Game::GetInstance().RemoveState();
		Game::GetInstance().AddState(new StageState());
	}
	if(bgTimer.GetTime() < bgTimer.GetLimit()) {
		bgTimer.Update(Game::GetInstance().GetDeltaTime());
	}else{
		bgTimer.Start();
		bgBool = !bgBool;
	}

}

void TitleState::Render() {
	if(bgBool)
		bg.Render(0, 0);
	bg2.Render(0, 160);
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

