#include "DetectionHUD.h"
#include "ui_DetectionHUD.h"
#include "configuration.h"
#include <qquickview.h>


DetectionHUD::DetectionHUD(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DetectionHUD)
{
    ui->setupUi(this);

    this->hRoot=NULL;

    ui->label->setStyleSheet("qproperty-alignment: 'AlignCenter';font-size: 40px;");
    this->scene=new QGraphicsScene();
    ui->graphicsView->setScene(this->scene);
    ui->graphicsView->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->graphicsView->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->graphicsView->show();

    connect(VStreamSimulator::instance(),SIGNAL(streamImage(const QImage&)), this, SLOT(streamImage(const QImage&)));
    connect(VStreamSimulator::instance(),SIGNAL(videoStreamDisconnected()), this, SLOT(videoStreamDisconnected()));


    lastTime=clock();

}

void DetectionHUD::streamImage(const QImage &image)
{
    ui->label->hide();

    if (!image.isNull())
    {

        thisTime=clock();
        this->img=image;



        if((thisTime-lastTime>1*NUM_SECONDS)){
            // Set Detection HUD layer:

            //setDHUD(img);
            GraphicsCompute *go=new GraphicsCompute(this);
            connect(go,SIGNAL(imageReady(const QImage&)), this, SLOT(imageReady(const QImage&)),Qt::QueuedConnection);
           emit startCompute();

            lastTime=thisTime;
        }
    }
    else
        ui->label->show();
}

void DetectionHUD::setDHUD(QImage &img){

    // Display detected horizpn:
    Mat matImg=ImgRecoTool::QImageToCvMat(img);
    ImgRecoTool::contrast(matImg,100);
    this->hRoot=ImgRecoTool::createHorizon(50,matImg,170);

    matImg=ImgRecoTool::QImageToCvMat(img);
    ImgRecoTool::drawHorizon(50,this->hRoot,matImg);
    img=ImgRecoTool::cvMatToQImage(matImg);

    ImgRecoTool::releaseHRoot(hRoot);


}

void DetectionHUD::videoStreamDisconnected(){
    scene->clear();
    ui->label->show();
}

DetectionHUD::~DetectionHUD()
{
    delete ui;
}

void DetectionHUD::imageReady(const QImage &img){
    // Refresh scene & update next video frame.
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(img).scaled(ui->graphicsView->size(),
                                                      Qt::KeepAspectRatio, Qt::FastTransformation));
}

QImage* DetectionHUD::getImage(){
    return &img;
}
