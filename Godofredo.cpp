#include "Godofredo.h"
#include "InputManager.h"
#include "Camera.h"
#include "StageState.h"

Godofredo* Godofredo::player = nullptr;

Godofredo::Godofredo(int x, int y) {
	if(player) {
		printf("Multiple Instances\n");
		exit(EXIT_FAILURE);
	}else{
		player = this;
	}

	sp = Sprite("img/godofredo.png", 4);
	box = Rect(x+1, y+1, sp.GetWidth()-2, sp.GetHeight()-2);
	speed = Vec2(0, 0.6);
	hitpoints = 10;
	power = 2;
	defense = 0;
	healthBar = Sprite("img/healthBar.png", 11);
	healthBar.SetFrame(11);
	iFrames = Timer(500);
	attackCD = Timer(500);
	stealthBar = Sprite("img/stealthBar.png", 11);
	stealthBar.SetFrame(1);
	hiddenTimer = Timer(1000);
	minifredo = new Minifredo();
	StageState::AddObject(minifredo);
	droneRespawn = Timer(3000);
}

Godofredo::~Godofredo() {
	player = nullptr;
}

void Godofredo::Update(float dt) {
	sp.SetFrame(1);

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
	if(hiddenTimer.GetTime() > -1) {
		if(hiddenTimer.GetTime() < hiddenTimer.GetLimit()) {
			hiddenTimer.Update(dt);
			stealthBar.SetFrame(round((hiddenTimer.GetLimit()-hiddenTimer.GetTime())/100)+1);
		}else{
			hiddenTimer.Reset();
			stealthBar.SetFrame(1);
			hidden = false;
		}
	}
	if(droneRespawn.GetTime() > -1) {
		if(droneRespawn.GetTime() == 0) {
			minifredo = nullptr;
		}
		droneRespawn.Update(dt);
		if(!(droneRespawn.GetTime() < droneRespawn.GetLimit())) {
			droneRespawn.Reset();
			minifredo = new Minifredo();
			StageState::AddObject(minifredo);
		}
	}
	if(InputManager::GetInstance().KeyPress(SDLK_q)) {
		if(minifredo) {
			if(mf) {
				mf = false;
				minifredo->mf = false;
				Camera::GetInstance().Follow(this);

			}else{
				mf = true;
				minifredo->mf = true;
				Camera::GetInstance().Follow(minifredo);
			}
		}
	}
	if(!mf) {
		if(InputManager::GetInstance().IsKeyDown(SDLK_d)) {
			if(speed.x < 0.4)
				speed.x += 0.002*dt;
			else
				speed.x = 0.4;
			facing = RIGHT;
			sp.SetFrame(3);
		}else if(InputManager::GetInstance().IsKeyDown(SDLK_a)) {
			if(speed.x > -0.4)
				speed.x -= 0.002*dt;
			else
				speed.x = -0.4;
			facing = LEFT;
			sp.SetFrame(2);
		}else{
			if(speed.x > 0) {
				speed.x -= 0.001*dt;
				if(speed.x < 0)
					speed.x = 0;
			}else{
				speed.x += 0.001*dt;
				if(speed.x > 0)
					speed.x = 0;
			}
		}
		if(InputManager::GetInstance().IsKeyDown(SDLK_e)) {
			if(attackCD.GetTime() < 0) {
				if(facing == RIGHT)
					StageState::AddObject(new Attack("img/notattack.png", 2, box.x+box.w-16, box.y, power, 100, facing));
				else
					StageState::AddObject(new Attack("img/notattack.png", 2, box.x+16, box.y, power, 100, facing));
				attackCD.Start();
			}
		}
		wallJumping = 0;
		if(jumping) {
			NotifyTileCollision(WALLJUMP);
			printf("%i", wallJumping);
		}
		if(InputManager::GetInstance().KeyPress(SDLK_SPACE)) {
			if(!jumping) {
				jumping = true;
				speed.y = -0.6;
				if(wallJumping == 1)
					speed.x = 0.4;
				if(wallJumping == 2)
					speed.x = -0.4;
			}
		}
		if(InputManager::GetInstance().IsKeyDown(SDLK_s)) {
			NotifyTileCollision(SPECIAL);
		}
	}
	if(hidden) {
		sp.SetFrame(4);
	}

	speed.y += 0.002*dt;

	box.x += speed.x*dt;

	if(speed.x > 0)
		NotifyTileCollision(RIGHT);
	else
		NotifyTileCollision(LEFT);

	box.y += speed.y*dt;
	if(speed.y > 0)
		NotifyTileCollision(BOTTOM);
	else
		NotifyTileCollision(UPPER);
}

void Godofredo::Render() {
	sp.Render(box.x - Camera::GetInstance().pos.x -1, box.y - Camera::GetInstance().pos.y -1);
	healthBar.Render(5, 5);
	stealthBar.Render(5, 10+stealthBar.GetHeight());
}

void Godofredo::Damage(int damage) {
	if(iFrames.GetTime() < 0) {
		hitpoints -= (damage-defense);
		healthBar.SetFrame(hitpoints+1);
		iFrames.Start();
	}
}

bool Godofredo::NotifyTileCollision(Face face) {
	TileMap* map = StageState::GetTileMap();
	unsigned limitX = ((box.x+box.w)/map->GetTileWidth() < map->GetWidth()) ? ((box.x+box.w)/map->GetTileWidth()) : (map->GetWidth());
	unsigned limitY = ((box.y+box.h)/map->GetTileHeight() < map->GetHeight()) ? ((box.y+box.h)/map->GetTileHeight()) : (map->GetHeight());
	if(face == LEFT) {
		for(unsigned x = box.x/map->GetTileWidth(); x <= limitX; x++) {
			for(unsigned y = box.y/map->GetTileHeight(); y <= limitY; y++) {
				if(map->At(x,y,0) > 5) {
					printf("%i %i\n", x, y);
					Rect tile = Rect(x*map->GetTileWidth(), y*map->GetTileHeight(), map->GetTileWidth(), map->GetTileHeight());
					if(box.x < tile.x+tile.w && box.x+box.w > tile.x+tile.w) {
						box.x = tile.x+tile.w+1;
						if(map->At(x,y,0) > 11)
							Damage(1);
						return true;
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
						box.x = tile.x-box.w-1;
						if(map->At(x,y,0) > 11)
							Damage(1);
						return true;
					}
				}
			}
		}
	}
	if(face == UPPER || face == BOTTOM) {
		for(unsigned x = box.x/map->GetTileWidth(); x <= limitX; x++) {
			for(unsigned y = box.y/map->GetTileHeight(); y <= limitY; y++) {
				if(map->At(x,y,0) > 5) {
					Rect tile = Rect(x*map->GetTileWidth(), y*map->GetTileHeight(), map->GetTileWidth(), map->GetTileHeight());
					if(box.y < tile.y+tile.h && box.y+box.h > tile.y+tile.h) {
						box.y = tile.y+tile.h+1;
						speed.y = 0.6;
					}else if(box.y+box.h > tile.y && box.y < tile.y) {
						box.y = tile.y-box.h-1;
						speed.y = 0.6;
						jumping = false;
					}
					if(map->At(x,y,0) > 11)
						Damage(1);
				}
			}
		}
	}
	if(face == SPECIAL) {
		for(unsigned x = box.x/map->GetTileWidth(); x <= limitX; x++) {
			for(unsigned y = box.y/map->GetTileHeight(); y <= limitY; y++) {
				if(map->At(x,y,1) == 0) {
					Rect tile = Rect(x*map->GetTileWidth(), y*map->GetTileHeight(), map->GetTileWidth(), map->GetTileHeight());
					if((((box.x+box.w >= tile.x) && (box.x <= tile.x+tile.w))
					   && ((box.y+box.h >= tile.y) && (box.y <= tile.y+tile.h)))) {
							hiddenTimer.Start();
							hidden = true;
					}
				}
			}
		}
	}
	if(face == WALLJUMP) {
		for(unsigned x = box.x/map->GetTileWidth(); x <= limitX; x++) {
			for(unsigned y = box.y/map->GetTileHeight(); y <= limitY; y++) {
				printf("%i %i\n", x, y);
				if(map->At(x,y,0) > 5) {
					Rect tile = Rect(x*map->GetTileWidth(), y*map->GetTileHeight(), map->GetTileWidth(), map->GetTileHeight());
					printf("got here");
					if(box.x < tile.x+tile.w && box.x+box.w > tile.x+tile.w) {
						box.x = tile.x+tile.w+1;
						if(map->At(x,y,0) > 11)
							Damage(1);
						jumping = false;
						wallJumping = 1;
					}
					else if(box.x+box.w > tile.x && box.x < tile.x) {
						box.x = tile.x-box.w-1;
						if(map->At(x,y,0) > 11)
							Damage(1);
						jumping = false;
						wallJumping = 2;
					}
				}
			}
		}
	}
	return false;
}

void Godofredo::NotifyCollision(GameObject* other) {
	if(other->Is("Notattack"))
		Damage(other->power);
}

bool Godofredo::Is(std::string type) {
	return (type == "Godofredo");
}

bool Godofredo::IsHidden() {
	return hidden;
}

bool Godofredo::IsDead() {
	return (hitpoints < 1);
}

Godofredo* Godofredo::get() {
	return this;
}

