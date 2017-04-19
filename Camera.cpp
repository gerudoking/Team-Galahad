#include "Camera.h"
#include "Game.h"
#include "InputManager.h"

Camera* Camera::instance = nullptr;

Camera::Camera() {
	focus = nullptr;
	pos = Vec2();
	speed = Vec2(0.3, 0.3);
}

Camera::~Camera() {
	focus = nullptr;
}

void Camera::Follow(GameObject* newFocus) {
	focus = newFocus;
}

void Camera::Unfollow() {
	focus = nullptr;
}

void Camera::Update(float dt) {
	if(!focus) {
		if(InputManager::GetInstance().IsKeyDown(UP_ARROW_KEY))
			pos.y -= speed.y*dt;
		if(InputManager::GetInstance().IsKeyDown(DOWN_ARROW_KEY))
			pos.y += speed.y*dt;
		if(InputManager::GetInstance().IsKeyDown(LEFT_ARROW_KEY))
			pos.x -= speed.x*dt;
		if(InputManager::GetInstance().IsKeyDown(RIGHT_ARROW_KEY))
			pos.x += speed.x*dt;
	}else{
		//atribui movimentação ao objeto em foco
		/*if(InputManager::GetInstance().IsKeyDown(UP_ARROW_KEY))
			focus->box.y -= speed.y*dt;
		if(InputManager::GetInstance().IsKeyDown(DOWN_ARROW_KEY))
			focus->box.y += speed.y*dt;
		if(InputManager::GetInstance().IsKeyDown(LEFT_ARROW_KEY))
			focus->box.x -= speed.x*dt;
		if(InputManager::GetInstance().IsKeyDown(RIGHT_ARROW_KEY))
			focus->box.x += speed.x*dt;*/
		int w, h;
		SDL_GetRendererOutputSize(Game::GetInstance().GetRenderer(), &w, &h);
		pos.x = focus->box.GetCenter().x-w/2;
		pos.y = focus->box.GetCenter().y-h/2;
	}
}

GameObject* Camera::GetFocus() {
	return focus;
}

Camera& Camera::GetInstance() {
	if(instance == nullptr)
		instance = new Camera();
	return *instance;
}
