#include "interface.h"

iFace::iFace(){
	cam=NULL;
}

iFace::~iFace(){

}

void sliderCB (newSlider * sld, void * camP){
	cameraImage * cam = (cameraImage *)camP;
	cam->gotoPercent(sld->getPercent());
}

void iFace::init(cameraImage * camera){
	cam = camera;
	title.loadFont("fonts/Aller_Bd.ttf");
	title.setSize(24);
	title.setMode(OF_FONT_TOP);

	inst.loadFont("fonts/Aller_Lt.ttf");
	inst.setSize(24);
	inst.setMode(OF_FONT_TOP);

	feed.loadImage("img/feed.png");
	play.loadImage("img/play.png");
	rec.loadImage("img/record.png");

	playBut.setup(64,64,"buttons/pause.png","buttons/play.png");
	videoPlay.setup(256,256,"buttons/videoPlay.png");
	pos.setup("buttons/knob.png");
	pos.setCallback(sliderCB,cam);

	timer=0;
	redisplay=false;
}

void iFace::update(){
	playBut.setPressed(!cam->isPlaying());
	if(cam->isPlaying()) redisplay=false;
	pos.setPercent(cam->getPercentage());
}

void iFace::draw(){
	//Box behind camera
	//Charcoal();
	//ofRoundedRect(cam->x-80,cam->y-80,cam->w+160,ofGetHeight()-(cam->y-80)-70,8);
	Black();
	//ofRect(cam->x-75,cam->y-75,cam->w+150,ofGetHeight()-(cam->y-75)-75);
	ofRect(0,150,ofGetWidth()/2,ofGetHeight()-150);

	Charcoal();
	ofRect(cam->x-12,cam->y-12,cam->w+24,cam->h+24);
	LtGray();
	ofRect(cam->x-10,cam->y-10,cam->w+20,cam->h+20);
	Charcoal();
	ofRect(cam->x-2,cam->y-2,cam->w+4,cam->h+4);

	//Controls, if present
	if(!cam->isLive()&&!cam->isRecording()){
		drawControls(cam->x,cam->y+cam->h+30,cam->w,150);
	}
	else redisplay=true;

	//Camera image
	ofColor(0xffffff);
	cam->draw((ofGetWidth()/2-cam->w)/2,(ofGetHeight()-cam->h)/2);

	White();
	title.setSize(12);
	if(cam->isLive()&&!cam->isRecording()){
		feed.draw(cam->x+10,cam->y+10,16,16);
		title.drawString("LIVE",cam->x+26,cam->y+10);
	}
	else if(cam->isRecording()){
		rec.draw(cam->x+10,cam->y+10,16,16);
		title.drawString("RECORDING",cam->x+26,cam->y+10);
	}
	else{
		play.draw(cam->x+10,cam->y+10,16,16);
		title.drawString("PLAYBACK",cam->x+26,cam->y+10);
		if(!cam->isPlaying()&&redisplay) videoPlay.draw(cam->x+(cam->w-videoPlay.w)/2,cam->y+(cam->h-videoPlay.h)/2);
	}

	Yellow();
	title.setSize(24);
	title.drawString("Experiment Carriage Camera",cam->x,cam->y-75);

	//Title bar
	Black();
	ofRect(0,0,ofGetWidth(),150);
	ofSetColor(75,75,75);
	ofRect(0,150,ofGetWidth(),3);

	Yellow();
	title.setSize(48);
	title.drawString("Falling Makes You Weightless",50,50);

	//Center bar
	/*LtGray();
	ofRect(ofGetWidth()/2,170,3,ofGetHeight()-190);
	Charcoal();
	ofRect(ofGetWidth()/2,172,-1,ofGetHeight()-194);*/

	drawInstructions(ofGetWidth()/2,153,ofGetWidth()/2,ofGetHeight()-153);
}

void iFace::drawControls(int x, int y, int w, int h){
	y+=(h-64)/2;
	playBut.draw(x,y);
	pos.draw(x+playBut.w,y,w-playBut.w,64);
}

void iFace::drawInstructions(int x, int y, int w, int h){
	Charcoal();
	ofRect(x,y,w,h);

	int textPos = (cam->y-75);

	Yellow();
	title.setSize(36);
	title.drawString("Use the drop tower",x+100,textPos+=title.stringHeight("kjhg")*1.1);
	title.drawString("to experiment with",x+100,textPos+=title.stringHeight("kjhg")*1.1);
	title.drawString("weightlessness.",x+100,textPos+=title.stringHeight("kjhg")*1.1);

	textPos+=100;
	White();
	inst.setSize(24);
	if(cam->isLive()&&!cam->isRecording()){
		inst.drawString("Experiment is being raised.",x+100,textPos+=inst.stringHeight("kjhg")*1.1);
		textPos+=inst.stringHeight("kjhg")*1.1;
		inst.drawString("Press DROP button once",x+100,textPos+=inst.stringHeight("kjhg")*1.1);
		inst.drawString("experiment reaches the top.",x+100,textPos+=inst.stringHeight("kjhg")*1.1);
	}
	else if(cam->isRecording()){
		inst.drawString("Experiment is dropping",x+100,textPos+=inst.stringHeight("kjhg")*1.1);
		inst.drawString("and video is being recorded.",x+100,textPos+=inst.stringHeight("kjhg")*1.1);
	}
	else{
		inst.drawString("Play back video using controls to the left.",x+100,textPos+=inst.stringHeight("kjhg")*1.1);
		textPos+=inst.stringHeight("kjhg")*1.1;
		inst.drawString("Close experiment door",x+100,textPos+=inst.stringHeight("kjhg")*1.1);
		inst.drawString("and press RAISE button",x+100,textPos+=inst.stringHeight("kjhg")*1.1);
		inst.drawString("when ready for the next experiment.",x+100,textPos+=inst.stringHeight("kjhg")*1.1);
	}
}

void iFace::clickDown(int x, int y){
	if(pos.clickDown(x,y)){
		cam->pause();
		playBut.setPressed(true);
	}
	if(playBut.toggle(x,y)){
		if(!playBut.pressed()){
			cam->play();
			if(cam->getPercentage()>.9) cam->reset();
		}
		else cam->pause();
	}
	if(videoPlay.clickDown(x,y)&&!cam->isPlaying()){
		redisplay=false;
		cam->play();
		playBut.setPressed(false);
	}
}

void iFace::clickUp(int x, int y){
	pos.clickUp(x,y);
	videoPlay.clickUp();
}

void iFace::mouseMove(int x, int y){
	pos.drag(x,y);
	//cam->gotoPercent(pos.getPercent());
}

void iFace::buttonPress(int key){

}