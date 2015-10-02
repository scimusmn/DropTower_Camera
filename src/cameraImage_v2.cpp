#include "cameraImage.h"
#include "config.h"

cameraImage::cameraImage(){
	bLive = true;
	bRecord = false;
	bPlay=false;
	liveBuffer = 0;
	colorCorrect = 0;
}

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

void captureImage( OBJECT_INFO* pObjectInfo, IMAGE_INFO* pImageInfo ){
	cameraImage* img = (cameraImage*)pObjectInfo->pUserPointer;
	if(!img) return;
	img->store(pImageInfo);
}

void cameraImage::init(){
	camera.init();
	objectInfo.pUserPointer=this;
	camera.open(&captureImage,&objectInfo);

	camera.getImageSize();

	if(camera.bufferSize){
		liveBuffer = new BYTE[camera.bufferSize];
		colorCorrect = new BYTE[camera.bufferSize];
		buffers.allocate(250,camera.bufferSize);
	}

	image.allocate(camera.width, camera.height, OF_IMAGE_COLOR);
	timer = ofGetElapsedTimeMillis()+50;
	autoPlayTimer = 0;
	autoPlayLock = false;

	camera.testImage(TESTIMAGE_OFF);
	camera.grab();
}

bool cameraImage::update(){
	bool ret=false;
	if(ofGetElapsedTimeMillis()>timer&&camera.ready()){
		if(!camera.isCapturing()) camera.close();
		timer=ofGetElapsedTimeMillis()+((bLive)?20:50);
		PBYTE temp = stream();
		if(temp) image.setFromPixels(temp,camera.width,camera.height,OF_IMAGE_COLOR,true);
		if(!bLive&&bPlay){
			bPlay=buffers.nextFrame();
			if(cfg().loopVideo&&!bPlay) bPlay=true, reset();
		}
		if(!bLive) ret=true;
	}
	if(cfg().autoPlay&&autoPlayLock&&ofGetElapsedTimeMillis()>autoPlayTimer){
		play();
		autoPlayLock=false;
	}
	if(!camera.ready()) camera.open(&captureImage,&objectInfo),camera.grab();
	return ret;
}

void cameraImage::draw(int _x, int _y){
	x=_x,y=_y,w=image.width*1.25,h=image.height*1.25;
	ofSetColor(255,255,255);
	image.draw(_x,_y,w,h);
}

void cameraImage::draw(int _x, int _y, int _w, int _h){
	x=_x,y=_y,w=_w,h=_h;
	ofSetColor(255,255,255);
	image.draw(_x,_y,w,h);
}

void cameraImage::close(){
	camera.close();
}

void cameraImage::liveFeed(){
	bLive=true;
}

void cameraImage::playback(){
	bLive=false;
}

void cameraImage::play(){
	bPlay = true;
	bLive = false;
}

void cameraImage::pause(){
	bPlay = false;
}

void cameraImage::reset(){
	buffers.gotoPercent(0);
}

void cameraImage::gotoPercent(float perc){
	buffers.gotoPercent(perc);
}

float cameraImage::getPercentage(){
	return buffers.getPercentDone();
}

PBYTE cameraImage::stream(){
	PBYTE temp = liveBuffer;
	if(!bLive) temp = buffers.currentFrame();
	if(bAvailable&&temp) convertBGRtoRGB(colorCorrect,temp,camera.width,camera.height);
	return colorCorrect;
}

void cameraImage::startRecording(){
	startRecord = ofGetElapsedTimeMillis();
	bRecord = true;
	buffers.resetStore();
	cout << "Started recording at " << ofGetElapsedTimef() << endl;
}

void cameraImage::stopRecording(){
	bRecord = false;
}

float cameraImage::recordingPercent(){
	float ret =0;
	if(bRecord){
		ret=(float(ofGetElapsedTimeMillis())-float(startRecord))/(cfg().recordTime*1000.);
	}
	return ret;
}

void* normPImage=0;

void cameraImage::store(IMAGE_INFO* pImageInfo){
	if(!pImageInfo) return;
	if(liveBuffer&&pImageInfo->pImage&&(!normPImage||normPImage==pImageInfo->pImage)){
		if(!normPImage) normPImage=pImageInfo->pImage,cout << normPImage << endl;
		UINT unWidth		= pImageInfo->width;
		UINT unHeight		= pImageInfo->height;
		PIXEL_FORMAT ePixelFormat = pImageInfo->pixelFormat;

		//CameraGetWidth( camera.camera, &unWidth );
		//CameraGetHeight( camera.camera, &unHeight );
		//CameraGetPixelFormat( camera.camera, &ePixelFormat);
		if(!ConvertPixelFormat( ePixelFormat,liveBuffer, (PBYTE)pImageInfo->pImage, unWidth, unHeight )){
			cout << "error converting image" << endl;
		}
		else bAvailable=true;

		if(bRecord){
			bRecord = buffers.store(liveBuffer)&&(startRecord+cfg().recordTime*1000>ofGetElapsedTimeMillis());
			if(!bRecord){
				cout << "Stopped recording at " << ofGetElapsedTimef() << endl;
				cout << "Last frame was " << buffers.storageNumber() << " of " << buffers.maxFrames() << endl;
				cout << ((startRecord+cfg().recordTime*1000)-ofGetElapsedTimeMillis()) << endl;
				cout << startRecord+cfg().recordTime*1000 << " :end time | millis(): " <<ofGetElapsedTimeMillis() << endl;
				bLive=false;
				reset();
				pause();
				if(cfg().autoPlay){
					autoPlayTimer=cfg().autoPlayDelay*1000+ofGetElapsedTimeMillis();
					autoPlayLock=true;
					cout << "autoPlay"<< endl;
				}
			}
		}
	}
}