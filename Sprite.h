#ifndef SPRITE_H_
#define SPRITE_H_

#include "Resources.h"

class Sprite {
private:
	SDL_Texture* texture;
	SDL_Rect clipRect;
	SDL_Rect dstRect;
	float scaleX = 1;
	float scaleY = 1;
	int frameCount = 1;
	int currentFrame = 1;
	float frameTime = 0;
	float timeElapsed = 0;
	int width = 0;
	int height = 0;

public:
	Sprite();
	Sprite(std::string file, int frameCount = 1, float frameTime = 1);
	~Sprite();
	void Open(std::string file);
	void Update(float dt);
	void Render(int x, int y, float angle = 0);
	void SetClip(int x, int y, int w, int h);
	void SetScaleX(float scale);
	void SetScaleY(float scale);
	void SetFrame(int frame);
	void SetFrameCount(int frameCount);
	void SetFrameTime(float frameTime);
	int GetWidth();
	int GetHeight();
	bool IsOpen();
};

#endif /* SPRITE_H_ */
