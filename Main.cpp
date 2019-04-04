/*----------------VTEL Project for videos in dalas highway----
------------------Project information-------------------------
The project aims to 
------------------Environment---------------------------------
--Video infromation:
--resolution: 1920 (w) * 1080 (h)
--fps: 25
--Camera information:
--not available
--Shooting information:
--angle: lens parallel with the local ground
*/

#include "myVideo.h"
#include <iostream>
using namespace std;
int main()
{
	//input file location
	char* file_location = "cars_passing_input.mp4";
	//create a new class to process the video
	myVideo myvideo(file_location);
	//find the wheels and calculate speed of each wheels
	myvideo.Operate();
	//1 if show results frame by frame, 0 ohterwise
	//1 if saveresult, 0 otherwise
	int show = 0;
	int saveresult = 1;
	//open the file to read it and print wheels and speeds to each frame
	myvideo.cap.open(file_location);
	//show and save results
	myvideo.Showresult(show, saveresult);

	return 0;
}
