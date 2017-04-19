#include "Sprite.h"
#include "Game.h"

Sprite::Sprite() {
	texture = nullptr;
}

Sprite::Sprite(std::string file, int frameCount, float frameTime){
	texture = nullptr;
	Sprite::frameCount = frameCount;
	Sprite::frameTime = frameTime;
	Open(file);
}

Sprite::~Sprite() {
	texture = nullptr;
}

void Sprite::Open(std::string file) {
	texture = Resources::GetImage(file);
	if(!IsOpen()) {
		printf("IMG_LoadTexture failed: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
	SetClip((width/frameCount)*(currentFrame-1), 0, width/frameCount, height);
}

void Sprite::Update(float dt) {
	timeElapsed += dt;
	if(timeElapsed > frameTime) {
		if(currentFrame < frameCount)
			currentFrame++;
		else
			currentFrame = 1;
		SetClip((width/frameCount)*(currentFrame-1), 0, width/frameCount, height);
		timeElapsed = 0;
	}
}

void Sprite::Render(int x, int y, float angle) {
	dstRect.w = clipRect.w*scaleX;
	dstRect.h = clipRect.h*scaleY;
	dstRect.x = x - ((dstRect.w-clipRect.w)/2);
	dstRect.y = y - ((dstRect.h-clipRect.h)/2);
	SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture, &clipRect, &dstRect, angle, nullptr, SDL_FLIP_NONE);
}

void Sprite::SetClip(int x, int y, int w, int h) {
	clipRect.x = x;
	clipRect.y = y;
	clipRect.w = w;
	clipRect.h = h;
}

void Sprite::SetScaleX(float scale) {
	scaleX = scale;
}

void Sprite::SetScaleY(float scale) {
	scaleY = scale;
}

void Sprite::SetFrame(int frame) {
	currentFrame = frame;
	SetClip((width/frameCount)*(currentFrame-1), 0, width/frameCount, height);
}

void Sprite::SetFrameCount(int frameCount) {
	Sprite::frameCount = frameCount;
}

void Sprite::SetFrameTime(float frameTime) {
	Sprite::frameTime = frameTime;
}

int Sprite::GetWidth() {
	return width/frameCount*scaleX;
}

int Sprite::GetHeight() {
	return height*scaleY;
}

bool Sprite::IsOpen() {
	return (!texture) ? false : true;
}
