#include "GraphicsCompute.h"

GraphicsCompute::GraphicsCompute(QObject *parent)
    :QThread(parent)
{
    connect(parent,SIGNAL(startCompute()), this, SLOT(startCompute()));
   img=dynamic_cast<DetectionHUD*> (parent)->getImage();
}



void GraphicsCompute::run(){

    // Display detected horizon:

    mutex.lock();
        Mat matImg=ImgRecoTool::QImageToCvMat(*img);
        ImgRecoTool::contrast(matImg,100);
        this->hRoot=ImgRecoTool::createHorizon(50,matImg,170);

        matImg=ImgRecoTool::QImageToCvMat(*img);
        ImgRecoTool::drawHorizon(50,this->hRoot,matImg);
        *img=ImgRecoTool::cvMatToQImage(matImg);

        ImgRecoTool::releaseHRoot(hRoot);

        emit imageReady(*img);

    mutex.unlock();

    QThread::deleteLater();
}




