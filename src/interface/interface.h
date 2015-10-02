#pragma once

#include "../cameraImage.h"
#include "styleSheet.h"
#include "experimentInstructions.h"

class iFace {
protected:
	cameraImage * cam;
	ofFont title;
	//ofFont inst;
	ofImage feed;
	ofImage rec;
	ofImage play;

	newSlider pos;
	ofButton videoPlay;

	ofButton langBut;

	dualLangImg selectTitle;
	dualLangImg liveFeed;
	dualLangImg recording;
	dualLangImg playback;

	ofImage tower;
	ofImage carriage;

	long raiseStartTime;
	//instGroup select;

	bool redisplay;
	long timer;
public:
	iFace();
	~iFace();

	void init(cameraImage * camera);
	void update();
	void draw();
	void drawTower(int x, int y, int w, int h);
	void drawCameraBox(int x, int y, int w, int h);
	void drawControls(int x, int y, int w, int h);
	void drawInstructions(int x, int y, int w, int h);
	void drawSelect(int x, int y, int w,int h);
	void startRaise();
	void clickDown(int x, int y);
	void clickUp(int x, int y);
	void mouseMove(int x, int y);
	void buttonPress(int key);
};