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

using namespace cv;
using namespace std;



class Node{
    public:
    int **data;
    Node *next;

    Node(int x[][2], Node * y){
        data=new int*[2];

        for(int i=0;i<=1;i++){
            data[i]=new int[2];
            for(int j=0;j<=1;j++){
                data[i][j]=x[i][j];
            }
        }

        next = y;
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
    private:
        static int **pointInSquare(int x,int y, Node *squares);
};
}

#endif // IMGRECOTOOL_H
