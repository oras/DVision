/*=============================================================================
 * EVENTS DITECTION USING THE GROUNDCONTROL PROJECT
 *
 * This code is part of the JCE Software & Computer Enginnering Department
 * research project, Written by Asnin Or All rights 2015-2016.
 *
 * www.jce.ac.il
 *
 * This is an image processing Tool Box using the OpenCV libraty project
===============================================================================*/

#ifndef IMGRECOTOOL_H
#define IMGRECOTOOL_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/imgcodecs.hpp"
#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <QImage>
#include <QPixmap>
#include <Horizon.h>

using namespace cv;
using namespace std;

const int POINT_SIZE=2;

class Node{
    public:
    cv::Point* point;
    Node* next;

    Node(cv::Point* p, Node * y){
        point=new cv::Point[POINT_SIZE];

        for(int i=0;i<=1;i++){
            point[i].x=p[i].x;
            point[i].y=p[i].y;
        }

        next = y;
    }
};

class Circle{
public:
    int radios;
    cv::Point p;
    Circle* next;

    Circle(int x, int y, int radios){

    }

private:
    int getCenter(const int &x1,const int &x2){

        // Check if even
        int length=abs(x2-x1);
        if(length%2){
            return length/2+1;
        }
        else
            return length/2;
    }

    int getRadios(const int &x1, const int &y1, const int &x2, const int &y2){
        int lengthX=abs(x2-x1);
        int lengthY=abs(y2-y1);

        double caliber=double(sqrt(pow(lengthX,2))+double(pow(lengthY,2)));

        int radios=caliber/2;

        return radios;
    }
};



namespace irt{

class ImgRecoTool{
    public:


       static Mat hsiTransform(Mat &src);

        static Mat getRGBHistogram(Mat &src);

        static Mat getImageChannel(Mat mat,int num);

        static Node* getSaliencyMap(Mat &srcImg, int scan, int thresh);

        static void drawSaliencyDatas(Node *squares, Mat &src);

        static void CannyThreshold(int lowThreshold,Mat &src);

        static void transferToBinaryImage(int val, Mat &src);

        static QImage cvMatToQImage(const Mat &inMat);

        static Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData = true );

        static void markRGBflameAreas(Mat &src, Node *areas, int horizon);

        static void contrast(Mat &src, int val);

        static void drawHorizon(const unsigned int &resolution, const cv::Point *hRoot, Mat &src);

        static cv::Point* createHorizon(const unsigned int &resolution,Mat &img,int binaryThresh);

        static void releaseHRoot(cv::Point *root);

       static bool markCircleOnFlame(Mat &src,Node* squares, Horizon* horizon);

        static void freeNodeLinkedList(Node *p);
private:



        static cv::Point* pointInSquare(int x,int y, Node *squares);

        static bool pointUnderHorizon(const int &x,const int &y,const cv::Point* h1,const cv::Point* h2);

signals:
    void imageReady(const QImage &img);

};
}

#endif // IMGRECOTOOL_H
