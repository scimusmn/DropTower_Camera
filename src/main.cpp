#include "ofMain.h"
#include "testApp.h"
//#include "viewWorksHS.h"
//#include "threadedImageSave\threadImageSave.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1680,1050,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

	//ofTexture			videoTexture;
	/*ofThreadImageSave	save;
	highSpeedCam		cam;
	long timer=0;

	cam.init();
	//videoTexture.allocate(cam.width, cam.height, GL_BGR);
	//imageSave.allocate(cam.width, cam.height, OF_IMAGE_COLOR);
	cam.open();

	//videoTexture.allocate(cam.getWidth(), cam.getHeight(), GL_RGB);
	cout << cam.getWidth() << " is the width" << endl;
	timer=0;
	cam.testImage(TESTIMAGE_OFF);
	//cam.snap(1);
	cam.grab();

	int cnt=0;

	while(1){
		if(cam.newFrame){
			cam.newFrame=false;
			save.saveImage(cam.currentFrame(),cam.getWidth(),cam.getHeight(),(++cnt%=200));
			cout << cnt << " is the current image" << endl;
		}
	}*/
}
