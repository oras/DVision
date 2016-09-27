#include "CameraCapture.h"
#include <QtGlobal>
#include <QProcess>
#include <QDir>
#include <QGCCore.h>

CameraCapture* CameraCapture::instance(){
    static CameraCapture* _instance=0;

    if(_instance==0){
        _instance=new CameraCapture();

        // Set the application as parent to ensure that this object
        // will be destroyed when the main application exits
        _instance->setParent(qApp);
    }

    return _instance;
}

CameraCapture::CameraCapture(QObject *parent){
    cameraNum=1;
    go=true;

    initalizeCameraMounted();

    start(LowPriority);
}

void CameraCapture::initalizeCameraMounted(){
        cap=new VideoCapture(0); // open the video camera no. 0
        capIR=new VideoCapture(1); // open the video camera no. 1


        QProcess process;
        QString path=QDir::currentPath()+"/../../apm_planner/./EasyCap_su.sh";
        process.start(path);

        if (!cap->isOpened())  // if not success, exit program
        {
            cout << "Cannot open the video cam" << endl;
            return ;
        }

        frameRate = (int) cap->get(CV_CAP_PROP_FPS);
        frameRateIR = (int) capIR->get(CV_CAP_PROP_FPS);

       dWidth = cap->get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
       dheight = cap->get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

        dWidthIR = capIR->get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
       dheightIR = capIR->get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video




    /*while(1){
        //cap>>frame; // get a new frame from camera
        //cvtColor(frame, edges, COLOR_BGR2GRAY);
        //GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        //Canny(edges, edges, 0, 30, 3);

    }*/
}

void CameraCapture::run(){
    while(go){
        streamVideo();
    }

    sleep(1);
}

void CameraCapture::streamVideo(){
    int delay = (1000/frameRate);


    while (cap->read(frame))
    {

       if(!QGCCore::SIMULATOR){

           if (frame.channels()== 3){
                cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
                img = QImage((const unsigned char*)(RGBframe.data),
                                  RGBframe.cols,RGBframe.rows,QImage::Format_RGB888);
            }
            else
            {
                img = QImage((const unsigned char*)(frame.data),
                                     frame.cols,frame.rows,QImage::Format_Indexed8);
            }


               streamIR();

               emit streamImage(img);
       }

       this->msleep(delay);


    }

    emit videoStreamDisconnected();
}

void CameraCapture::streamIR(){
   try{
        capIR->read(frameIR);

         if (frameIR.channels()== 3){
             cv::cvtColor(frameIR, RGBframeIR, CV_BGR2RGB);
             imgIR = QImage((const unsigned char*)(RGBframeIR.data),
                               RGBframeIR.cols,RGBframeIR.rows,QImage::Format_RGB888);
         }
         else
         {
             imgIR = QImage((const unsigned char*)(frameIR.data),
                                  frameIR.cols,frameIR.rows,QImage::Format_Indexed8);
         }

         if(!imgIR.isNull())
            emit streamImageIR(imgIR);
    }
    catch(Exception e){

    }

}

CameraCapture::~CameraCapture(){
    mutex.lock();
        con = false;
        go=false;
        cap->release();
        capIR->release();
    mutex.unlock();

    wait();

}
