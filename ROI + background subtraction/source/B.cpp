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
//C++
#include <iostream>
#include <sstream>

#define ESC 27
#define ERROR -1

const char FIRST = '1';
const char SECOND = '2';
const char EXIT = 'e';

using namespace cv;
using namespace std;

void into();
const char* CURRENT_FRAME_WINDOW = "Current Frame";
const char* RESULT_WINDOW = "Result";

int main(int, char**)
{
	into();
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
	{
		printf("Could not open camera!! aborting...");
		return ERROR;
	}
	Mat currentFrame, result;
	namedWindow(CURRENT_FRAME_WINDOW, 1);
	namedWindow(RESULT_WINDOW, 2);
	//MOG2 Background subtractor
	Ptr<BackgroundSubtractor> pMOG2 = createBackgroundSubtractorMOG2(); //MOG2 approach
	while (true)
	{
		cap >> currentFrame; // get a new frame from camera
		pMOG2->apply(currentFrame, result);
		imshow(CURRENT_FRAME_WINDOW, currentFrame);
		imshow(RESULT_WINDOW, result);
		if ((cv::waitKey(30)) > 0)
		{
			break;
		}
	}
	cap.release();
	destroyAllWindows();
	return 0;
}


void into()
{
	printf("-----------------------------------------------------------------------------------\n\n");
	printf("-----------------------------------[Tamir Sagi]-------------------------------------\n");
	printf("----------------------------------[HW1 - Part B]-------------------------------------\n\n");
	printf("--------------------------------------------------------------------------------------\n\n");
	printf("The Program shows select ROI (Region of interest) from a simple image\n\n");
	printf("Press Esc To Captrue the Background, then Esc again to terminate the program\n\n");
}
