#ifndef TITLESTATE_H_
#define TITLESTATE_H_

#include "State.h"

class TitleState : public State {
private:

public:
	TitleState();
	~TitleState();
	void LoadAssets();
	void Update();
	void Render();
	bool QuitRequested();
	bool Is(std::string type);
	TitleState* get();
};

#endif /* TITLESTATE_H_ */
