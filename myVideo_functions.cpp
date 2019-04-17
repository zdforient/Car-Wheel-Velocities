#include "myVideo.h"
myVideo::myVideo(const String &filename)
{
	open(filename);
}

myVideo::~myVideo()
{
	cap.release();
}

int myVideo::open(const String &filename) {
	cap.open(filename);
	if (!cap.isOpened()) {
		cout << "Error opening video stream or file" << endl;
		exit(-1);
		return -1;
	}
	return 0;
};

void myVideo::Findcircles() {
	//frame number
	count++;
	Mat gray;
	//convert to gray
	cvtColor(frame, gray, cv::COLOR_RGB2GRAY);
	GaussianBlur(gray, gray, Size(9, 9), 2, 2);
	vector<Vec3f> circles;
	//set detectiong area as wheels only appear in the middle
	Rect detectrect = Rect(0, YPIXEL / 3, XPIXEL, YPIXEL / 3);
	Mat mattodetect = gray(detectrect);
	//find circles
	HoughCircles(mattodetect, circles, CV_HOUGH_GRADIENT , 1, 1, 60, 30, 50, 100);
	vector<Point> points;
	vector<int> radius;
	for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		//if detected circles is wheel
		if (isWheel(center, radius)) {
			Wheels* w = new Wheels(center, radius);
			if (currentwheels.empty())
				currentwheels.push_back(w);
			else {
				int k = 0;
				for (auto j : currentwheels) {
					if (abs(j->P.x - w->P.x) < 50)
						k = 1;
				}
				if (k)
					continue;
				int j = 0;
				while (center.x > currentwheels[j]->P.x && j != currentwheels.size() - 1)
					j++;
				if (center.x > currentwheels[j]->P.x)
					currentwheels.push_back(w);
				else
					currentwheels.insert(currentwheels.begin() + j, w);
			}
			//circle(frame, center + Point(0,YPIXEL/3), radius, Scalar(0, 0, 255), 3, 8, 0);
		}

		// draw the circle outline
			//use vector and insert.
	}
	int j = 0;
	int ii = 0;
	for (int i = 0; i < currentwheels.size(); i++) {
		if (wheels.empty()) {
			ii = currentwheels.size();
			break;
		}
		if (currentwheels[i]->P.x > wheels[j]->position.back().x) {
			wheels[j]->position.push_back(currentwheels[i]->P);
			wheels[j]->radius.push_back(currentwheels[i]->R);
			wheels[j]->framenum.push_back(count);
			j++;
		}
		else
			ii++;
	}
	for (int i = 0; i < ii; i++) {
		addWheel(currentwheels[i]->P, currentwheels[i]->R, count);
	}
	currentwheels.clear();
}

//add new wheel to the list of wheels
void myVideo::addWheel(Point point, int radius, int framenumber) {
	Wheels* newwheel = new Wheels(point, radius);
	newwheel->position.push_back(point);
	newwheel->radius.push_back(radius);
	newwheel->framenum.push_back(framenumber);
	if (wheels.empty()) {
		newwheel->number = 0;
		wheels.push_back(newwheel);
	}
	else {
		newwheel->number = wheels[0]->number + 1;
		wheels.insert(wheels.begin(), newwheel);
	}
}


void myVideo::Showresult(int show, int save) {
	count = 0;
	cap.set(CV_CAP_PROP_POS_FRAMES, 0);	
	if(show){
	namedWindow("video", WINDOW_NORMAL | WINDOW_KEEPRATIO);
	cv::moveWindow("video", 100, 200);
	cv::resizeWindow("video", XPIXEL, YPIXEL);
	cout << "there";
	}
	VideoWriter video("output_vid.mp4", CV_FOURCC('F', 'M', 'P', '4'), 10, Size(XPIXEL, YPIXEL));
	while (1) // looply reading frames from the video file
	{
		count++;
		if (!cap.read(frame)) {
			break;
		}
		if (frame.empty())
			break;
		bool wheelprinted = 0;
		for (int i = wheels.size() - 1; i > -1; i--) {
			if (count < wheels[i]->framenum[0])
				break;
			for (int j = 0; j < wheels[i]->framenum.size(); j++) {
				if (wheels[i]->framenum[j] == count) {
					//draw wheels to the frame
					circle(frame, wheels[i]->position[j] + Point(0, YPIXEL / 3), wheels[i]->radius[j], Scalar(0, 0, 255), 3, 8, 0);
					putText(frame, "Number: " + to_string(wheels[i]->number), wheels[i]->position[j] + Point(0, YPIXEL / 3), 1, 3, Scalar(0, 255, 255), 3);
					putText(frame, "Speed: " + to_string(wheels[i]->spd[j]), wheels[i]->position[j] + Point(0, YPIXEL / 3) + Point(0,50), 1, 3, Scalar(255, 0, 255), 3);
				}
			}
		}
		if (show) {
			imshow("video", frame);
			cv::waitKey(400);
		}
		if(save)
			video.write(frame);
	}
	video.release();
	cap.release();
}

//if the circle is a wheel, average color is greater than (50,50,50), radius between (60,70), center is in the middle of the frame
bool myVideo::isWheel(Point center, int radius) {
	if (center.y > YPIXEL * 3 / 24 && center.y < YPIXEL * 5 / 24) {
		int xl = max(1, center.x - radius / 2);
		int xr = max(2, center.x + radius / 2);
		xl = min(XPIXEL - 2, xl);
		xr = min(XPIXEL - 1, xr);
		Mat roi = frame(cv::Range(center.y - radius / 2 + YPIXEL / 3, center.y + radius / 2 + YPIXEL / 3), cv::Range(xl, xr));
		cv::Mat1b mask(roi.rows, roi.cols);
		cv::Scalar mean = cv::mean(roi, mask);
		if (mean[0] > 50 && mean[1] > 50 && mean[2] > 50 && radius > 60 && radius < 70)
			return 1;
	}
	return 0;
}

//calculate average speed
void myVideo::Calculatespeed() {
	for (auto &i : wheels) {
		float aspd = 0.0;
		for (int j = 1; j < i->position.size(); j++) {
			aspd += (i->position[j].x - i->position[j-1].x);
		}
		aspd = aspd / float(i->position.size() - 1) * float(FRAMERATE);
		for (int j = 0; j < i->position.size(); j++) {
			i->spd.push_back(aspd);
		}
	}
}
