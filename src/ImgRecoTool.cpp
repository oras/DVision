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

#include "ImgRecoTool.h"
#include <QtGlobal>
#include <QtDebug>


namespace irt{

    Mat ImgRecoTool::hsiTransform(Mat &src){
        Mat tempImg=src.clone();
        float pi=3.14159265;

        Mat hsi(src.rows,src.cols,src.type());

        float r,g,b,h,s,in,min_val;

        for(int i=0;i<tempImg.rows;i++){
            for(int j=0;j<tempImg.cols;j++){
                b=tempImg.at<Vec3b>(i,j)[0];
                g=tempImg.at<Vec3b>(i,j)[1];
                r=tempImg.at<Vec3b>(i,j)[2];
                in=(b+g+r)/3;

                min_val=std::min(r,std::min(b,g));

                s=1-3*(min_val/(b+g+r));

                if(s<0.00001)
                    s=0;
                else if(s>0.99999)
                    s=1;

                if(s!=0){
                    h=0.5*((r-g)+(r-b))/sqrt(((r-g)*(r-g))+((r-b)*(g-b)));
                    h=acos(h);
                }

                if(g>b)
                    h=((360*pi)/180.0)-h;

                hsi.at<Vec3b>(i,j)[0]=(h*180)/pi;
                hsi.at<Vec3b>(i,j)[1]=s*100;
                hsi.at<Vec3b>(i,j)[2]=in;
            }
        }

        return hsi;
    }

    Mat ImgRecoTool::getRGBHistogram(Mat &src){
        Mat dst;

        // Separate the image in 3 places ( B, G and R )
        std::vector<Mat> bgr_planes;
        split(src, bgr_planes);

        // Establish the number of bins
        int histSize = 256;

        // Set the ranges ( for B,G,R) )
        float range[] = { 0, 256 } ;
        const float* histRange = { range };

        bool uniform = true; bool accumulate = false;

        Mat b_hist, g_hist, r_hist;

        // Compute the histograms:
        calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
        calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
        calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

        // Draw the histograms for B, G and R
        int hist_w = 512; int hist_h = 400;
        int bin_w = cvRound( (double) hist_w/histSize );

        Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

        // Normalize the result to [ 0, histImage.rows ]
        normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
        normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
        normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

        // Draw for each channel
        for( int i = 1; i < histSize; i++ ){
              line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                               cv::Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                               Scalar( 255, 0, 0), 2, 8, 0  );
              line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                               cv::Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                               Scalar( 0, 255, 0), 2, 8, 0  );
              line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                               cv::Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                               Scalar( 0, 0, 255), 2, 8, 0  );
        }

        return histImage;
    }

    Mat ImgRecoTool::getImageChannel(Mat mat,int num){
        // "channels" is a vector of 3 Mat arrays:
        vector<Mat> channels(3);
        // split img:
        split(mat, channels);
        // get the channels (dont forget they follow BGR order in OpenCV)
        return channels[num];
    }

    Node* ImgRecoTool::getSaliencyMap(Mat &srcImg, int scan, int thresh){
        Node *head=NULL;
        cv::Point *jP;
        Mat binaryImage;
        Mat tempImage=srcImg.clone();
        cv::Point point[2]; //4 cv::Points of square mark data to analyze.
        int maxX, maxY=0;
        int i, j=0; //Inner analyzing data image matrix indexes.
        int sum=0;

        threshold(getImageChannel(srcImg,2),binaryImage,thresh,255,0);

        //The row&col (square) scan option.
        if(scan==0){
            for(int r=0;r<binaryImage.rows;r++){
                for(int c=0;c<binaryImage.cols;c++){
                    if(binaryImage.at<uchar>(r,c)==255){
                        jP=pointInSquare(c,r,head);
                        if(jP==NULL){
                            point[0].x=c;
                            point[1].y=r;
                            maxX=c;
                            maxY=r;
                            c++;
                            i=c;
                            j=r;

                            //Horizon interesting square data finder.
                            while(i<binaryImage.cols && abs(i-maxX)< binaryImage.cols*0.3){
                                jP=pointInSquare(i,j,head);
                                if(jP==NULL){
                                    if(binaryImage.at<uchar>(j,i)==255){
                                        maxX=i;
                                    }
                                }
                                else{
                                    break;
                                }

                                i++;
                            }

                            i=maxX;

                            //Vertical interesting square data finder.
                            while(j<binaryImage.rows && abs(j-maxY)< binaryImage.rows*0.3){
                                jP=pointInSquare(i,j,head);
                                if(jP==NULL){
                                    if(binaryImage.at<uchar>(j,i)==255){
                                        maxY=j;
                                    }
                                }
                                else{
                                    break;
                                }

                                j++;
                            }

                            point[1].x=maxX;
                            point[1].y=maxY;

                            if(head==NULL){
                                head=new Node(point,NULL);


                                sum++;
                            }
                            else{
                                Node *curr= new Node(point,head);

                                head=curr;

                            }
                        }
                        else{
                            c=jP[1].x + 1;
                        }
                    }
                }
            }
        }
        //The diagonal scan option.
        else if(scan==1){
            for(int r=0;r<binaryImage.rows;r++){
                for(int c=0;c<binaryImage.cols;c++){
                    if(binaryImage.at<uchar>(r,c)==255){
                        jP=pointInSquare(c,r,head);
                        if(jP==NULL){
                            point[0].x=c;
                            point[1].y=r;
                            maxX=c;
                            maxY=r;
                            c++;
                            i=c;
                            j=r;

                            //Horizon interesting square data finder.
                            while((i<binaryImage.cols && abs(i-maxX)< binaryImage.cols*0.3)&&
                                    (j<binaryImage.cols && abs(j-maxY)<binaryImage.rows*0.3)){

                                if(pointInSquare(i,j,head)!=NULL||
                                   pointInSquare(c,j,head)!=NULL||
                                   pointInSquare(i,r,head)!=NULL)
                                    break;

                                else if(binaryImage.at<uchar>(j,i)==255){
                                        maxX=i;
                                        maxY=j;

                                }

                                i++;
                                j++;
                            }

                            point[1].x=maxX;
                            point[1].y=maxY;

                            if(head==NULL){
                                head=new Node(point,NULL);


                                sum++;
                            }
                            else{
                                Node *curr= new Node(point,head);

                                head=curr;

                            }

                        }
                        else{
                            c=jP[1].x + 1;
                        }

                    }
                }
            }
        }

        return head;
    }

    Point *ImgRecoTool::pointInSquare(int x, int y, Node *squares){
        for(Node *p=squares;p!=NULL;p=p->next){
            if(p->point!=NULL)
                if(x>=p->point[0].x&&x<=p->point[1].x&&y>=p->point[0].y&&y<=p->point[1].y){
                    return p->point;
                }
        }

        return NULL;
    }

    void ImgRecoTool::drawSaliencyDatas(Node *squares, Mat &src){
        //Squares mark color:
        const int R=112, G=238, B=28;

        for(Node *p=squares;p!=NULL;p=p->next){

            //Draw horizontal square lines
            for(int i=p->point[0].x;i<=p->point[1].x;i++){
                src.at<Vec3b>(p->point[0].y,i)[0]=R;
                src.at<Vec3b>(p->point[1].y,i)[0]=R;
                src.at<Vec3b>(p->point[0].y,i)[1]=G;
                src.at<Vec3b>(p->point[1].y,i)[1]=G;
                src.at<Vec3b>(p->point[0].y,i)[2]=B;
                src.at<Vec3b>(p->point[1].y,i)[2]=B;
            }

            //Draw verticals square lines:
            for(int j=p->point[0].y;j<=p->point[1].y;j++){
                src.at<Vec3b>(j,p->point[0].x)[0]=R;
                src.at<Vec3b>(j,p->point[1].x)[0]=R;
                src.at<Vec3b>(j,p->point[0].x)[1]=G;
                src.at<Vec3b>(j,p->point[1].x)[1]=G;
                src.at<Vec3b>(j,p->point[0].x)[2]=B;
                src.at<Vec3b>(j,p->point[1].x)[2]=B;
            }
        }
    }


    void ImgRecoTool::CannyThreshold(int lowThreshold,Mat &src){
        Mat detected_edges,src_gray, dstDOG;
        int ratio=3,kernel_size=3;

        src_gray=getImageChannel(src,2);

        //Reduce noise with a kernel 3x3
        blur(src_gray, detected_edges, Size(3,3) );

        //Canny detector.
        Canny(detected_edges,detected_edges,lowThreshold,lowThreshold*ratio,kernel_size);

        //Using Canny's output as a mask, we display our result
        dstDOG = Scalar::all(0);

        src.copyTo(dstDOG, detected_edges);

        src=dstDOG.clone();
    }

/*
   Functions to convert between OpenCV's cv::Mat and Qt's QImage and QPixmap.

   Andy Maloney
   23 November 2013
   http://asmaloney.com/2013/11/code/converting-between-cvmat-and-qimage-or-qpixmap
 */

    QImage ImgRecoTool::cvMatToQImage(const Mat &inMat){
     switch (inMat.type()){
        // 8-bit, 4 channel
        case CV_8UC4:{
           QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32 );

           return image;
        }

        // 8-bit, 3 channel
        case CV_8UC3:{
           QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888 );

           return image.rgbSwapped();
        }

        // 8-bit, 1 channel
        case CV_8UC1:{
           static QVector<QRgb>  sColorTable;

           // only create our color table once
           if ( sColorTable.isEmpty() ){
              for ( int i = 0; i < 256; ++i )
                 sColorTable.push_back( qRgb( i, i, i ) );
           }

           QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8 );

           image.setColorTable( sColorTable );

           return image;
        }

        default:
           qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
           break;
     }

     return QImage();
    }

    Mat ImgRecoTool::QImageToCvMat( const QImage &inImage, bool inCloneImageData)
       {
          switch ( inImage.format() )
          {
             // 8-bit, 4 channel
             case QImage::Format_RGB32:
             {
                cv::Mat  mat( inImage.height(), inImage.width(), CV_8UC4, const_cast<uchar*>(inImage.bits()), inImage.bytesPerLine() );

                return (inCloneImageData ? mat.clone() : mat);
             }

             // 8-bit, 3 channel
             case QImage::Format_RGB888:
             {
                if ( !inCloneImageData )
                   qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning since we use a temporary QImage";

                QImage   swapped = inImage.rgbSwapped();

                return cv::Mat( swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine() ).clone();
             }

             // 8-bit, 1 channel
             case QImage::Format_Indexed8:
             {
                cv::Mat  mat( inImage.height(), inImage.width(), CV_8UC1, const_cast<uchar*>(inImage.bits()), inImage.bytesPerLine() );

                return (inCloneImageData ? mat.clone() : mat);
             }

             default:
                qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
                break;
          }

          return Mat();
    }

    void ImgRecoTool::transferToBinaryImage(int val, Mat &src){
      threshold(getImageChannel(src,2),src,val,255,0);
    }

    void ImgRecoTool::markRGBflameAreas(Mat &src, Node *areas, int horizon){
        const int R=112, G=238, B=28;		//RGB mark of flame color.
        const int minR=243, maxR=255,		//RGB rule of flame color detection.
                  minG=148, maxG=255,
                  minB=72, maxB=183;



        float r,g,b, gr, br;

        int sum=0;

        horizon=src.rows-horizon-1;

        for(Node *p=areas;p!=NULL;p=p->next){
            for(int i=(int)p->point[0].y;i<=(int)p->point[1].y && i<src.rows;i++){
                for(int j=(int)p->point[0].x;j<=(int)p->point[1].x && j<src.cols;j++){
                    b=src.at<Vec3b>(i,j)[0];
                    g=src.at<Vec3b>(i,j)[1];
                    r=src.at<Vec3b>(i,j)[2];

                    gr=g/(r+1);
                    br=b/(r+1);

                    if((i>horizon)&& (((r>=minR && r<=maxR) && (g>=minG && g<=maxG) && (b>=minB && b<=maxB)) ||
                            ((gr>=0.4 && gr<=0.8) && (br>=0 && br<=0.5) && (r>200) && (g<200) && (b<100)))){

                        src.at<Vec3b>(i,j)[0]=R;
                        src.at<Vec3b>(i,j)[1]=G;
                        src.at<Vec3b>(i,j)[2]=B;
                        sum++;
                    }
                }
            }
        }
    }

    void ImgRecoTool::contrast(Mat &src, int val){
        const int COLOR_NUM=3;
        const int C=259;
        const int C2=255;
        const int C3=128;
        const float FACTOR=(C*(val + C2))/(C2*(C-val));
        for(int i=0;i<src.rows;i++)
            for(int j=0;j<src.cols;j++)
                for(int c=0;c<COLOR_NUM;c++){
                    src.at<Vec3b>(i,j)[c]=
                            saturate_cast<uchar>(FACTOR*(src.at<Vec3b>(i,j)[c]-C3)+C3);
                }

    }

    void ImgRecoTool::drawHorizon(const unsigned int &resolution, const cv::Point *hRoot,Mat &src){
        const int IMG_BORDER_POINTS=2;
        const int NUM_OF_X_POINTS=resolution+IMG_BORDER_POINTS;
        int y,y0,y1,x0,x1=0;

        //Horizon mark color:
        const int R=112, G=238, B=28;

        /*They are two diffrent methods to draw the horizon:
         * 1. Linear interpolation.
         * 2. Polynomial interpolation (Didn't realize).
        */

        //1

        for(int i=0;i<NUM_OF_X_POINTS-1;i++){
            for(int x=hRoot[i].x;x<hRoot[i+1].x;x++){
                y0=hRoot[i].y;
                x0=hRoot[i].x;
                y1=hRoot[i+1].y;
                x1=hRoot[i+1].x;

                y=y0+(y1-y0)*(x-x0)/(x1-x0);

                src.at<Vec3b>(y,x)[0]=R;
                src.at<Vec3b>(y+1,x)[0]=R;
                src.at<Vec3b>(y,x)[1]=G;
                src.at<Vec3b>(y+1,x)[1]=G;
                src.at<Vec3b>(y,x)[2]=B;
                src.at<Vec3b>(y+1,x)[1]=G;

            }
        }


        //2
    }

    cv::Point* ImgRecoTool::createHorizon(const unsigned int &resolution,Mat &img,int binaryThresh){
        const int IMG_BORDER_POINTS=2;
        const int MAX_RESOLUTION=resolution;
        const int IMG_WIDTH_PIX_SIZE=img.cols;
        const int IMG_HEIGHT_PIX_SIZE=img.rows;
        const int IMG_PART_SIZE=IMG_WIDTH_PIX_SIZE/(MAX_RESOLUTION+1);
        int x=0;
        Mat binaryImg;

        cv::Point *root=new cv::Point[MAX_RESOLUTION+IMG_BORDER_POINTS];

        threshold(getImageChannel(img,2),binaryImg,binaryThresh,255,0);

        for(int i=0;i<=MAX_RESOLUTION+1;i++){
            if(i==MAX_RESOLUTION+1)
               root[i].x=img.cols-1;
            else
               root[i].x=x;

            //Find the Y of cv::Point:
            for(int y=0;y<IMG_HEIGHT_PIX_SIZE;y++){
                 if(binaryImg.at<uchar>(y,x)==0){
                     root[i].y=y;
                     break;
                 }
            }

            x+=IMG_PART_SIZE;
        }

        return root;
    }

    bool ImgRecoTool::markCircleOnFlame(Mat &src,Node* squares, Horizon* horizon){
        bool warning=false;

        const int minR=243, maxR=255,		//RGB rule of flame color detection.
                  minG=148, maxG=255,
                  minB=72, maxB=183;

        const int R=112, G=238, B=28;


        float r,g,b, gr, br;


        cv::Point* point=new Point[3];


        for(const Node *p=squares;p!=NULL;p=p->next){
            int xSum=0;
            int ySum=0;
            int yBigest=0;
            int xBigest=0;
            int sum=0;

            for(int i=(int)p->point[0].y;i<=(int)p->point[1].y && i<src.rows;i++){
                for(int j=(int)p->point[0].x;j<=(int)p->point[1].x && j<src.cols;j++){


                    b=src.at<Vec3b>(i,j)[0];
                    g=src.at<Vec3b>(i,j)[1];
                    r=src.at<Vec3b>(i,j)[2];

                    gr=g/(r+1);
                    br=b/(r+1);

                    // Debug
                    /*if((r>=255)&&((j==304&&i==155)
                     ||(j==304&&i==154)
                     ||(j==303&&i==155)
                     ||(j==302&&i==156)
                     ||(j==302&&i==157)
                     ||(j==303&&i==157)
                     ||(j==302&&i==158)
                     ||(j==301&&i==159)
                     ||(j==304&&i==154)
                     ||(j==306&&i==154)))
                        cout<<"Flame has been detected!"<<endl;*/

                   if(r>198&&b<130){
                       cout<<"Flame has been detected!"<<endl;

                   }

                    if((pointUnderHorizon(j,i,horizon->getPrevHorizonPoint(j,src.cols),horizon->getForwardHorizonPoint(j,src.cols)))&&
                            /*(((r>=minR && r<=maxR) && (g>=minG && g<=maxG) && (b>=minB && b<=maxB)) ||*/
                            ((gr>=0.4 && gr<=0.99) && (br>=0 && br<=0.56) && (r>179) && (g<238) && (b<130))){


                      if(i>yBigest)
                         yBigest=i;

                      if(j>xBigest)
                          xBigest=j;

                      xSum+=j;
                      ySum+=i;

                      sum++;

                    }
                }
            }

            if(sum>0){

                point[0].x=(int)xSum/sum;
                point[0].y=(int)ySum/sum;

                yBigest=abs(yBigest-point->y);
                xBigest=abs(xBigest-point->x);

                int rad=(yBigest>xBigest?yBigest:xBigest);

                if(rad<40)
                    rad=40;

                circle(src,point[0],rad,Scalar(R,G,B),2,8,0);

                warning=true;
            }
        }

        return warning;
    }

    bool ImgRecoTool::pointUnderHorizon(const int &x,const int &y,const cv::Point* h1,const cv::Point* h2){
        int yDiagonal,y0,y1,x1,x0=0;

        y0=h1->y;
        x0=h1->x;
        y1=h2->y;
        x1=h2->x;

        yDiagonal=y0+(y1-y0)*(x-x0)/(x1-x0);

        if(y>=yDiagonal)
            return true;

        return false;
    }

    void ImgRecoTool::releaseHRoot(cv::Point *root){
       delete[] root;
       root=NULL;
    }

    void ImgRecoTool::freeNodeLinkedList(Node *p){
        Node* toDelete = p;
        while(toDelete != NULL) {
            Node* next = toDelete->next;
            delete toDelete;
            toDelete = next;
        }
    }
}


