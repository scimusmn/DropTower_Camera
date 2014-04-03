#pragma once
#include "ofMain.h"
#include "ofExtended.h"

void Black();
void Charcoal();
void LtGray();
void Blue();
void White();
void Yellow();

ofColor & ltGray();
ofColor & black();
ofColor & charcoal();
ofColor & blue();
ofColor & white();
ofColor & yellow();

ofFont & label();
void label(string str, int x, int y);

class newSlider : public ofInterObj{
	ofButton knob;
	void * dataPointer;
	void (* cbFunc)(newSlider *,void *);
public:
	newSlider();
	~newSlider();

	void setup(string knobImg);

	void draw(int x, int y, int w, int h);
	bool clickDown(int x, int y);
	bool clickUp(int x, int y);
	void drag(int x,int y);
	void setPercent(float perc);
	float getPercent();
	void setCallback(void (* fxn)(newSlider *,void *),void * dPoint);
};