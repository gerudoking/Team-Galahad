#ifndef NOTFREDO_H_
#define NOTFREDO_H_

#include "Notattack.h"

class Notfredo : public GameObject {
private:
	Rect radius = Rect();
	int hitpoints;
	Timer iFrames;
	Timer attackCD;
	Timer vigil;

public:
	bool mf = false;

	Notfredo(int x, int y);
	~Notfredo();
	void Update(float dt);
	void Render();
	void Damage(int damage);
	bool NotifyTileCollision(Face face);
	void NotifyCollision(GameObject* other);
	bool Is(std::string type);
	bool IsDead();
	Notfredo* get();
};

#endif /* NOTFREDO_H_ */
