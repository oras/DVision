#include "IR.h"
#include "ui_IR.h"
#include "CameraCapture.h"
#include <QSettings>
#include "VStreamSimulator.h"

IR::IR(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IR)
{
    ui->setupUi(this);
    this->scene=new QGraphicsScene();
    ui->graphicsView->setScene(this->scene);
    ui->graphicsView->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->graphicsView->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->graphicsView->show();


     //connect(VStreamSimulator::instance(),SIGNAL(streamImage(const QImage&)), this, SLOT(streamImage(const QImage&)));
     connect(CameraCapture::instance(),SIGNAL(videoStreamDisconnected()), this, SLOT(videoStreamDisconnected()));
     connect(CameraCapture::instance(),SIGNAL(streamImageIR(const QImage&)), this, SLOT(streamImageIR(const QImage&)));

}

 void IR::streamImageIR(const QImage &image){
     scene->clear();

     while(irt::m_continue.tryLock());
         this->img=image.copy();
     irt::m_continue.unlock();


     cvtColor(ImgRecoTool::QImageToCvMat(img), imGray, CV_BGR2GRAY);
     applyColorMap(imGray, colorMap, COLORMAP_JET);

     scene->addPixmap(QPixmap::fromImage(ImgRecoTool::cvMatToQImage(colorMap)).scaled(ui->graphicsView->size(),
                                                       Qt::KeepAspectRatio, Qt::FastTransformation));
 }


 void IR::videoStreamDisconnected(){

 }

IR::~IR()
{
    delete ui;
}
