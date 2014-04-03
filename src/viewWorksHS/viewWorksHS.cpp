#include "viewWorksHS.h"


//#include "VwResourceType.h"

highSpeedCam::highSpeedCam(){
	camera=NULL;
	GigE=NULL;
	currentFPS=0;
	imageCount=0;
	processFunction=NULL;
	imageBufferNumber=1;
	newFrame = false;
	formatMultiplier=0;
	bufferSize=0;

	BmpInfo1 = (BITMAPINFO*)new BYTE[(sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD))];
	ZeroMemory(BmpInfo1, sizeof(BITMAPINFOHEADER));

	BmpInfo1->bmiHeader.biSize		    = sizeof( BITMAPINFOHEADER );
	BmpInfo1->bmiHeader.biPlanes		    = 1;	BmpInfo1->bmiHeader.biCompression	= BI_RGB;
	BmpInfo1->bmiHeader.biClrImportant   = 0;
	BmpInfo1->bmiHeader.biBitCount	    = 8;
	BmpInfo1->bmiHeader.biClrUsed		= 256;

	for( UINT i = 0; i < BmpInfo1->bmiHeader.biClrUsed; i++ )
	{
		BmpInfo1->bmiColors[i].rgbBlue     = i;
		BmpInfo1->bmiColors[i].rgbGreen    = i;
		BmpInfo1->bmiColors[i].rgbRed      = i;
		BmpInfo1->bmiColors[i].rgbReserved = 0;
	}

	objectInfo = new OBJECT_INFO;
}

highSpeedCam::~highSpeedCam(){

	close();

	if (BmpInfo1)
	{
		delete BmpInfo1;
		BmpInfo1 = NULL;
	}

	if (objectInfo)
	{
		delete objectInfo;
		objectInfo = NULL;
	}

	if( camera ){
		camera->CloseCamera();
		//delete camera;
		camera = NULL;
	}

	if ( NULL != GigE )
	{
		GigE->Close();
		GigE = NULL;
	}

}

void highSpeedCam::init(){
	GigE	=	 new VwGigE;
	if ( NULL == GigE )
	{
		cout << "Error creating GigE connection" << endl;
		abort();
	}

	if ( RESULT_SUCCESS != GigE->Open() )
	{
		cout << "Error opening GigE connection" << endl;
		abort();
	}
}

void storeImage( OBJECT_INFO* pObjectInfo, IMAGE_INFO* pImageInfo ){
	highSpeedCam* cam = (highSpeedCam*)pObjectInfo->pUserPointer;

	/*UINT unWidth		= pImageInfo->width;
	UINT unHeight		= pImageInfo->height;
	PIXEL_FORMAT ePixelFormat = pImageInfo->pixelFormat;
	void* vpBuffer		= pImageInfo->pImage;

	if(!ConvertPixelFormat( ePixelFormat, cam->unpackedImage, (PBYTE)pImageInfo->pImage, unWidth, unHeight )){
		cout << "error converting image" << endl;
	}*/
}

void highSpeedCam::open(){
	objectInfo->pUserPointer = this;
	open(storeImage,objectInfo);
}

void highSpeedCam::open(void (* func)( OBJECT_INFO* pObjectInfo, IMAGE_INFO* pImageInfo ),OBJECT_INFO* objInfo){
	objInfo->pVwCamera = camera;

	RESULT result = RESULT_ERROR;
	result = GigE->OpenCamera((UINT)0, &camera, imageBufferNumber, 0, 0, 0, objInfo, func, NULL);//(*processFunction)
	if(result != RESULT_SUCCESS){
		cout << "Error " << result << " opening the camera" << endl;
	}

	if ( NULL == camera )
	{
		return;
	}

	//camera->SetPixelFormat(PIXEL_FORMAT_BAYRG8);
	/*char szVendorName[ 256 ];
	size_t cbVendor = sizeof( szVendorName );
	camera->GetDeviceVendorName(0,szVendorName, &cbVendor);
	cout << szVendorName  << endl;*/
	
	getImageSize();
	allocateBuffer();
}

void highSpeedCam::close(){

	if(isCapturing()&&camera){
		//camera->AcquisitionStop();
		//camera->Abort();
	}
}

void highSpeedCam::getImageSize(){
	if ( NULL == camera )
	{
		// ERROR
		return;
	}

	pixelFormat = PIXEL_FORMAT_MONO8;

	camera->GetWidth( &width );
	camera->GetHeight( &height );
	camera->GetPixelFormat( &pixelFormat );

	switch( pixelFormat )
	{
		case PIXEL_FORMAT_MONO10_PACKED:
		case PIXEL_FORMAT_MONO12_PACKED:
		case PIXEL_FORMAT_BAYGR10_PACKED:
			formatMultiplier=6;
			break;
		case PIXEL_FORMAT_BAYGR8:
		case PIXEL_FORMAT_BAYRG8:
		case PIXEL_FORMAT_MONO16:
			formatMultiplier=3;
			break;
		default:
			formatMultiplier=3;
			break;
	}

	bufferSize = width*height*formatMultiplier;
}

void highSpeedCam::allocateBuffer(){
	if ( RESULT_SUCCESS != camera->ChangeBufferFormat( imageBufferNumber, width, height, pixelFormat ) )
	{
		cout << "Can't change buffer format" << endl;
		return;
	}
}

void highSpeedCam::testImage(TESTIMAGE test){
	if(!camera) return;
	camera->SetTestImage(test);
}

void highSpeedCam::setFPS(int num){

}

void highSpeedCam::setRotation(){

}

void highSpeedCam::handleError(RESULT result){

}

// Image processing functions
void highSpeedCam::setProcessFunction(void (* func)( OBJECT_INFO* pObjectInfo, IMAGE_INFO* pImageInfo )){
	processFunction = func;
}

// Stream controls
void highSpeedCam::setCaptureMode(){

}

void highSpeedCam::setPixelFormat(PIXEL_FORMAT){

}

void highSpeedCam::snap(int num=1){
	if(!camera) return;
	RESULT result = camera->Snap( num );
	if(result == RESULT_SUCCESS){
		cout << "Snapping new frame"<<endl;
	}
	else{
		cout << "Error "<< result << endl;
	}
}

void highSpeedCam::grab(){
	if(!camera) return;
	if(isCapturing()){
		//camera->AcquisitionStop();
		//camera->Abort();
	}

	/*camera->SetTriggerMode(false);
	camera->SetGain(GAIN_ANALOG_ALL,3);
	camera->SetTriggerMode(true);
	camera->SetTriggerSource(TRIGGER_SOURCE_SW);

	camera->SetExposureTime(8000);
	camera->SetExposureMode(EXPOSURE_MODE_TRIGGERWIDTH);*/

	RESULT result = camera->Grab();
	if(result == RESULT_SUCCESS){
		cout << "Grabbing Frames"<<endl;

	}
	else{
		cout << "Error "<< result << endl;
	}
}

bool highSpeedCam::isCapturing(){
	BOOL ret=false;
	if(camera)
		camera->GetGrabCondition(ret);
	return ret;
}

int highSpeedCam::getExposureTime(){
	UINT ret=0;
	camera->GetExposureTime(&ret);
	return ret;
}

BOOL ConvertPixelFormat( PIXEL_FORMAT ePixelFormat, BYTE* pDest, BYTE* pSource, int nWidth, int nHeight )
{
	if ( NULL == pDest ||
		 NULL == pSource )
	{
		return FALSE;
	}
	
	if ( 0 == nWidth || 0 == nHeight )
	{
		return FALSE;
	}

	BOOL bRet = TRUE;
	BYTE* bpConvertPixelFormat =	new BYTE[ nWidth * nHeight * 2 ];

	switch( ePixelFormat )
	{
		//-----------------------------------------------------------------
		// about MONO Pixel Format Series ---------------------------------
		//-----------------------------------------------------------------
	case PIXEL_FORMAT_MONO8:
		memcpy( pDest, pSource, nWidth * nHeight );
		break;
	case PIXEL_FORMAT_MONO10:
		ConvertMono10ToBGR8(PBYTE(pSource), nWidth*nHeight*2, pDest);
		break;
	case PIXEL_FORMAT_MONO12:
		ConvertMono12ToBGR8(PBYTE(pSource), nWidth*nHeight*2, pDest);
		break;
	case PIXEL_FORMAT_MONO10_PACKED:
	case PIXEL_FORMAT_MONO12_PACKED:
		ConvertMonoPackedToBGR8( pSource,
												UINT(1.5*nWidth*nHeight),
												pDest );
		break;
	case PIXEL_FORMAT_MONO16:
		ConvertMono16PackedToBGR8( pSource,
												UINT(2*nWidth*nHeight),
												pDest );
		break;
		//-----------------------------------------------------------------
		// about BAYER Pixel Format Series --------------------------------
		//-----------------------------------------------------------------
	case PIXEL_FORMAT_BAYGR8:
		ConvertBAYGR8ToBGR8( pSource,
											pDest,
											nWidth,
											nHeight );
		break;
	case PIXEL_FORMAT_BAYRG8:
		ConvertBAYRG8ToBGR8( pSource,
											pDest,
											nWidth,
											nHeight );
		break;
	case PIXEL_FORMAT_BAYGR10:
		ConvertBAYGR10ToBGR8( (WORD*)(pSource),
											pDest,
											nWidth,
											nHeight );
		break;
	case PIXEL_FORMAT_BAYRG10:
		ConvertBAYRG10ToBGR8( (WORD*)(pSource),
											pDest,
											nWidth,
											nHeight );
		break;
	case PIXEL_FORMAT_BAYGR12:
		ConvertBAYGR12ToBGR8( (WORD*)(pSource),
											pDest,
											nWidth,
											nHeight );
		break;
	case PIXEL_FORMAT_BAYRG12:
		ConvertBAYRG12ToBGR8( (WORD*)(pSource),
											pDest,
											nWidth,
											nHeight );
		break;
	case PIXEL_FORMAT_BAYGR10_PACKED:
		ConvertMono10PackedToMono16bit( (PBYTE)pSource,
														nWidth,
														nHeight,
														bpConvertPixelFormat );
		ConvertBAYGR10ToBGR8( (WORD*)bpConvertPixelFormat,
												pDest,
												nWidth,
												nHeight );
		break;
	case PIXEL_FORMAT_BAYRG10_PACKED:
		ConvertMono10PackedToMono16bit( (PBYTE)pSource,
														nWidth,
														nHeight,
														bpConvertPixelFormat );
		ConvertBAYRG10ToBGR8( (WORD*)bpConvertPixelFormat,
												pDest,
												nWidth,
												nHeight );
		break;
	case PIXEL_FORMAT_BAYGR12_PACKED:
		ConvertMono12PackedToMono16bit( (PBYTE)pSource,
														nWidth,
														nHeight,
														bpConvertPixelFormat );
		ConvertBAYGR12ToBGR8( (WORD*)bpConvertPixelFormat,
												pDest,
												nWidth,
												nHeight );
		break;
	case PIXEL_FORMAT_BAYRG12_PACKED:
		ConvertMono12PackedToMono16bit( (PBYTE)pSource,
														nWidth,
														nHeight,
														bpConvertPixelFormat );
		ConvertBAYRG12ToBGR8( (WORD*)bpConvertPixelFormat,
													pDest,
													nWidth,
													nHeight );
		break;
	case PIXEL_FORMAT_RGB8_PACKED:
		ConvertRGB8toBGR8( (PBYTE)pSource,
											UINT(3*nWidth*nHeight),
											pDest );
		break;
	case PIXEL_FORMAT_BGR8_PACKED:
		bRet = FALSE;
		break;
	case PIXEL_FORMAT_RGB12_PACKED:
		ConvertRGB12PackedtoBGR8( (PBYTE)pSource,
												UINT(6*nWidth*nHeight),
												pDest );
		break;
	case PIXEL_FORMAT_BGR12_PACKED:
		bRet = FALSE;
		break;
	case PIXEL_FORMAT_YUV411:
		ConvertYUV411toBGR8( (PBYTE)pSource,
											UINT(1.5*nWidth*nHeight),
											pDest );
		break;
	case PIXEL_FORMAT_YUV422_UYVY:
		ConvertYUV422_UYVYtoBGR8( (PBYTE)pSource,
												nWidth,
												nHeight,
												pDest );
		break;
	case PIXEL_FORMAT_YUV422_YUYV:
		ConvertYUV422_YUYVtoBGR8( (PBYTE)pSource,
												nWidth,
												nHeight,
												pDest );
		break;
	case PIXEL_FORMAT_YUV444:
		ConvertYUV444toBGR8( (PBYTE)pSource,
											UINT(1.5*nWidth*nHeight),
											pDest );
		break;
	case PIXEL_FORMAT_BGR10V1_PACKED:
		bRet = FALSE;
		break;
	case PIXEL_FORMAT_YUV411_10_PACKED:
	case PIXEL_FORMAT_YUV411_12_PACKED:
		ConvertYUV411PackedtoBGR8( (PBYTE)pSource,
													UINT(2.25*nWidth*nHeight),
													pDest );
		break;
	case PIXEL_FORMAT_YUV422_10_PACKED:
	case PIXEL_FORMAT_YUV422_12_PACKED:
		ConvertYUV422PackedtoBGR8( (PBYTE)pSource,
													UINT(3*nWidth*nHeight),
													pDest );
		break;
	case PIXEL_FORMAT_PAL_INTERLACED:
	case PIXEL_FORMAT_NTSC_INTERLACED:
		break;
	default:
		{
			bRet = FALSE;
		}
	}

	if ( NULL != bpConvertPixelFormat )
	{
		delete [] bpConvertPixelFormat;
	}
	
	return bRet;
}

void convertBGRtoRGB(PBYTE pDest,BYTE* pSource, UINT nWidth, UINT nHeight){
	for(unsigned int i=0; i<nWidth*nHeight*3; i+=3){
		pDest[i+2] = pSource[i];
		pDest[i+1] = pSource[i+1];
		pDest[i] = pSource[i+2];
	}
}