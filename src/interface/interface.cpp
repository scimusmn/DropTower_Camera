#include "interface.h"
#include "../config.h"

//constructor: set cam pointer to null on startup
iFace::iFace(){
	cam=NULL;
}

iFace::~iFace(){

}

//function called when the video scrub bar is moved
void sliderCB (newSlider * sld, void * camP){
	cameraImage * cam = (cameraImage *)camP;
	cam->gotoPercent(sld->getPercent());
}

//initialize the interface with the camera info and load fonts and images
void iFace::init(cameraImage * camera){
	cam = camera;
	title.loadFont("fonts/Aller_Bd.ttf");
	title.setSize(12);
	title.setMode(OF_FONT_TOP);

	/*inst.loadFont("fonts/Aller_Lt.ttf");
	inst.setSize(24);
	inst.setMode(OF_FONT_TOP);*/

	feed.loadImage("img/feed.png");
	play.loadImage("img/play.png");
	rec.loadImage("img/record.png");

	selectTitle.load("img/SelectTitle-english.png","img/SelectTitle-espanol.png");
	liveFeed.load("img/VideoStateLiveFeed-english.png","img/VideoStateLiveFeed-espanol.png");
	recording.load("img/VideoStateRecording-english.png","img/VideoStateRecording-espanol.png");
	playback.load("img/VideoTitle-english.png","img/VideoTitle-espanol.png");

	langBut.setup(128,128,"img/LanguageButton-english.png","img/LanguageButton-espanol.png");

	tower.loadImage("img/Tower.png");
	carriage.loadImage("img/TowerCaddy.png");

	videoPlay.setup(256,256,"buttons/videoPlay.png");
	pos.setup("img/VideoPlay.png","img/VideoPause.png");
	pos.setCallback(sliderCB,cam);

	timer=0;
	redisplay=false;
}

//function called during the app update function
void iFace::update(){
	//playBut.setPressed(!cam->isPlaying());

	//if the stored images are being played from the camera object,
	if(cam->update()){
		//set the scrub knob to display either the play or pause symbol.
		pos.knob.setPressed(cam->isPlaying());
		//if the video is already playing, disable the big play arrow over the screen
		if(cam->isPlaying()) redisplay=false;
		//set the scrubber knob to the current playback percentage
		pos.setPercent(cam->getPercentage());
	}

	//test options:: enable the autoMode flag in the config file to auto-run.
	if(cfg().autoMode){
		if(cam->camera.ready()&&raiseStartTime+(cfg().raiseTime+1.)*1000<ofGetElapsedTimeMillis()&&cam->isLive()&&!cam->isRecording()){
			cam->startRecording();
		}
		else if(!cam->isLive()&&!cam->isRecording()&&!cam->isPlaying()&&!cam->autoPlayLock){
			startRaise();
		}
	}

}

//draw the tower graphic at the specified location
void iFace::drawTower(int x, int y, int w, int h){
	//box to hold the carriage
	ofRectangle car(x,y,w,h/16);
	int travelDist = h-car.height*2;
	int offset = car.height/2;
	if(cam->isRecording()){
		int posi = y+offset+pow(cam->recordingPercent(),2)*travelDist;
		carriage.draw(car.x,posi,car.width,car.height);
	}
	else if(cam->isLive()){
		if(raiseStartTime+cfg().raiseTime*1000>ofGetElapsedTimeMillis()){
			int posi = y+offset+travelDist*(1-(float(ofGetElapsedTimeMillis())-float(raiseStartTime))/float(cfg().raiseTime*1000.));
			carriage.draw(car.x,posi,car.width,car.height);
		}
		else carriage.draw(car.x,y+offset,car.width,car.height);
	}
	else {
		//int posi = y+offset+pow(1.,2)*travelDist;
		//carriage.draw(car.x,posi,car.width,car.height);
		float perc = min(1,cam->buffers.getPercentDone()/cfg().fallPercent);
		int posi = y+offset+pow(perc,2)*travelDist;
		carriage.draw(car.x,posi,car.width,car.height);
	}
	tower.draw(x,y,w,h);
}

void iFace::drawCameraBox(int x, int y, int w, int h){
	ofPoint pad(20,20);
	LtGray();
	ofRect(x,y,w,h);

	ofRectangle ttlBox(x+pad.x,y+pad.y,w-pad.x*2,h/20);
	ofRectangle camBox(x+pad.x,ttlBox.y+ttlBox.height+pad.y,w-pad.x*2,h*.75);
	ofRectangle controls(x+pad.x,camBox.y+camBox.height+pad.y,w-pad.x*2,(h-(camBox.y+camBox.height))-pad.y);

	Black();
	ofRect(ttlBox);
	ofRect(camBox);
	ofRect(controls);

	//Camera image
	MedGray();
	ofRect(cam->x-pad.x/20,cam->y-pad.y/20,cam->w+pad.x/10,cam->h+pad.y/10);
	cam->draw(camBox.x+pad.x,camBox.y+(camBox.height-cam->h)/2,w*.7,((w*.7)/cam->image.width)*cam->image.height);

	drawTower(camBox.x+camBox.width*.425+cam->w/2,camBox.y+camBox.height/20,camBox.width*.15,camBox.height*.9);

	White();
	//title.setSize(12);
	playback.draw(ttlBox.x,ttlBox.y,ttlBox.width,ttlBox.height);
	if(cam->isLive()&&!cam->isRecording()){
		liveFeed.draw(controls.x,controls.y,controls.width,controls.height);
	}
	else if(cam->isRecording()){
		recording.draw(controls.x,controls.y,controls.width,controls.height);
	}
	else{
		drawControls(controls.x,controls.y,controls.width,controls.height);
		if(!cam->isPlaying()&&redisplay) videoPlay.draw(cam->x+(cam->w-videoPlay.w)/2,cam->y+(cam->h-videoPlay.h)/2);
	}

	//Controls, if present
	if(!cam->isLive()&&!cam->isRecording()){
		
	}
	else redisplay=true;
}

void iFace::drawSelect(int x, int y, int w, int h){
	ofPoint pad(20,20);
	MedGray();
	ofRect(x,y,w,h);
	Black();
	ofRect(x+pad.x,y+pad.y,w-pad.x*2,h/8);
	White();
	selectTitle.draw(x+pad.x,y+pad.y,w-pad.x*2,h/8);
	select().drawButtons(x+pad.x,y+pad.y*2+h/8,w-pad.x*2,h-(pad.y*3+h/8),pad.x);
}

void iFace::draw(){
	//Box behind camera
	//Charcoal();
	//ofRoundedRect(cam->x-80,cam->y-80,cam->w+160,ofGetHeight()-(cam->y-80)-70,8);
	ofPoint pad(20,20);

	ofRectangle camBox(pad.x,pad.y,ofGetWidth()*.5,ofGetWidth()*.5);
	ofRectangle instBox(camBox.x+camBox.width+pad.x,pad.y,(ofGetWidth()-(camBox.x+camBox.width+pad.x))-pad.x,ofGetHeight()/2);

	Charcoal();
	ofRect(0,0,ofGetWidth(),ofGetHeight());

	drawCameraBox(camBox.x,camBox.y,camBox.width,camBox.height);

	if(select().insts.size()>1){
		drawInstructions(instBox.x,instBox.y,instBox.width,instBox.height);
	}
	else drawInstructions(instBox.x,instBox.y,instBox.width,camBox.height);

	if(select().insts.size()>1){
		drawSelect(instBox.x,instBox.y+instBox.height+pad.y,instBox.width,((camBox.y+camBox.height)-(instBox.y+instBox.height))-pad.y);
	}

	White();
	langBut.draw(ofGetWidth()-(pad.x+langBut.w),ofGetHeight()-(pad.y+langBut.h),ofGetWidth()*.104,ofGetHeight()*.063);
}

void iFace::drawControls(int x, int y, int w, int h){
	ofPoint pad(20,20);
	//playBut.draw(x,y);
	pos.draw(x,y,w,h);
}

void iFace::drawInstructions(int x, int y, int w, int h){
	Blue();
	ofRect(x,y,w,h);

	White();
	select().drawInst(x,y,w,h);
}

void iFace::startRaise(){
	cam->camera.grab();
	cam->liveFeed();
	raiseStartTime=ofGetElapsedTimeMillis();
}

void iFace::clickDown(int x, int y){
	if(pos.clickDown(x,y)){
		cam->pause();
		pos.knob.setPressed(false);
	}
	/*if(playBut.toggle(x,y)){
		if(!playBut.pressed()){
			cam->play();
			if(cam->getPercentage()>.9) cam->reset();
		}
		else cam->pause();
	}*/
	if(videoPlay.clickDown(x,y)&&!cam->isPlaying()&&!cam->isLive()){
		redisplay=false;
		cam->play();
		pos.knob.setPressed(true);
	}
	select().clickDown(x,y);
	if(langBut.toggle(x,y)){
		cfg().english = langBut.pressed();
	}
}

void iFace::clickUp(int x, int y){
	if(pos.clickUp(x,y)){
		if(cam->getPercentage()>.9) cam->reset();
		cam->play();
	}
	videoPlay.clickUp();
}

void iFace::mouseMove(int x, int y){
	pos.drag(x,y);
	//cam->gotoPercent(pos.getPercent());
}

void iFace::buttonPress(int key){

}