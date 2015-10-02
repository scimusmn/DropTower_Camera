#include "cameraImage.h"
#include "config.h"


//constructor
cameraImage::cameraImage(){
	bLive = true;
	bRecord = false;
	bPlay=false;
	liveBuffer = 0;
	colorCorrect = 0;
}

//deconstructor-- if I try to actually delete any of the pointers, the program starts throwing errors.
cameraImage::~cameraImage(){
	/*if( liveBuffer != NULL )
	{
		delete [] liveBuffer;
		liveBuffer = NULL;
	}
	if( colorCorrect != NULL )
	{
		delete [] colorCorrect;
		colorCorrect = NULL;
	}*/
	bAvailable=false;
}


//This is the callback function which is called when an image is sent from the camera.
void captureImage( OBJECT_INFO* pObjectInfo, IMAGE_INFO* pImageInfo ){
	cameraImage* img = (cameraImage*)pObjectInfo->pUserPointer;
	if(!img) return;
	img->store(pImageInfo);
}

//Initiatlizes the camera object
void cameraImage::init(){
	camera.init();

	//store this object for use in the callback
	objectInfo.pUserPointer=this;
	camera.open(&captureImage,&objectInfo);

	camera.getImageSize();

	if(camera.bufferSize){
		liveBuffer = new BYTE[camera.bufferSize];
		colorCorrect = new BYTE[camera.bufferSize];
		buffers.allocate(250,camera.bufferSize);
	}

	//reserve memory space for the camera frame information
	image.allocate(camera.width, camera.height, OF_IMAGE_COLOR);
	timer = ofGetElapsedTimeMillis()+50;
	autoPlayTimer = 0;
	autoPlayLock = false;

	camera.testImage(TESTIMAGE_OFF);
	camera.grab();
	//while(!camera.isCapturing()) camera.grab();
}

//function to update the currently displayed image, either from the stream or from the saved frames.
bool cameraImage::update(){
	bool ret=false;

	//if it's time to change the displayed image, and the camera is open and ready
	if(ofGetElapsedTimeMillis()>timer&&camera.ready()){
		//set the camera to capture if it isn't already
		if(!camera.isCapturing()&&bLive) camera.grab(), cout << "grab!\n";
		//else if(camera.isCapturing()&&!bLive) camera.stopGrab(), cout << "stop!\n";

		//reset the display timer, based on if the camera view is live or not
		timer=ofGetElapsedTimeMillis()+((bLive)?20:50);
		PBYTE temp = stream();

		//if the buffer is valid, dump the image data into the image container
		if(temp) image.setFromPixels(temp,camera.width,camera.height,OF_IMAGE_COLOR,true);

		//if we are playing the recorded video
		if(!bLive&&bPlay){
			//if we are at the end of recorded frames, stop playback; also queues the next frame
			bPlay=buffers.nextFrame();
			//if the "loop play" variable is set in the config file, loop playback at the end
			if(cfg().loopVideo&&!bPlay) bPlay=true, reset();
		}
		//if we are playing images back, set the return value to true.
		if(!bLive) ret=true;
	}

	//if autoplay is enabled, trigger playback after the autoplay delay time.
	if(cfg().autoPlay&&autoPlayLock&&ofGetElapsedTimeMillis()>autoPlayTimer){
		play();
		autoPlayLock=false;
	}
	return ret;
}

//function to draw the camera image, called in the interface draw camera box
void cameraImage::draw(int _x, int _y){
	x=_x,y=_y,w=image.width*1.25,h=image.height*1.25;
	ofSetColor(255,255,255);
	image.draw(_x,_y,w,h);
}

// function to draw a scaled image from the camera
void cameraImage::draw(int _x, int _y, int _w, int _h){
	x=_x,y=_y,w=_w,h=_h;
	ofSetColor(255,255,255);
	image.draw(_x,_y,w,h);
}

//closes the camera object as defined in the viewworksHS files.
void cameraImage::close(){
	camera.close();
}

//call to display live feed
void cameraImage::liveFeed(){
	bLive=true;
}

//call to deactivate the live image feed
void cameraImage::playback(){
	bLive=false;
}

//begin playback of the stored images
void cameraImage::play(){
	bPlay = true;
	bLive = false;
}

//pause image playback
void cameraImage::pause(){
	bPlay = false;
}

//return the playback location to the beginning of the stored images.
void cameraImage::reset(){
	buffers.gotoPercent(0);
}

//jump to a percentage of playback
void cameraImage::gotoPercent(float perc){
	buffers.gotoPercent(perc);
}

//return the percentage of the stored images which have been played.
float cameraImage::getPercentage(){
	return buffers.getPercentDone();
}

//determine which video source to use, either the live stream or the current frame from the saved images
PBYTE cameraImage::stream(){
	PBYTE temp = liveBuffer;
	if(!bLive) temp = buffers.currentFrame();

	//swap the pixel order before displaying
	if(bAvailable&&temp) convertBGRtoRGB(colorCorrect,temp,camera.width,camera.height);
	return colorCorrect;
}

//instructs the computer to begin storing frames.
void cameraImage::startRecording(){
	startRecord = ofGetElapsedTimeMillis();
	bRecord = true;
	buffers.resetStore();
	cout << "Started recording at " << ofGetElapsedTimef() << endl;
}

//stop storing frames.
void cameraImage::stopRecording(){
	bRecord = false;
}

//percentage of total frames which have been recorded.
float cameraImage::recordingPercent(){
	float ret =0;
	if(bRecord){
		ret=(float(ofGetElapsedTimeMillis())-float(startRecord))/(cfg().recordTime*1000.);
	}
	return ret;
}

/* //////////////////////////////////////////////////////////

if there is a problem in the program, it probably happens here:
this function is called by the camera callback function, and is where incoming data
is processed.

//////////////////////////////////////////////////////////////*/
void cameraImage::store(IMAGE_INFO* pImageInfo){
	//if pointer is NULL, exit
	if(!pImageInfo) return;

	//if the livebuffer isn't null and the image pointer is valid,
	if(liveBuffer&&pImageInfo->pImage){
		//unpack the width and height of the image, as well as the pixel format
		UINT unWidth		= pImageInfo->width;
		UINT unHeight		= pImageInfo->height;
		PIXEL_FORMAT ePixelFormat = pImageInfo->pixelFormat;

		//CameraGetWidth( camera.camera, &unWidth );
		//CameraGetHeight( camera.camera, &unHeight );
		//CameraGetPixelFormat( camera.camera, &ePixelFormat);

		//Attempt to convert the image to BGR packed bmp
		if(!ConvertPixelFormat( ePixelFormat,liveBuffer, (PBYTE)pImageInfo->pImage, unWidth, unHeight )){
			cout << "error converting image" << endl;
		}
		else bAvailable=true;

		//if we are recording
		if(bRecord){
			//Store the image into the next available buffer; if doing so fills the buffers, stop recording
			//also, if the record time is exceeded, stop recording.
			bRecord = buffers.store(liveBuffer)&&(startRecord+cfg().recordTime*1000>ofGetElapsedTimeMillis());
			//if we just stopped recording, write out some information
			if(!bRecord){
				cout << "Stopped recording at " << ofGetElapsedTimef() << endl;
				cout << "Last frame was " << buffers.storageNumber() << " of " << buffers.maxFrames() << endl;
				
				//stop the livefeed
				bLive=false;

				//reset and pause the playback
				reset();
				pause();

				//if the config file instructs us to autoplay the movie
				if(cfg().autoPlay){
					//set up the timer, and enable the autoplay lock
					autoPlayTimer=cfg().autoPlayDelay*1000+ofGetElapsedTimeMillis();
					autoPlayLock=true;
					cout << "autoPlay"<< endl;
				}
			}
		}
	}
}