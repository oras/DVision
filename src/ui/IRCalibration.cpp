#include "IRCalibration.h"
#include "ui_IRCalibration.h"
#include "VStreamSimulator.h"
#include <QSettings>
#include "CameraCapture.h"
#include <QGraphicsEllipseItem>
#include <MainWindow.h>
#include <QStyle>

IRCalibration::IRCalibration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IRCalibration)
{
    ui->setupUi(this);


    scene=new CalScene("IR",this);
    ui->graphicsView->setScene(this->scene);
    ui->graphicsView->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->graphicsView->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->graphicsView->show();


    scene2=new CalScene("RGB",this);
    ui->graphicsView_2->setScene(this->scene2);
    ui->graphicsView_2->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->graphicsView_2->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->graphicsView_2->show();



    connect(CameraCapture::instance(),SIGNAL(streamImage(const QImage&)), this, SLOT(streamImage(const QImage&)));
    connect(CameraCapture::instance(),SIGNAL(streamImageIR(const QImage&)), this, SLOT(streamImageIR(const QImage&)));


    calibMode=false;
    IRPoints=new QPointF[MAX_POINTS];
    RGBPoints=new QPointF[MAX_POINTS];
    IRCounter=RGBCounter=0;
    mc_pauseRequired=false;

    testMode=false;

    loadDiskDefaultImg();
}

IRCalibration::~IRCalibration()
{
    delete ui;
}

void IRCalibration::streamImage(const QImage &image){
    if(!calibMode){
        img=image.copy().scaled(640,480);

        if(testMode){
            if(mc_pauseRequired)
                  mc_pauseManager.wait(&mc_continue);

             mc_pauseRequired=true;
             cvtColor(ImgRecoTool::QImageToCvMat(imgIR), src, CV_BGR2GRAY);
             mc_pauseRequired=false;

             cv::resize(src,this->dst,*size);

             applyColorMap(dst, src, COLORMAP_JET);

             colorMap=ImgRecoTool::QImageToCvMat(img);

             drawHotZones(src,colorMap);

             img=ImgRecoTool::cvMatToQImage(colorMap);
        }

        scene2->clear();
        scene2->addPixmap(QPixmap::fromImage(img).scaled(ui->graphicsView->size()));

    }
}

void IRCalibration::streamImageIR(const QImage &image){
    if(!calibMode){
        if(mc_pauseRequired)
              mc_pauseManager.wait(&mc_continue);

        mc_pauseRequired=true;
        imgIR=image.copy().scaled(640,480);
        mc_pauseRequired=false;

        scene->clear();
        scene->addPixmap(QPixmap::fromImage(imgIR).scaled(ui->graphicsView->size()));
    }
}

void IRCalibration::on_pushButton_3_clicked()
{
    calibMode=false;
    testMode=false;

    scene2->clear();
    scene2->addPixmap(QPixmap::fromImage(img).scaled(ui->graphicsView_2->size()));
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(imgIR).scaled(ui->graphicsView->size()));

    calibMode=true;
    resetCalibration();
}

void IRCalibration::on_pushButton_4_clicked()
{
    testMode=false;
    calibMode=false;
}

void IRCalibration::addNewIRPoint(QPointF p){
    IRPoints[IRCounter]=p;

    IRCounter++;
}

void IRCalibration::addNewRGBPoint(QPointF p){
    RGBPoints[RGBCounter]=p;

    RGBCounter++;
}

int IRCalibration::getIRPointsNum(){
    return IRCounter;
}

int IRCalibration::getRGBPointsNum(){
    return RGBCounter;
}

CalScene::CalScene(QString format,QObject *parent){;
    pen.setWidth(1);
    pen.setBrush(Qt::green);
    painter.save();
    painter.setMatrixEnabled(false);
    QFont font("David");
    font.setPointSize (14);
    this->myColor=Qt::green;

    painter.setFont(font);
    painter.setPen(Qt::green);
    this->format=format;
    par=parent;

}

void CalScene::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    IRCalibration *calParent=dynamic_cast<IRCalibration*>(par);

    // Add the new point by scene format
    if(format=="RGB"){
        if(calParent->getIRPointsNum()==calParent->getRGBPointsNum()){
            calParent->addNewRGBPoint(e->scenePos());
            text = addText(QString::number(calParent->getRGBPointsNum()));
            drawPoint(e->scenePos());
        }
    }
    else{
        if(calParent->getRGBPointsNum()-calParent->getIRPointsNum()){
            calParent->addNewIRPoint(e->scenePos());
            text = addText(QString::number(calParent->getIRPointsNum()));
             drawPoint(e->scenePos());
        }
    }



}

void CalScene::drawPoint(QPointF point){
    int rad=2;
    addEllipse(point.x()-rad, point.y()-rad, rad*2.0, rad*2.0,
    pen, QBrush(Qt::SolidPattern));
    text->setDefaultTextColor(myColor);
    text->setPos(point.x(), point.y());
}

void IRCalibration::on_pushButton_clicked()
{
    if(RGBCounter>0){
        int x11=RGBPoints[0].x(),
            x12=IRPoints[0].x(),
            y11=RGBPoints[0].y(),
            y12=IRPoints[0].y(),
            x21=RGBPoints[1].x(),
            x22=IRPoints[1].x(),
            y21=RGBPoints[1].y(),
            y22=IRPoints[1].y();


        ratio=(double)(sqrt((double)pow(x11-x21,2)+(double)pow(y11-y21,2))/
                       (sqrt((double)pow(x12-x22,2)+(double)pow(y12-y22,2))));

        ratio+=0.2;
        diffX=(x12*ratio+25)-x11;
        diffY=y12*ratio-y11;
    }

    size=new cv::Size((int)imgIR.width()*ratio,(int)imgIR.height()*ratio);
    testMode=true;
    calibMode=false;
}

void IRCalibration::resetCalibration(){
    RGBCounter=IRCounter=0;

    delete[] IRPoints;
    delete[] RGBPoints;

    IRPoints=new QPointF[MAX_POINTS];
    RGBPoints=new QPointF[MAX_POINTS];
}

void IRCalibration::drawHotZones(Mat &src, Mat &dst){
    // Blue color
    int R=35;

    for(int r=0;r<dst.rows;r++){
        for(int c=0;c<dst.cols;c++){
            if(src.at<Vec3b>(r,c)[2]>=R){
                if((r-diffY>=0)&&(r-diffY<dst.rows)&&(c-diffX>=0)&&(c-diffX<dst.cols)){
                    dst.at<Vec3b>(r-diffY,c-diffX)[0]=src.at<Vec3b>(r,c)[0];
                    dst.at<Vec3b>(r-diffY,c-diffX)[1]=src.at<Vec3b>(r,c)[1];
                    dst.at<Vec3b>(r-diffY,c-diffX)[2]=src.at<Vec3b>(r,c)[2];
                }
            }
        }
    }
}

bool IRCalibration::isFlameTexture(int r, int g, int b){
   int gr=g/(r+1);
   int br=b/(r+1);

    if((gr>=0.4 && gr<=0.99) && (br>=0 && br<=0.56) && (r>179) && (g<238) && (b<130))
        return true;

    return false;
}

void IRCalibration::on_pushButton_2_clicked()
{
    // Save image name + path to disk.
    QFile mFile("/home/or/workspace/apm_planner/CalibrationData.txt");

    if(mFile.open(QIODevice::ReadWrite | QIODevice::Truncate)){
        QTextStream stream(&mFile);
        stream<<this->ratio<<endl<<this->diffX<<endl<<this->diffY;

        mFile.close();

        po=new cv::Point(this->diffX,this->diffY);
        emit  updateImageIRDiff(*po);

        QMessageBox::information(
                    this,
                    tr("APMPlanner 2.0"),
                    tr("The settings has been saved"));
    }
}

void IRCalibration::loadDiskDefaultImg(){
    QFile mFile("/home/or/workspace/apm_planner/CalibrationData.txt");

    if(mFile.open(QIODevice::ReadOnly)){
        QTextStream in(&mFile);

        this->ratio=in.readLine().toDouble();
        this->diffX=in.readLine().toInt();
        this->diffY=in.readLine().toInt();


        mFile.close();

        po=new cv::Point(this->diffX,this->diffY);
        emit  updateImageIRDiff(*po);
    }
}
