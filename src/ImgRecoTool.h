#ifndef IMGRECOTOOL_H
#define IMGRECOTOOL_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <unistd.h>

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

class ImgRecoTool{
    int lowThreshold;
    public:
        ImgRecoTool();

        Mat hsiTransform(Mat &src);

        Mat getRGBHistogram(Mat &src);

        Mat getImageChannel(Mat mat,int num);

        Node* getSaliencyMap(Mat &srcImg, int scan);

        void drawSaliencyDatas(Node *squares, Mat &src);

        Mat CannyThreshold(int,void*,Mat &src);
    private:
        int **pointInSquare(int x,int y, Node *squares);
};

#endif // IMGRECOTOOL_H
