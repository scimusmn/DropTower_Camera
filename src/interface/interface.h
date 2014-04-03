#pragma once

#include "../cameraImage.h"
#include "styleSheet.h"

class iFace {
protected:
	cameraImage * cam;
	ofFont title;
	ofFont inst;
	ofImage feed;
	ofImage rec;
	ofImage play;

	newSlider pos;
	ofButton playBut;
	ofButton videoPlay;

	bool redisplay;
	long timer;
public:
	iFace();
	~iFace();

	void init(cameraImage * camera);
	void update();
	void draw();
	void drawControls(int x, int y, int w, int h);
	void drawInstructions(int x, int y, int w, int h);
	void clickDown(int x, int y);
	void clickUp(int x, int y);
	void mouseMove(int x, int y);
	void buttonPress(int key);
};