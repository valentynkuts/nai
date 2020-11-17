#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <cmath>

using namespace std;
using namespace cv;

CascadeClassifier car_cascade;


bool isRectFound(vector<Rect> vec, Rect item){
    
    Point p1 = Point(item.x,item.y);
    Point p2 = Point(item.x+item.width, item.y);
    Point p3 = Point(item.x+item.width, item.y + item.height);
    Point p4 = Point(item.x, item.y + item.height);
    
    bool found = false;
    
    for (auto & elem : vec)
    {
        
        Point e1 = Point(elem.x,elem.y);
        Point e2 = Point(elem.x+elem.width, elem.y);
        Point e3 = Point(elem.x+elem.width, elem.y + elem.height);
        Point e4 = Point(elem.x, elem.y + elem.height);
        
        Point d1,d2,d3,d4;
        d1 = p1 - e1;
        d2 = p2 - e2;
        d3 = p3 - e3;
        d4 = p4 - e4;
        
        int x1 = abs(d1.x);
        int y1 = abs(d1.y);
        
        int x2 = abs(d2.x);
        int y2 = abs(d2.y);
        
        int x3 = abs(d3.x);
        int y3 = abs(d3.y);
        
        int x4 = abs(d4.x);
        int y4 = abs(d4.y);
        
        int diff = 30;
        
        if ((x1 < diff && y1 < diff) && (x2 < diff && y2 < diff) 
            && (x3 < diff && y3 < diff) && (x4 < diff && y4 < diff))
        {
                found = true;
                break;
        }
    }
    
    if(found)
        return true;
    
    return false;
    
}


vector<Rect> find_car(const Mat frame0){
    
        Mat frame = frame0.clone();
        vector<Rect> cars;
        
        cvtColor(frame, frame, COLOR_BGR2GRAY);//to black-white
        equalizeHist(frame,frame);
        
        car_cascade.detectMultiScale(frame,cars, 1.1, 3, 0, Size(50,50)); //41:00
        
        return cars; 
    
}

bool compareRect(Rect &l, Rect &r) {
//     return (l.width * l.height) > (r.width * r.height);
    return l.area() > r.area();
}
 
//--------------------------
int main(void) {
    
	VideoCapture capture;
	Mat frame;
        
	//-- 1. Load the cascade
    
//    if(!car_cascade.load("haarcascade_car.xml")) 
//     {
// 		return -8;
// 	};
   
   if(!car_cascade.load("cars.xml")) 
    {
		return -8;
	};

    capture.open( -1 );
	if (!capture.isOpened()) {
		return -7;
	}

        
    vector<Rect> storageRect;
    int count = 0;
   
	while (capture.read(frame)) {
        
        capture >> frame;
        flip(frame, frame, 1);
        resize(frame,frame,{900,700});
        
		if(frame.empty())
            return -1;
 
        vector<Rect> cars = find_car(frame);
        int size = cars.size();
        
        sort(cars.begin(), cars.end(), compareRect);
        
        for(int i = 0;i < size; i++){
            
            rectangle(frame,{cars.at(i).x, cars.at(i).y},{cars.at(i).x + cars.at(i).width,cars.at(i).y + cars.at(i).height},{0,255,255},2);
            
//             putText(frame,to_string(size), Point2f(200,200), FONT_HERSHEY_PLAIN, 2,  Scalar(0,0,255,255));
    
            
            if(i == 1){
               
                Rect rect = cars.at(i);
                
                if(!isRectFound(storageRect,rect)){
                    
                    storageRect.push_back(rect);
                    count++;
                }
                
            }
 

        }
        
    putText(frame,to_string(count), Point2f(100,100), FONT_HERSHEY_PLAIN, 3,  Scalar(0,0,255,255), 2);

    imshow("obraz", frame);

    if( (waitKey( 1 )&0x0ff) == 27 ) 
        return 0;
	}
	
	return 0;
}
 
