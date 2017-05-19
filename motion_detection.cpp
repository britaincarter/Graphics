#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include "opencv2/opencv.hpp"
#define SIZE 255
#define CONT_SIZE 8 

using namespace cv;
using namespace std;


int main (int argc, char * const argv[]){

	//enum RGB{RED, GREEN, BLUE, WHITE} Color;


	//Assume black
	int r = 0;
	int g = 0;
	int b = 0;	
	if(argc!=2){
		cout <<"usage: <color>\n";
		return -1;

	}
	/*
	string choice(argv[1]);
	switch(choice){
		case "RED" : r = 255;
		case "GREEN" : g = 255;
		case "BLUE" : b = 255;
		case "WHITE": r = 255; g=255; b=255;
	}*/
	else{
		string choice(argv[1]);

		if(choice == "RED"){
			r = 255;
		}else if(choice == "GREEN"){
			g = 255;	
		}else if(choice == "BLACK"){
		}else if(choice == "BLUE"){
			b = 255;
		}else if(choice == "WHITE"){
			r = 255;
			g = 255;
			b = 255;
		}else{
			cout <<"Invalid color choice, must be RED, GREEN, BLUE, WHITE or BLACK\n";
			return -1;
		}
		
	}
	cout << "color: "<<r<<" "<<g<< " "<<b<< " \n";


	//Video Initialization
	VideoCapture cap(0); //open the video camera no. 0
	if(!cap.isOpened()){
		cout << "Cannot open camera" << endl;
		return -1;
	}
	namedWindow("Unchanged", CV_WINDOW_AUTOSIZE);
	namedWindow("Motion_Detection", CV_WINDOW_AUTOSIZE);
	
	//Motion parameters
	int num_changes = 0;
	int num_sequence = 0;
	Scalar color(b, g, r);
	int motion_pix_threshold = 5;
	int motion_threshold = 10;
	int max_deviat = 20;
	Point pnt(-1, -1);

	
	// Motion Detection Objects
	Mat motion; 
	Mat result;
	Mat previous_frame;
	Mat next_frame;
	Mat kernal = getStructuringElement(MORPH_RECT, Size(2,2));
	
	//Image Initialization
	cap.read(previous_frame); 
	cap.read(next_frame);
	
	//Greyscale
	cvtColor(previous_frame, previous_frame, CV_RGB2GRAY);
	cvtColor(next_frame, next_frame, CV_RGB2GRAY);

	while(true){
		//Contour initialization
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		previous_frame = next_frame;
		cap.read(next_frame);
		Mat original = next_frame;
		result = next_frame;
		
		imshow("Unchanged", original);
		cvtColor(next_frame, next_frame, CV_RGB2GRAY);

		//calculation: difference between images; AND-operations
		//threshold image, low differences are ignored (i.e. contrast change to sun)
		absdiff(previous_frame, next_frame, motion);
		threshold(motion, motion, motion_threshold, SIZE, CV_THRESH_BINARY);
		dilate(motion, motion, kernal, pnt, 2);
		findContours(motion, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		cvtColor(motion, motion, CV_GRAY2RGB);
		for(int i = 0; i< contours.size(); i++){
			drawContours(motion, contours, i, color, CV_FILLED, CONT_SIZE, hierarchy, 0);
		}
		absdiff(motion, original, result);
		imshow("Motion_Detection", result);
		if(waitKey(5)==27){
			cout << "Esc pressed" << endl;
			break;
		}
	}
	return 0;
}

