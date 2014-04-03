#include "styleSheet.h"

ofColor blck(3,3,3);
ofColor wht(250,250,250);
ofColor blu(0x00,0xB5,0xAD);
ofColor ltgry(100,100,100);
ofColor chrcl(51,51,51);
ofColor yllw(251,193,8);

void Black()
{
	ofSetColor(blck);
}

void Blue()
{
	ofSetColor(blu);
}

void LtGray()
{
	ofSetColor(ltgry);
}

void Charcoal()
{
	ofSetColor(chrcl);
}

void White()
{
	ofSetColor(wht);
}

void Yellow()
{
	ofSetColor(yllw);
}



ofColor & black()
{
	return blck;
}

ofColor & blue()
{
	return blu;
}
ofColor & ltGray()
{
	return ltgry;
}
ofColor & charcoal()
{
	return chrcl;
}
ofColor & white()
{
	return wht;
}

ofColor & yellow()
{
	return yllw;
}

ofFont lbl;

ofFont & label()
{
	return lbl;
}

void label(string str, int x, int y)
{
	lbl.drawString(str,x,y);
}


newSlider::newSlider(){
	bPressed=false;
	knob.setPressed(false);
}

newSlider::~newSlider(){

}

void newSlider::setup(string knobImg){
	if(knobImg.length()){
		knob.setup(64,64,knobImg);
	}
}

void newSlider::draw(int _x, int _y, int _w, int _h){
	x=_x,y=_y;
	w=_w-_h;
	h=_h;
	White();
	ofRoundedRect(x+knob.w/2,(y+knob.h/2)-8,_w-knob.w,_h/4,_h/8);
	knob.draw(x+knob.relPos.x,y,h,h);
}
bool newSlider::clickDown(int _x, int _y){
	bool ret=false;
	if(knob.clickDown(_x-knob.relPos.x, _y)){
		ret=bPressed=true;
	}
	else if(over(_x-h, _y)){
		knob.setPressed(true);
		knob.relPos.x=(_x-x<=w)?_x-x:w;
		ret=bPressed=true;
	}

	if(cbFunc) (*cbFunc)(this,dataPointer);

	return ret;
}

bool newSlider::clickUp(int x, int y){
	bool ret=bPressed;
	knob.clickUp();
	bPressed=false;
	return ret;
}

void newSlider::drag(int _x,int _y){
	if(bPressed||knob.pressed()){
		knob.relPos.x=(_x-x>=0)?(_x-x<=w)?_x-x:w:0;
		if(cbFunc) (*cbFunc)(this,dataPointer);
	}
}

void newSlider::setPercent(float perc){
	if(perc<0) perc=0;
	else if(perc>1) perc=1;
	knob.relPos.x=w*perc;
}

float newSlider::getPercent(){
	return knob.relPos.x/w;
}

void newSlider::setCallback(void (* fxn)(newSlider *,void *),void * dPoint){
	cbFunc = fxn;
	dataPointer = dPoint;
}