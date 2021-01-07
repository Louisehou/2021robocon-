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

bool colorred(Mat a)        //查找图像内的有无红色像素点,有就返回1，无就返回0
{
	IplImage *image = &IplImage(a);
	CvScalar scalar1;
	vector<int> sca;
	bool flag = false;
	int count = 0;
	int r = 0, g = 0, b = 0;
	//CvScalara scalar1;
	for (int i = 0;i <= image->height - 1;++i)
	{
		for (int j = 0;j <= image->width - 1;++j)
		{
			scalar1 = cvGet2D(image, i, j);
			/*r = scalar1.val[2];
			g = scalar1.val[1];
			b = scalar1.val[0];
			if (r > 150 && g > 120 && b > 100)
			{
			count++;
			}*/
			if (scalar1.val[2] > 240 && scalar1.val[1] > 110 && scalar1.val[0] > 100)
			{
				/*int a = (i*image->width) + j;
				sca.push_back(a);*/
				count++;
				//flag = true;
			}
		}
	}
	if (count>70)
		flag = true;
	return flag;
}

void colorred2(Mat a)           //筛选轮廓中的红色
{
	IplImage *image = &IplImage(a);
	CvScalar scalar2;
	Mat img_color = Mat::zeros(a.size(), CV_8SC3);
	Mat labeles, stats, centroids;
	int nccomps = connectedComponentsWithStats(a, labeles, stats, centroids);
	for (int i = 0;i < nccomps;i++)
	{

	}
	vector<Vec3b>colors(nccomps + 1);
	colors[0] = Vec3b(0, 0, 0);

}

vector<vector<Point>> NEW_contours(vector<vector<Point>> contours)   //把一些比较小的轮廓删掉
{
	for (int i = 0;i < contours.size();++i)
	{
		if (contours[i].size() > 30)
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
	Mat previousframe, currentframe, difframe, difframe2, difframe3,s3;
	Mat adframe_bw, mid;
	int numFrames = capture.get(CV_CAP_PROP_FPS);
	int num = numFrames;
	while (1)
	{
		capture >> frame;
		if (frame.empty())break;
		previousframe = frame.clone();   //第一帧

		capture >> frame;
		currentframe = frame.clone();   //第二帧

		capture >> frame;
		s3 = frame.clone();            //第三帧

		cvtColor(previousframe, previousframe, CV_RGB2GRAY);
		cvtColor(currentframe, currentframe, CV_RGB2GRAY);
		cvtColor(s3, s3, CV_RGB2GRAY);


		absdiff(currentframe, previousframe, difframe);
		absdiff(previousframe, s3, difframe2);
		bitwise_and(difframe, difframe2, difframe3);
		threshold(difframe3, adframe_bw, 60, 255, 0);
		medianBlur(adframe_bw, mid, 5);
		Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
		erode(mid, mid, element);
		//cvtColor(difframe3, gray, CV_RGB2GRAY);
		vector<vector<Point>> contours;
		vector<vector<Point>> contours_red;
		vector<Vec4i> hierarchy;
		findContours(difframe3, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_NONE);   //参数选择
		contours_red = NEW_contours(contours);
		vector<Rect> boundRect(contours_red.size());
		Rect r;
		if (contours.size() >= 1)
		{

			//if (a==true)
			for (int i = 0;i < contours_red.size();i++)
			{
				boundRect[i] = boundingRect(contours_red[i]);
				r = boundingRect(contours_red[i]);
				Mat new_arrow;
				new_arrow = frame(Rect(r));
				bool a = colorred(new_arrow);
				if (a == true)
				{
					rectangle(frame, Point(boundRect[i].x, boundRect[i].y), Point(boundRect[i].x + boundRect[i].width, boundRect[i].y + boundRect[i].height), Scalar(0, 255, 0), 2);
				}
					
			}
			
		}
		
		imshow("ok", frame);
		waitKey(200);
		boundRect.clear();
	}
	return 0;
}