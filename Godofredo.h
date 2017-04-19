#ifndef GODOFREDO_H_
#define GODOFREDO_H_

#include "Minifredo.h"
#include "Attack.h"

class Godofredo : public GameObject{
private:
	int hitpoints;
	Sprite healthBar;
	Timer iFrames;
	Timer attackCD;
	bool jumping = false;
	int wallJumping = 0;
	Sprite stealthBar;
	Timer hiddenTimer;
	bool hidden = false;

public:
	static Godofredo* player;

	Minifredo* minifredo;
	Timer droneRespawn;
	bool mf = false;

	Godofredo(int x, int y);
	~Godofredo();
	void Update(float dt);
	void Render();
	void Damage(int damage);
	bool NotifyTileCollision(Face face);
	void NotifyCollision(GameObject* other);
	bool Is(std::string type);
	bool IsHidden();
	bool IsDead();
	Godofredo* get();
};

#endif /* GODOFREDO_H_ */
