#include "testApp.h"
#include "config.h"

//--------------------------------------------------------------
void testApp::setup(){	 
	cfg().setup();

	

	int midWid=cfg().camHgt;
	int midHgt=cfg().camWid;
	
	cam.init();
	inter.init(&cam);
}

//--------------------------------------------------------------
void testApp::update(){
	
	//if(cam.update()){
		inter.update();
	//}
	
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(255,255,255);
	inter.draw();
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	if(key=='r'){
		//cam.record();
		cam.startRecording();
	}
	if(key==' '){
		cam.reset();
		cam.play();
	}
	if(key=='b'){
		cam.reset();
	}
	if(key=='l'){
		inter.startRaise();
	}
	if(key=='t') cout << cam.camera.getExposureTime() << endl;
	if(key=='g') cam.camera.grab();
	if(!cam.isLive()){
		if(key==',') cam.pause(), cam.buffers.prevFrame();
		if(key=='.') cam.pause(), cam.buffers.nextFrame();
	}
	if(key==27){
		cout << "closing...\n";
		cam.close();
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	inter.mouseMove(x,y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	inter.clickDown(x,y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	inter.clickUp(x,y);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::exit(){
	//cam.onExit();
	cam.close();
}
