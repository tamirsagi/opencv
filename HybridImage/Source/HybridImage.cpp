#include <opencv2/opencv.hpp>
//opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
//C
#include <stdio.h>
#include <math.h>
//C++
#include <iostream>
#include <sstream>


using namespace cv;
using namespace std;

#define ERROR -1
/*

HW 2 Hybrid Image
Tamir Sagi 302774773

*/
//Methods declarions
void intro();
void createHybridFirstIsClose();
void createHybridSecondIsClose();
char menu();
void showHybridImageWindow(char aOption);

//varibles 
char* WINDOW_ORIGINAL_IMAGE_1 = "Original First Image";
char* WINDOW_ORIGINAL_IMAGE_2 = "Original Second Image";
char* WINDOW_HYBRID_IMAGE = "Hybrid Image";

char* ORIGINAL_IMAGE_1 = "image1.jpg";
char* ORIGINAL_IMAGE_2 = "image2.jpg";

const static int LEVEL = 8;

const char FIRST_IMAGE_FIRST = '0';
const char SECONDS_IMAGE_FIRST = '1';
const char EXIT = 'e';
Mat firstImage;
Mat secondImage;
Mat result;
Mat temp;
int sliderValue = 0;



//guasian
Mat* firstGuasisan = new Mat[LEVEL];
Mat* secondGuasisan = new Mat[LEVEL];

//laplacian
Mat* firstLaplacian = new Mat[LEVEL - 1];
Mat* secondLaplacian = new Mat[LEVEL - 1];

//zoom params
Mat zoomTemp;
int trackBarZoomIndex;
int lastTrackBarValue = 0;
Mat* zoomArray = new Mat[LEVEL];
int zoomArraySize = 0;
const int MAX_ZOOM = LEVEL / 3;

int main(int, char**) {

	intro();
	char option = menu();

	//load images
	firstImage = imread(ORIGINAL_IMAGE_1); //load original
	if (firstImage.empty()) {
		printf(" error while loading image [%s]", ORIGINAL_IMAGE_1);
		return ERROR;
	}
	secondImage = imread(ORIGINAL_IMAGE_2); // load original
	if (secondImage.empty()) {
		printf(" error while loading image [%s]", ORIGINAL_IMAGE_2);
		return ERROR;
	}
	resize(secondImage, secondImage, firstImage.size());
	printf(" First  Image Dim -> [%d]x[%d]\n", firstImage.cols, firstImage.rows);
	printf(" Second Image Dim -> [%d]x[%d]\n", secondImage.cols, secondImage.rows);

	//copy original to array
	firstImage.copyTo(firstGuasisan[0]);
	secondImage.copyTo(secondGuasisan[0]);
	//prepare all arrays (gaussians and laplacians)
	for (int i = 0; i < LEVEL - 1; i++) {
		pyrDown(firstGuasisan[i], firstGuasisan[i + 1]); //gaussian
		pyrUp(firstGuasisan[i + 1], temp, firstGuasisan[i].size());
		firstLaplacian[i] = firstGuasisan[i] - temp; //laplacian
		pyrDown(secondGuasisan[i], secondGuasisan[i + 1]); //gausian
		pyrUp(secondGuasisan[i + 1], temp, secondGuasisan[i].size());
		secondLaplacian[i] = secondGuasisan[i] - temp; //laplacian 
	}

	//show result with trackbar
	showHybridImageWindow(option);
	//wait until exist
	while (waitKey(30) != EXIT);
	return 0;
}

void intro()
{
	printf("-----------------------------------------------------------------------\n\n");
	printf("--------------------------[Tamir Sagi]----------------------\n");
	printf("----------------------------[HW2 - Hybrid Image]-------------------------\n\n");
	printf("--------------------------------------------------------------------------\n\n");
}

char menu() {
	char option = ' ';
	while (option != FIRST_IMAGE_FIRST && option != SECONDS_IMAGE_FIRST) {
		printf("---------------------------Hybrid Image Menu------------------------\n\n");
		printf("First Image is Close.................................................0\n\n");
		printf("Second Image Is Close................................................1\n\n");
		cin >> option;
	}
	return option;
}

void createHybridFirstIsClose() {
	firstGuasisan[sliderValue].copyTo(result);
	for (int i = sliderValue - 1; i >= 0; i--) {
		pyrUp(result, result, secondLaplacian[i].size());
		result += secondLaplacian[i];
	}
	resize(result, zoomTemp, zoomTemp.size());
	zoomTemp.copyTo(zoomArray[trackBarZoomIndex]);
	for (int i = trackBarZoomIndex - 1; i >= 0; i--) {
		resize(zoomArray[i + 1], zoomArray[i], zoomArray[i].size());
	}
	imshow(WINDOW_HYBRID_IMAGE, zoomTemp);
	resizeWindow(WINDOW_HYBRID_IMAGE, firstImage.cols, firstImage.rows);
}

/*Call back to trackbar*/
void createHybridSecondIsClose() {
	secondGuasisan[sliderValue].copyTo(result);
	for (int i = sliderValue - 1; i >= 0; i--) {
		pyrUp(result, result, firstLaplacian[i].size());
		result += firstLaplacian[i];
	}
	resize(result, zoomTemp, zoomTemp.size());
	zoomTemp.copyTo(zoomArray[trackBarZoomIndex]);
	for (int i = trackBarZoomIndex - 1; i >= 0; i--) {
		resize(zoomArray[i + 1], zoomArray[i], zoomArray[i].size());
	}
	imshow(WINDOW_HYBRID_IMAGE, zoomTemp);
	resizeWindow(WINDOW_HYBRID_IMAGE, firstImage.cols, firstImage.rows);
}

//perform zoom to current image
void zoom() {
	
	if (lastTrackBarValue < trackBarZoomIndex) {
		zoomTemp.copyTo(zoomArray[zoomArraySize++]);
		pyrDown(zoomTemp, zoomTemp);
	}
	else {
		zoomArray[--zoomArraySize].copyTo(zoomTemp);
	}
	lastTrackBarValue = trackBarZoomIndex;
	imshow(WINDOW_HYBRID_IMAGE, zoomTemp);
	resizeWindow(WINDOW_HYBRID_IMAGE, firstImage.cols, firstImage.rows);
}

//show the initial image inside hybrid window
void showHybridImageWindow(char aOption) {
	namedWindow(WINDOW_HYBRID_IMAGE, 3);
	namedWindow(WINDOW_ORIGINAL_IMAGE_1, 1);
	namedWindow(WINDOW_ORIGINAL_IMAGE_2, 2);
	resizeWindow(WINDOW_ORIGINAL_IMAGE_1, firstImage.cols, firstImage.rows);
	resizeWindow(WINDOW_ORIGINAL_IMAGE_2, secondImage.cols, secondImage.rows);
	switch (aOption) {
	case FIRST_IMAGE_FIRST:
		firstImage.copyTo(zoomArray[0]);
		firstImage.copyTo(zoomTemp);
		imshow(WINDOW_ORIGINAL_IMAGE_1, firstImage);
		imshow(WINDOW_ORIGINAL_IMAGE_2, secondImage);
		imshow(WINDOW_HYBRID_IMAGE, zoomArray[0]);
		createTrackbar("Hybrid", WINDOW_HYBRID_IMAGE, &sliderValue, LEVEL - 1, TrackbarCallback(createHybridFirstIsClose));
		break;
	case SECONDS_IMAGE_FIRST:
		secondImage.copyTo(zoomArray[0]);
		secondImage.copyTo(zoomTemp);
		imshow(WINDOW_ORIGINAL_IMAGE_1, secondImage);
		imshow(WINDOW_ORIGINAL_IMAGE_2, firstImage);
		imshow(WINDOW_HYBRID_IMAGE, zoomArray[0]);
		createTrackbar("Hybrid", WINDOW_HYBRID_IMAGE, &sliderValue, LEVEL - 1, TrackbarCallback(createHybridSecondIsClose));
		break;
	default:
		cvDestroyWindow(WINDOW_HYBRID_IMAGE);
		break;
	}
	createTrackbar("Zoom", WINDOW_HYBRID_IMAGE, &trackBarZoomIndex, MAX_ZOOM, TrackbarCallback(zoom));
}
