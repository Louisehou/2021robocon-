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
	VideoCapture capture("F://4.avi");
	
	Mat frame;
	Mat frame_0;
	Mat background, adframe;
	Mat gray;
	Mat frame_hsv;
	Mat hsv[3];
	Mat mid, adframe_bw;
	Mat b, c, bf, cf, n;
	
	int numFrames = capture.get(CV_CAP_PROP_FPS);
	int num = numFrames;
	while (1)
	{
		capture >> frame;
		/*if (frame.empty())break;*/
		c = frame.clone();
		capture >> frame;
		b = frame.clone();
		cvtColor(b, bf, CV_RGB2GRAY);
		cvtColor(c, cf, CV_RGB2GRAY);
		/*if (num == numFrames)
		{
			background = frame_hsv.clone();
			frame_0 = background;
		}
		else
		{
			background = frame_0;
		}*/
		absdiff(bf, cf, adframe);
		threshold(adframe, adframe_bw, 65, 255, 0);
		medianBlur(adframe_bw, mid, 5);
		Mat element = getStructuringElement(MORPH_RECT, Size(4, 4));
		erode(mid, mid, element);
		/*Mat ww;
		frame.copyTo(ww, mid);*/
		//threshold(red, red, 150, 255, THRESH_OTSU);
		//morphologyEx(red, red, MORPH_OPEN, element);
		vector<vector<Point>> contours;
		vector<vector<Point>> contours_red;
		vector<Vec4i> hierarchy;
		findContours(mid, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_NONE);   //参数选择
		contours_red = NEW_contours(contours);
		vector<Rect> boundRect(contours_red.size());
		for (int i = 0;i < contours_red.size();i++)
		{
			boundRect[i] = boundingRect(contours_red[i]);
		    rectangle(frame, Point(boundRect[i].x, boundRect[i].y), Point(boundRect[i].x + boundRect[i].width, boundRect[i].y + boundRect[i].height), Scalar(0, 255, 0), 2);
		}
		imshow("ok", frame);
		waitKey(200);
		
	}
	
	return 0;
}