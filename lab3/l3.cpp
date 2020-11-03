 
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

using namespace cv;
using namespace std;

class MyObject
{
public:
	int maxC;
	vector<Point> pos;
    //average
	Point getP()  
	{
		Point sump = std::accumulate(pos.begin(), pos.end(), Point(0, 0));
        
        double pos_size = (double)pos.size();
        sump.x /= pos_size;
		sump.y /= pos_size;
        
		return sump;
	}
	void addP(Point p)
	{
		pos.push_back(p);
		if (pos.size() > maxC)
		{
            //vector withot first element
			pos = vector<Point>(pos.begin() + 1, pos.end());
		}
	}
	void addEmpty()
	{
		if (pos.size() > 0)
		{
			pos = vector<Point>(pos.begin() + 1, pos.end());
		}
	}
};

int main() {
    
    //yellow
    int loRange[3] = {22,93,0};
	int hiRange[3] = {45,255,255};

	VideoCapture camera(0);
    
    MyObject myobj, myobj2;
    myobj.maxC = 10;
    myobj2.maxC = 10;
    
	
	while ( waitKey(1) != 27  ) {  //ESC
		Mat frame;
		Mat frameMask;

        camera >> frame;
		flip(frame, frame, 1);
		
		cvtColor(frame, frameMask, COLOR_BGR2HSV);
		inRange(frameMask, Scalar(loRange[0],loRange[1],loRange[2]),
						Scalar(hiRange[0],hiRange[1],hiRange[2]), frameMask);

        vector<vector<Point>> contours;
		findContours(frameMask, contours, cv::RETR_LIST, cv::CHAIN_APPROX_TC89_KCOS);
        
        sort(contours.begin(), contours.end(),
			 [](auto &a, auto &b) {
				 return contourArea(a, false) > contourArea(b, false);
			 });
        
        
        for(int i = 0;i < contours.size(); i++){
            approxPolyDP(contours.at(i),contours.at(i),10,true);
            
            if(i == 0 || i == 1) 
                drawContours(frame,contours,i,{0,0,255,255},4); //draw  contours 
            
           //putText(frame,to_string(contours.at(i).size()),contours.at(i).at(0),cv::FONT_HERSHEY_PLAIN,2,{0,0,255,255});//show number
            
        }
        
        
        if (contours.size() >= 2)
		{
			Point avg;
			Rect r = boundingRect(contours.at(0)); // środek z prostokąta  //contours.at(0)  - kontur zerowy 
			avg.x = r.x + r.width / 2;//pos + środek
			avg.y = r.y + r.height / 2;
			myobj.addP(avg);

            
            Point avg1;
			Rect r1 = boundingRect(contours.at(1));
			avg1.x = r1.x + r1.width / 2;
			avg1.y = r1.y + r1.height / 2;
			myobj2.addP(avg1);
		}
// 		else
// 		{
// 			myobj.addEmpty();
//          myobj2.addEmpty();
// 		}
		
		if (myobj.pos.size() > 1) {
			putText(frame, "X", myobj.getP(), cv::FONT_HERSHEY_PLAIN, 2, {0, 0, 100, 255});
            
		}
		
		if (myobj2.pos.size() > 1) {
			putText(frame, "X", myobj2.getP(), cv::FONT_HERSHEY_PLAIN, 2, {0, 0, 100, 255});
            
 		}
		
        double d = abs(myobj.getP().y - myobj2.getP().y);
        
        if(d < 100){
             Scalar colorLine(0,255,0); // Green
             //int thicknessLine = 2;
             line(frame, myobj.getP(), myobj2.getP(), colorLine, 2);
        }  
        
       /* if(myobj.getP().y == myobj2.getP().y  ){
             Scalar colorLine(0,255,0); // Green
             int thicknessLine = 2;
             line(frame, myobj.getP(), myobj2.getP(), colorLine, 2);
        }     */        
		
		imshow("contours", frame);
        

       
	}
	return 0; 
}


 
 
 
 
