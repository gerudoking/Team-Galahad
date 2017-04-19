#include "Notfredo.h"
#include "InputManager.h"
#include "Camera.h"
#include "StageState.h"

Notfredo::Notfredo(int x, int y) {
	sp = Sprite("img/Notfredo.png", 3);
	box = Rect(x+1, y+1, sp.GetWidth()-2, sp.GetHeight()-2);
	speed = Vec2(0.2, 0.6);
	hitpoints = 10;
	power = 1;
	iFrames = Timer(500);
	attackCD = Timer(1000);
	vigil = Timer(1500);
	vigil.Start();

}

Notfredo::~Notfredo() {

}

void Notfredo::Update(float dt) {
	sp.SetFrame(3);
	if(iFrames.GetTime() > -1) {
		if(iFrames.GetTime() < iFrames.GetLimit())
			iFrames.Update(dt);
		else
			iFrames.Reset();
	}
	if(attackCD.GetTime() > -1) {
		if(attackCD.GetTime() < attackCD.GetLimit())
			attackCD.Update(dt);
		else
			attackCD.Reset();
	}
	radius = Rect(box.x-200, box.y-200, box.w+400, box.h+400);
	if(Godofredo::player) {
		if(status != STUNNED) {
			Rect player = Godofredo::player->box;
			if(StageState::IsColliding(radius, player) && !Godofredo::player->IsHidden()) {
				speed.x = 0.3;
				if(player.x < box.x) {
					if(attackCD.GetTime() < 0) {
						StageState::AddObject(new Notattack("img/notattack.png", 2, box.x+16, box.y, power, 100, facing));
						attackCD.Start();
					}
					if(box.x - speed.x*dt < player.x)
						box.x = player.x;
					else
						box.x -= speed.x*dt;
					facing = LEFT;
					sp.SetFrame(2);
				}else{
					if(attackCD.GetTime() < 0) {
						StageState::AddObject(new Notattack("img/notattack.png", 2, box.x+box.w-16, box.y, power, 100, facing));
						attackCD.Start();
					}
					if(box.x + speed.x*dt > player.x)
						box.x = player.x;
					else
						box.x += speed.x*dt;
					facing = RIGHT;
					sp.SetFrame(3);
				}
				NotifyTileCollision(facing);
			}else{
				speed.x = 0.2;
				if(vigil.GetTime() < vigil.GetLimit()) {
					if(facing == LEFT) {
						box.x -= speed.x*dt;
						sp.SetFrame(2);
					}else{
						box.x += speed.x*dt;
						sp.SetFrame(3);
					}
					NotifyTileCollision(facing);
				}else{
					vigil.Start();
					if(facing == LEFT)
						facing = RIGHT;
					else
						facing = LEFT;
				}
				vigil.Update(dt);
			}
		}
	}
	speed.y += 0.06;
	box.y += speed.y*dt;
	if(speed.y < 0)
		NotifyTileCollision(UPPER);
	else
		NotifyTileCollision(BOTTOM);
	if(statusTimer.GetTime() > -1) {
		statusTimer.Update(dt);
		if(statusTimer.GetTime() > statusTimer.GetLimit()) {
			status = NONE;
			statusTimer.Reset();
		}
	}
}

void Notfredo::Render() {
	sp.Render(box.x - Camera::GetInstance().pos.x -1, box.y - Camera::GetInstance().pos.y -1);
}

void Notfredo::Damage(int damage) {
	if(iFrames.GetTime() < 0) {
		hitpoints -= (damage-defense);
		iFrames.Start();
	}
}

bool Notfredo::NotifyTileCollision(Face face) {
	TileMap* map = StageState::GetTileMap();
	unsigned limitX = ((box.x+box.w)/map->GetTileWidth() < map->GetWidth()) ? ((box.x+box.w)/map->GetTileWidth()) : (map->GetWidth());
	unsigned limitY = ((box.y+box.h)/map->GetTileHeight() < map->GetHeight()) ? ((box.y+box.h)/map->GetTileHeight()) : (map->GetHeight());
	if(face == LEFT) {
		for(unsigned x = box.x/map->GetTileWidth(); x <= limitX; x++) {
			for(unsigned y = box.y/map->GetTileHeight(); y <= limitY; y++) {
				if(map->At(x,y,0) > 5) {
					Rect tile = Rect(x*map->GetTileWidth(), y*map->GetTileHeight(), map->GetTileWidth(), map->GetTileHeight());
					if(box.x < tile.x+tile.w && box.x+box.w > tile.x+tile.w) {
						if(map->At(x,y-1,0) < 6 && map->At(x+1,y+1,0) > 5)
							speed.y = -0.46;
						box.x = tile.x+tile.w+1;
					}
				}
			}
		}
	}
	if(face == RIGHT) {
		for(unsigned x = box.x/map->GetTileWidth(); x <= limitX; x++) {
			for(unsigned y = box.y/map->GetTileHeight(); y <= limitY; y++) {
				if(map->At(x,y,0) > 5) {
					Rect tile = Rect(x*map->GetTileWidth(), y*map->GetTileHeight(), map->GetTileWidth(), map->GetTileHeight());
					if(box.x+box.w > tile.x && box.x < tile.x) {
						if(map->At(x,y-1,0) < 6 && map->At(x-1,y+1,0) > 5)
							speed.y = -0.46;
						box.x = tile.x-box.w-1;
					}
				}
			}
		}
	}
	if(face == UPPER) {
		for(unsigned x = box.x/map->GetTileWidth(); x <= limitX; x++) {
			for(unsigned y = box.y/map->GetTileHeight(); y <= limitY; y++) {
				if(map->At(x,y,0) > 5) {
					Rect tile = Rect(x*map->GetTileWidth(), y*map->GetTileHeight(), map->GetTileWidth(), map->GetTileHeight());
					if(box.y < tile.y+tile.h && box.y+box.h > tile.y+tile.h) {
						box.y = tile.y+tile.h+1;
						speed.y = 0.6;
					}
				}
			}
		}
	}
	if(face == BOTTOM) {
		for(unsigned x = box.x/map->GetTileWidth(); x <= limitX; x++) {
			for(unsigned y = box.y/map->GetTileHeight(); y <= limitY; y++) {
				if(map->At(x,y,0) > 5) {
					Rect tile = Rect(x*map->GetTileWidth(), y*map->GetTileHeight(), map->GetTileWidth(), map->GetTileHeight());
					if(box.y+box.h > tile.y && box.y < tile.y) {
						box.y = tile.y-box.h-1;
						speed.y = 0.6;
					}
				}
			}
		}
	}
	return false;
}

void Notfredo::NotifyCollision(GameObject* other) {
	if(other->Is("Attack") && other->facing == facing) {
		Damage(other->power);
	}else if(other->Is("Minifredo")) {
		status = STUNNED;
		statusTimer.Start();
	}

}

bool Notfredo::Is(std::string type) {
	return (type == "Notfredo");
}

bool Notfredo::IsDead() {
	return (hitpoints < 1);
}

Notfredo* Notfredo::get() {
	return this;
}

