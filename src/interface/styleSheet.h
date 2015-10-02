#pragma once
#include "ofMain.h"
#include "ofExtended.h"

void Black();
void Charcoal();
void MedGray();
void LtGray();
void Blue();
void White();
void Yellow();

ofColor & ltGray();
ofColor & medGray();
ofColor & black();
ofColor & charcoal();
ofColor & blue();
ofColor & white();
ofColor & yellow();

//ofFont & label();
//void label(string str, int x, int y);

class newSlider : public ofInterObj{
	void * dataPointer;
	void (* cbFunc)(newSlider *,void *);
	int startPos;
	bool wasPressed;
public:
	ofButton knob;
	ofImage track;
	newSlider();
	~newSlider();

	void setup(string playImg,string pauseImg);

	void draw(int x, int y, int w, int h);
	bool clickDown(int x, int y);
	bool clickUp(int x, int y);
	void drag(int x,int y);
	void setPercent(float perc);
	float getPercent();
	void setCallback(void (* fxn)(newSlider *,void *),void * dPoint);
};