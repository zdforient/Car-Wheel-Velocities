#include "myVideo.h"
void myVideo::Operate() {
	//read frame from the file;
	while (1) // looply reading frames from the video file
	{
		if (!cap.read(frame)) {
			break;
		}
		//the end of the file
		if (frame.empty())
			break;
		//Find white circles from the frame
		Findcircles();
	}
	//calculate speed of wheels
	Calculatespeed();
	cap.release();
}

