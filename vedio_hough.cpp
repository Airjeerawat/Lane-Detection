#include "opencv2/opencv.hpp"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <cmath>

#define x1  499
#define x2  831
#define x3  1127
#define x4  263

#define y1  497
#define y2  504
#define y3  635
#define y4  653

using namespace std;
using namespace cv;



int main(){

  	// Create a VideoCapture object and open the input file
  	// If the input is the web camera, pass 0 instead of the video file name
    VideoCapture cap("challenge.mp4");

  	// Check if camera opened successfully
  	if(!cap.isOpened()){
    		cout << "Error opening video stream or file" << endl;
    		return -1;
  	}
    int px1,px2,py1,py2;
    int px1_l,px2_l,py1_l,py2_l;

  	while(1){

        Mat frame ;
        // Capture frame-by-frame
    	cap >> frame;

   		Mat src, dst,dstmask, color_dst;
   		src = frame;
        dstmask = frame;
		//canny and gray
		Canny( src, dst, 50, 200, 3 );
        cvtColor( dst, color_dst, CV_GRAY2BGR );


        int new_w=0;
        int new_h=0;

        //RoI dst1
        if(src.cols>dstmask.cols)
        	new_w=dstmask.cols;
    	else
        	new_w=src.cols;
   		if(src.rows>dstmask.rows)
       		new_h=dstmask.rows;
   		else
       		new_h=src.rows;

   		Rect rectROI(0,0,new_w,new_h);
   		Mat mask(new_h, new_w, CV_8UC1, cv::Scalar(0));

  		Point P1(x1,y1);    //topleft
   		Point P2(x2,y2);	//topright
   		Point P3(x3,y3);	//downright
   		Point P4(x4,y4);	//downleft

   		vector< vector<Point> >  co_ordinates;
   		co_ordinates.push_back(vector<Point>());

   		co_ordinates[0].push_back(P1);
   		co_ordinates[0].push_back(P2);
   		co_ordinates[0].push_back(P3);
   		co_ordinates[0].push_back(P4);
   		drawContours( mask,co_ordinates,0, Scalar(255),CV_FILLED, 8 );

   		Mat srcROI=src(rectROI);
   		Mat dstROI=dst(rectROI);
   		Mat dst1;
   		Mat O_gb,O_Canny;
   		srcROI.copyTo(dst1,mask);
   		imshow("dst1",dst1);

   		//Mat new_ROI;


        cvtColor( dst1, color_dst, CV_BGR2GRAY );

       // imshow("dst",dst);
        imshow("color_dst",color_dst);
        GaussianBlur( color_dst, O_gb, Size( 7, 7), 0, 0 );
        imshow("Blur",O_gb);
        Canny( color_dst,O_Canny, 50, 200, 3 );
       // cout<< color_dst.at<uchar>(20,10) ;
   		imshow("Canny",O_Canny);
        for(int row = 0 ;row < color_dst.rows ; row++){
            for(int col = 0 ;col < color_dst.cols ; col++){
                if(color_dst.at<uchar>(row,col) < 100){
                    O_Canny.at<uchar>(row,col) = 0;
                }
                if (color_dst.at<uchar>(row,col) >= 100){
                    O_Canny.at<uchar>(row,col) = 1;
                }
            }
        }

        imshow("B_W",O_Canny);

        vector<Vec4i> lines;
        HoughLinesP( O_Canny, lines, 1, CV_PI/180,80,30,100);
        for( size_t i = 0; i < lines.size(); i++ ){
            line( src, Point(lines[i][0], lines[i][1]),
                 Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );

           circle(src, Point(lines[i][0],lines[i][1]),3, Scalar(255,255,255),CV_FILLED, 8,0);  //start
           circle(src, Point(lines[i][2], lines[i][3]),3, Scalar(0,255,0),CV_FILLED, 8,0);
            cout << "x1 = " << lines[i][0] << "\n";

            cout << "y1 = " << lines[i][1] << "\n";
            cout << "x2 = " << lines[i][2] << "\n";
            cout << "y2 = " << lines[i][3] << "\n";
            cout << "i = " << i << "\n";
            cout << "line final = " << lines.size() << "\n";
            if(i == 0){
                px1 = lines[i][0];
                py1 = lines[i][1];
                px2 = lines[i][2];
                py2 = lines[i][3];
            }
            else if(i == (lines.size() - 1)){
                px1_l = lines[i][0];
                py1_l = lines[i][1];
                px2_l = lines[i][2];
                py2_l = lines[i][3];
            }

        }
        cout << "px1 = " << px1 << "\n";
        cout << "py1 = " << py1 << "\n";
        cout << "px2 = " << px2 << "\n";
        cout << "py2 = " << py2 << "\n";
        cout << "px1_l = " << px1_l << "\n";
        cout << "py1_l = " << py1_l << "\n";
        cout << "px2_l = " << px2_l << "\n";
        cout << "py2_l = " << py2_l << "\n";

        //fine point action
        float m1,m2;
        m1 = float(py1 - py1_l)/(px1 - px1_l);
        m2 = float(py2 - py2_l)/(px2 - px2_l);
        cout << "m1 = " << m1 << "\n";
        cout << "m2 = " << m1 << "\n";
/*
        //line1
        //y - yp1  = m1*(x - xp1);
        int x,y;
        x = ((((py1 - m1*px1)*(py2 - m2*px2))- (py2 - m2*px2))/(m1 - m2));
        y = ((m1 * (py2 - m2*px2)-(m2 * (py1 - m1*px1))))/(m1 - m2);
*/
        int x,y;
        cout << "(x,y)" << " = (" << x << "," << y << ")\n" ;


        imshow( "Frame", src );

   		if (frame.empty())
   			break;

   		int key_pressed;
        key_pressed = cvWaitKey(0);

        /* v - double back
        * b - step back
        * n - step forward
        * m - double forward
        * q - quit
        * * - noting
        */
        switch (key_pressed)
        {
           case 'v':
               frame -= 5;
                break;
            case 'b':
               frame = frame - 1;
               break;
            case 'n':
               frame = frame + 1 ;
               break;
            case 'm':
               frame += 5;
               break;
            case 'q':
               exit(EXIT_SUCCESS);
               break;
            default:
               break;
        }

        // Press  ESC on keyboard to exit
    	char c=(char)waitKey(25);
    	if(c==27)
      		break;
  	}
  	// When everything done, release the video capture object
  	cap.release();

  	// Closes all the frames
  	destroyAllWindows();

  	return 0;
}
