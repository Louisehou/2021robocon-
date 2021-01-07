#include<opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include <stdio.h>
#include <string>
#include <iostream>
#include <io.h>
#include <fstream>
#include <vector>

using namespace std;
using namespace cv;

vector<vector<Point>> contours1;

vector<vector<Point>> NEW_contours(vector<vector<Point>> contours)   //把一些比较小的轮廓删掉
{
	for (int i = 0;i < contours.size();++i)
	{
		if (contours[i].size() > 100)
		{
			contours1.push_back(contours[i]);
		}
	}
	return contours1;
}

int main()
{
	VideoCapture capture("F://3.avi");

	Mat frame;
	Mat frame_0;
	Mat background, adframe;
	Mat gray;
	Mat frame_hsv;
	Mat hsv[3];
	Mat mid, adframe_bw;

	int numFrames = capture.get(CV_CAP_PROP_FPS);
	int num = numFrames;
	while (1)
	{
		capture >> frame;
		if (frame.empty())break;

		cvtColor(frame, frame_hsv, CV_RGB2GRAY);
		if (num == numFrames)
		{
			background = frame_hsv.clone();
			frame_0 = background;
		}
		else
		{
			background = frame_0;
		}
		absdiff(frame_hsv, background, adframe);
		threshold(adframe, adframe_bw, 60, 100, 0);
		Mat element = getStructuringElement(MORPH_RECT, Size(10, 10));
		erode(adframe_bw, mid, element);
		Mat imageROI;
		imageROI = mid(Rect(124.5, 13.0849, 465.5, 634.972));
		frame_0 = frame_hsv.clone();
		num--;
		imshow("ok", mid);
		waitKey(200);
	}
	return 0;
}