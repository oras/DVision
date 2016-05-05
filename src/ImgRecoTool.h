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
#include <ImgToolFactory.h>

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


namespace irt{

static const int LOCK=0;
static const int SQUARE=0;
static const int CIRCLE=1;
static const int STOP_LOCK=1;


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

       static void drawCircles(vector<Circ>* v,Mat &src);



       // Generic free linked list function.
       template <typename T> static void freeLinkedList(T* p){
           T* toDelete = p;
           while(toDelete != NULL) {
               T* next = toDelete->next;
               delete toDelete;
               toDelete = next;
           }
       }

private:
        static cv::Point* pointInSquare(int x,int y, Node *squares);

        static bool pointUnderHorizon(const int &x,const int &y,const cv::Point* h1,const cv::Point* h2);

        static bool circleIsContained(Circ &c1, Circ &c2);

        static bool circleIsCutt(Circ &c1, Circ &c2);

        static Circ* circleUnion(Circ &c1, Circ &c2);

        template <typename TN> static void addNewNode(vector<TN>* v, TN* node){
            //ints::iterator it = std::lower_bound( cont.begin(), cont.end(), value, std::greater<int>() ); // find proper position in descending order
              //  cont.insert( it, value ); // insert before iterator it

            if(dynamic_cast<Circ*> (node)!=NULL){
                for( typename vector<TN>::iterator it=v->begin(); it!=v->end();it++){

                    // Check if new circle is contained in exisst one
                    if(circleIsContained(*node,*it))
                        return;

                    // Other wise check if it's part contained (Cuts)
                    if(circleIsCutt(*node,*it)){
                        v->push_back(*circleUnion(*node,*it));
                        v->erase(it);
                            return;
                    }
                }
            }

            v->push_back(*node);
        }



signals:
    void imageReady(const QImage &img);

};
}

#endif // IMGRECOTOOL_H
