#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;

#define ERROR -1

/*
The Program shows select ROI (Region of interest) from a simple image.
*/


//Callback for mousclick event, the x-y coordinate of mouse button-up and button-down 
//are stored in two points pt1, pt2.
void mouse_click(int event, int x, int y, int flags, void *param);

//print intro message to console
void into();

//Check that the rectangle is inside the window frame
void checkBoundary();

//Show the cropped image
void ShowROI();

bool clicked = false, dragged = false;
Point pt1, pt2;
Mat original, cropped_IMG, backup;
Rect croppedArea(0, 0, 0, 0);
const char* ORIGINAL_WINDOW = "Original";
const char* CROPPED_IMG_WINDOW = "Cropped Area";
int main()
{
	into();
	original = imread("scream.jpg");
	if (original.empty()){
		return ERROR;
	}
	namedWindow(ORIGINAL_WINDOW, 1);
	cvSetMouseCallback(ORIGINAL_WINDOW, mouse_click, 0);
	imshow(ORIGINAL_WINDOW, original);
	while (cvWaitKey() < 0);
	printf("closing all!!\n");
	cvDestroyWindow(ORIGINAL_WINDOW);
	cvDestroyWindow(CROPPED_IMG_WINDOW);
	original.release();
	return 0;
}


void into()
{
	printf("-----------------------------------------------------------------------------------\n\n");
	printf("-----------------------------------[Tamir Sagi]-------------------------------------\n");
	printf("----------------------------------[HW1 - Part A]-------------------------------------\n\n");
	printf("--------------------------------------------------------------------------------------\n\n");
	printf("The Program shows select ROI (Region of interest) from a simple image\n\n");
	printf("Press Esc To Captrue the Background, then Esc again to terminate the program\n\n");
}


void mouse_click(int event, int x, int y, int flags, void *param)
{
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
		imshow(ORIGINAL_WINDOW, original);
		croppedArea = Rect(0, 0, 0, 0);
		cvDestroyWindow(CROPPED_IMG_WINDOW);
		clicked = true;
		printf("Mouse Left Button Pressed!!\n");
		pt1.x = x;
		pt1.y = y;
		break;
	case CV_EVENT_LBUTTONUP:
		printf("Mouse Left Button Released!!\n");
		clicked = false;
		if (dragged)
		{
			dragged = false;
			pt2.x = x;
			pt2.y = y;
		}
		break;
	case CV_EVENT_MOUSEMOVE:
		printf("Mouse was draged!!\n");
		if (clicked)
		{
			dragged = true;
			pt2.x = x;
			pt2.y = y;
		}
		break;
	}
	if (clicked && dragged){
		if (pt1.x > pt2.x){
			croppedArea.x = pt2.x;
			croppedArea.width = pt1.x - pt2.x;
		}
		else {
			croppedArea.x = pt1.x;
			croppedArea.width = pt2.x - pt1.x;
		}

		if (pt1.y > pt2.y){
			croppedArea.y = pt2.y;
			croppedArea.height = pt1.y - pt2.y;
		}
		else {
			croppedArea.y = pt1.y;
			croppedArea.height = pt2.y - pt1.y;
		}
	}
	if (dragged)
		ShowROI();
}

void checkBoundary()
{
	//check croping rectangle exceed image boundary
	int width = backup.cols - croppedArea.x, height = backup.rows - croppedArea.y;
	if (croppedArea.width > width)
		croppedArea.width = width;

	if (croppedArea.height > height)
		croppedArea.height = height;

	if (croppedArea.x < 0)
		croppedArea.x = 0;

	if (croppedArea.y < 0)
		croppedArea.height = 0;
}

void ShowROI()
{
	printf("Showing ROI!\n");
	backup = original.clone();
	checkBoundary();
	if (croppedArea.width > 0 && croppedArea.height > 0){
		cropped_IMG = original(croppedArea);
		imshow("cropped", cropped_IMG);
	}

	IplImage tmp = backup;
	cvRectangle(&tmp, pt1, pt2, Scalar(0, 255, 0));
	cvShowImage(ORIGINAL_WINDOW, &tmp);
}

