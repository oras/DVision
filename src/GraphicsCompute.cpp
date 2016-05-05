#include "GraphicsCompute.h"

GraphicsCompute::GraphicsCompute(QObject *parent)
    :QThread(parent)
{
    //connect(parent,SIGNAL(startCompute()), this, SLOT(startCompute()));
   img=dynamic_cast<DetectionHUD*> (parent)->getImage();
}


void GraphicsCompute::run(){
    mutex.lock();
    Mat matImg;


    // Display detected horizon:



    switch(2){

    case 1:
    {
                matImg=ImgRecoTool::QImageToCvMat(*img);
                ImgRecoTool::contrast(matImg,100);
                this->hRoot=ImgRecoTool::createHorizon(50,matImg,170);

                matImg=ImgRecoTool::QImageToCvMat(*img);
                ImgRecoTool::drawHorizon(50,this->hRoot,matImg);
                *img=ImgRecoTool::cvMatToQImage(matImg);

                ImgRecoTool::releaseHRoot(hRoot);

                emit imageReady(*img);

        break;
    }
    case 2:
    {
                matImg=ImgRecoTool::QImageToCvMat(*img);
                this->root=ImgRecoTool::getSaliencyMap(matImg,1,100);
                //ImgRecoTool::contrast(matImg,100);
                this->hRoot=ImgRecoTool::createHorizon(50,matImg,170);

                Horizon* horizon=new Horizon(this->hRoot,50);

                if(ImgRecoTool::markCircleOnFlame(matImg,this->root,horizon)){
                    emit playWarningSound();

                }
                else {
                    emit stopWarningSound();

                }

                *img=ImgRecoTool::cvMatToQImage(matImg);

                ImgRecoTool::releaseHRoot(hRoot);
                ImgRecoTool::freeLinkedList(root);

                delete horizon;

                emit imageReady(*img);

        break;
    }
    case 3:
    {
        matImg=ImgRecoTool::QImageToCvMat(*img);
                this->root=ImgRecoTool::getSaliencyMap(matImg,1,100);
                //ImgRecoTool::contrast(matImg,100);

                ImgRecoTool::drawSaliencyDatas(this->root,matImg);
                *img=ImgRecoTool::cvMatToQImage(matImg);

                ImgRecoTool::freeLinkedList(root);


                emit imageReady(*img);

        break;
    }
    }

    //Display detected

    mutex.unlock();

    QThread::deleteLater();
}




