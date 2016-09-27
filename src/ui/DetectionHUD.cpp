#include "DetectionHUD.h"
#include "ui_DetectionHUD.h"
#include "configuration.h"
#include <qquickview.h>
#include <EventsDetection.h>
#include <MainWindow.h>
#include <QSettings>

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
        connect(VStreamSimulator::instance(),SIGNAL(streamImage(const QImage&)), this, SLOT(streamImage(const QImage&)));
        connect(EventsDetection::instance(),SIGNAL(imgInfoReady()), this, SLOT(imgInfoReady()));

        connect(EventsDetection::instance(),SIGNAL(imgInfoReady()), this, SLOT(imgInfoReady()));
        connect(CameraCapture::instance(),SIGNAL(videoStreamDisconnected()), this, SLOT(videoStreamDisconnected()));
        connect(CameraCapture::instance(),SIGNAL(imgInfoReady()), this, SLOT(imgInfoReady()));
        connect(CameraCapture::instance(),SIGNAL(streamImage(const QImage&)), this, SLOT(streamImage(const QImage&)));



   warn=new WarnSound();

   lastTime=clock();
}


void DetectionHUD::setDHUD(QImage &img){




}

void DetectionHUD::videoStreamDisconnected(){ 
    scene->clear();
    ui->label->show();
}

DetectionHUD::~DetectionHUD()
{
    delete ui;
}

void DetectionHUD::streamImage(const QImage &img){
    // Refresh scene & update next video frame.
    ui->label->hide();
    scene->clear();
    while(irt::m_continue.tryLock());
        this->img=img.copy();
    irt::m_continue.unlock();

    if(!vCircle.empty()){
       // while(irt::m_continue.tryLock());
        matImg=ImgRecoTool::QImageToCvMat(img);
        //irt::m_continue.unlock();

        ImgRecoTool::drawCircles(&vCircle,matImg);

        this->img=ImgRecoTool::cvMatToQImage(matImg);
    }

    scene->addPixmap(QPixmap::fromImage(this->img).scaled(ui->graphicsView->size(),
                                                      Qt::KeepAspectRatio, Qt::FastTransformation));
}

QImage* DetectionHUD::getImage(){
    return &img;
}

void DetectionHUD::imgInfoReady(){
    this->vCircle.clear();
    this->vCircle=EventsDetection::instance()->getCircleVector();
}
