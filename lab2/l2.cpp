#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>

// g++ `pkg-config --cflags opencv4` cv.cpp `pkg-config --libs opencv4`

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{

    int width;
	int height;

	if(argc == 3){
		width = atoi(argv[1]);
		height = atoi(argv[2]);
	} else{
		width = 320;
		height = 200;
	}
	cout << width << " x " << height << endl;

	VideoCapture cap(0);

	if (!cap.isOpened())
		return -1;

	namedWindow("hsv", WINDOW_AUTOSIZE);
    int H_low = 0, H_high = 255;
	int S_low = 0 ,S_high = 255;
	int V_low = 0, V_high = 255;
	createTrackbar("H_low", "hsv", &H_low, 255);
    createTrackbar("S_low", "hsv", &S_low, 255);
    createTrackbar("V_low", "hsv", &V_low, 255);
    createTrackbar("H_high", "hsv", &H_high, 255);
    createTrackbar("S_high", "hsv", &S_high, 255);
    createTrackbar("V_high", "hsv", &V_high, 255);

	while (true)
	{
		Mat origin, blurred, hsv; //frames
		cap >> origin;

		resize(origin,origin,{width,height});

        GaussianBlur(origin, blurred, Size(5, 5), 0, 0);
        
        imshow("origin", origin);
		imshow("blurred", blurred);
        
 		cvtColor(blurred, hsv,COLOR_BGR2HSV);
        inRange(hsv, Scalar(H_low, S_low, V_low), Scalar(H_high, S_high, V_high), hsv);
         
        imshow("hsv", hsv);
		
		char k = waitKey(1);
		if (k == 27)
			break;
		if (k == 'x')
		{
			auto r = selectROI("blurred", blurred); // Rect r
            Mat roi = blurred(r);
            imwrite("/home/vk/Документы/PJATK_sm5/NAI/lab2/roi.png", roi);
		}
	}
	return 0;
}


