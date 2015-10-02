#pragma once
#include <iostream>
#include "windows.h"

//#include "ofMain.h"
//#include "ofExtended.h"

// for VwGigE
#include "VwGigE.h"
#include "VwImageProcess.h"
#include "vwgige.Global.h"
#include "VwGigE.API.h"

#include <list>
#include "VwCamera.h"

#include "../imgBuffer/imgBuffer.h"

class VwGigE;
class VwCamera;
struct OBJECT_INFO;
struct IMAGE_INFO;
enum PIXEL_FORMAT;
enum RESULT;



class highSpeedCam {
private:
protected:
	VwGigE* GigE;
	VwCamera* camera;
	BITMAPINFO* BmpInfo1;
	int imageCount;
	double currentFPS;
	UINT imageBufferNumber;
	OBJECT_INFO* objectInfo;
	UINT formatMultiplier;
	PIXEL_FORMAT pixelFormat;
	bool bReady;
public:
	UINT width,height,bufferSize;
	bool newFrame;

	void init();

	// Camera setting functions
	highSpeedCam();
	~highSpeedCam();
	void open();
	void open(void (* func)( OBJECT_INFO* pObjectInfo, IMAGE_INFO* pImageInfo ),OBJECT_INFO* objInfo);
	bool ready();
	void setFPS(int num);
	void setRotation();
	void handleError(RESULT result);
	void close();

	// Image processing functions
	void (* processFunction)( OBJECT_INFO* pObjectInfo, IMAGE_INFO* pImageInfo );
	void setProcessFunction(void (* func)( OBJECT_INFO* pObjectInfo, IMAGE_INFO* pImageInfo ));
	void getImageSize();
	void allocateBuffer();
	void testImage(TESTIMAGE);
	//static void storeImage( OBJECT_INFO* pObjectInfo, IMAGE_INFO* pImageInfo );

	// Stream controls
	void snap(int);
	void grab();
	void stopGrab();
	bool isCapturing();
	void setCaptureMode();
	int getExposureTime();
	void setPixelFormat(PIXEL_FORMAT);

	friend class cameraImage;
};

BOOL ConvertPixelFormat( PIXEL_FORMAT ePixelFormat, BYTE* pDest, BYTE* pSource, int nWidth, int nHeight );

void convertBGRtoRGB(PBYTE pDest, BYTE* pSource, UINT nWidth, UINT nHeight);