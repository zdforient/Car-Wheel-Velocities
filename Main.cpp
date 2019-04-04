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
	char* file_location = "cars_passing_input.mp4";
	myVideo myvideo(file_location);
	//find the wheels
	myvideo.Operate();
	//1 if save the result to create a video, 0 otherwise
	int show = 0;
	int saveresult = 1;
	myvideo.cap.open(file_location);

	myvideo.Showresult(show, saveresult);

	return 0;
}
