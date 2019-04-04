#pragma once
//#define HAVE_OPENCV_VIDEO
//#define HAVE_OPENCV_VIDEOIO
//#define HAVE_OPENCV_VIDEO
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/core/cuda.hpp>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <string>
#include <map>
#include <cmath>
#include <iomanip>
#include <thread>
#include <chrono>

using namespace cv;
using namespace std;
//constants, with and height, framerate
const int XPIXEL = 1920;
const int YPIXEL = 1080;
const int FRAMERATE = 25;

class myVideo
{
public:
	VideoCapture cap;
	Mat frame;
	int count = 0;
public:
	myVideo(const String &filename);
	~myVideo();
	void Showresult(int, int);
	void Operate();
	void Findcircles();
	int open(const String &filename);

public:
	class Page {
		vector<Point> wheelcenters;
		vector<int> wheelradius;
	};


	class Wheels {
	public:
		vector<Point> position;
		vector<int> radius;
		vector<int> framenum;
		vector<float> spd;
		int number;
		Point P;
		int R;
		Wheels(Point p,int r) {
			number = 0;
			P = p;
			R = r;
		}
	};
	vector<Wheels *> lastwheels;
	vector<Wheels *> wheels;
	vector<Wheels *> currentwheels;
	bool isWheel(Point, int);
	void addWheel(Point point, int radius, int framenumber);
	void Calculatespeed();
};

