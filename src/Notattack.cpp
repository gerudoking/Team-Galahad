#include "Notattack.h"
#include "Camera.h"

Notattack::Notattack(std::string file, int frames, int x, int y, int power, int lifetime, Face facing) {
	sp = Sprite(file, frames);
	box = Rect(x, y, sp.GetWidth(), sp.GetHeight());
	Notattack::power = power;
	Notattack::lifetime = Timer(lifetime);
	Notattack::lifetime.Start();
	Notattack::facing = facing;
	if(facing == LEFT) {
		sp.SetFrame(1);
		speed = Vec2(-0.4, 0);
	}else{
		sp.SetFrame(2);
		speed = Vec2(0.4, 0);
	}
}

Notattack::~Notattack() {

}

void Notattack::Update(float dt) {
	UpdateTimers(dt);
	UpdatePosition(dt);
}

void Notattack::UpdateTimers(float dt) {
	lifetime.Update(dt);
}

void Notattack::UpdatePosition(float dt) {
	box.x += speed.x*dt;
}

void Notattack::UpdateBoundingBox() {

}

void Notattack::Render() {
	sp.Render(box.x - Camera::GetInstance().pos.x -1, box.y - Camera::GetInstance().pos.y -1);
}

bool Notattack::NotifyTileCollision(Face face) {
	return false;
}

void Notattack::NotifyCollision(GameObject* other) {

}

bool Notattack::Is(std::string type) {
	return (type == "Notattack");
}

bool Notattack::IsDead() {
	return (lifetime.GetTime() > lifetime.GetLimit());
}

Notattack* Notattack::get() {
	return this;
}
