#pragma once
#include "ofMain.h"
#include "ofExtended.h"
#include "viewWorksHS\viewWorksHS.h"
#include "imgBuffer\imgBuffer.h"

class cameraImage {
protected:
	bool bLive;
	bool bRecord;
	bool bPlay;
	long timer;
	PBYTE colorCorrect;
	OBJECT_INFO objectInfo;
	bool bAvailable;
	long autoPlayTimer;
	bool autoPlayLock;
	long startRecord;
public:
	int x,y,w,h;
	PBYTE liveBuffer;
	ofImage image;
	highSpeedCam camera;
	imgBuffer buffers;

	cameraImage();
	~cameraImage();

	//setting functions
	void init();
	bool update();
	void draw(int x, int y);
	void allocate();
	void close();

	//playback
	void liveFeed();
	bool isLive() { return bLive; }
	void play();
	bool isPlaying() { return bPlay; }
	void pause();
	void playback();
	void reset();
	void gotoPercent(float perc);
	float getPercentage();
	PBYTE stream();

	//record controls
	void startRecording();
	void stopRecording();
	bool isRecording(){ return bRecord; }
	void store(IMAGE_INFO* pImageInfo);

	bool isLiveFeed(){ return bLive; }
};