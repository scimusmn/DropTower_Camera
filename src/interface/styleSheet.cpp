#include "styleSheet.h"

ofColor blck(3,3,3);
ofColor wht(250,250,250);
ofColor blu(0,111,151);
ofColor dkblu(0,88,120);
ofColor ltgry(192,191,191);
ofColor medgry(103,103,103);
ofColor chrcl(21,19,20);
ofColor yllw(230,182,49);

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

void MedGray()
{
	ofSetColor(medgry);
}

void DkBlue()
{
	ofSetColor(dkblu);
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

ofColor & dkBlue()
{
	return blu;
}

ofColor & ltGray()
{
	return ltgry;
}

ofColor & medGray()
{
	return medgry;
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
	startPos=0;
	wasPressed=false;
}

newSlider::~newSlider(){

}

void newSlider::setup(string playImg, string pauseImg){
	if(playImg.length()){
		knob.setup(128,128,playImg,pauseImg);
	}
	track.loadImage("img/VideoTrack.png");
}

void newSlider::draw(int _x, int _y, int _w, int _h){
	x=_x,y=_y;
	w=_w-_h;
	h=_h;
	White();
	//ofRoundedRect(x+knob.w/2,(y+knob.h/2)-8,_w-knob.w,_h/4,_h/8);
	track.draw(x,y,_w,_h);
	knob.draw(x+knob.relPos.x,y,h,h);
}
bool newSlider::clickDown(int _x, int _y){
	bool ret=false;
	if(knob.over(_x, _y)){
		wasPressed = !knob.pressed();
		knob.setPressed(true);
		ret=bPressed=true;
		startPos=_x;
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
	bool ret=false;
	//knob.clickUp();
	if(startPos>0&&abs(startPos-x)<10&&wasPressed){
		ret=!knob.pressed();
		knob.setPressed(ret);
	}
	startPos=-1;
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