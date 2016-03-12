#include "FireSmokeReco.h"
#include "ui_FireSmokeReco.h"
#include <QMessageBox>
#include <qpixmap.h>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include <QTextStream>
#include <VideoCapt.h>
#include <Analyze3D.h>

FireSmokeReco::FireSmokeReco(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FireSmokeReco)
{
    ui->setupUi(this);

    // Read default img from disk.
    loadDiskDefaultImg();

    this->defaultImg=imread(this->defaultImgPath.toStdString());

    this->scene = new QGraphicsScene();
    QPixmap m(this->defaultImgPath);
    scene->addPixmap(m.scaled(ui->graphicsView->width()-1,ui->graphicsView->height()-1));

    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(on_horizontalSlider_actionTriggered(int)));
    connect(ui->horizontalSlider,SIGNAL(sliderReleased()),this,SLOT(on_horizontalSlider_sliderReleased()));
    connect(ui->horizontalSlider_2,SIGNAL(valueChanged(int)),this,SLOT(on_horizontalSlider_2_actionTriggered(int)));
    connect(ui->horizontalSlider_2,SIGNAL(sliderReleased()),this,SLOT(on_horizontalSlider_2_sliderReleased()));
    connect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_3_clicked()));


    ui->graphicsView->setScene(this->scene);

    ui->horizontalSlider->setSingleStep(1);
    ui->horizontalSlider->setRange(1,255);
    ui->horizontalSlider_2->setSingleStep(1);
    ui->horizontalSlider_2->setRange(-255,255);

    ui->lcdNumber->setStyleSheet("*{background-color: #69EF5A;}");
    ui->lcdNumber_2->setStyleSheet("*{background-color: #69EF5A;}");
}

void FireSmokeReco::on_buttonBox_accepted()
{
    this->close();
}

void FireSmokeReco::loadDiskDefaultImg(){
    QFile mFile("/home/oras/workspace/apm_planner/Data.txt");

    if(mFile.open(QIODevice::ReadOnly)){
        QTextStream in(&mFile);

        this->defaultImgPath=in.readLine();

        mFile.close();
    }
}

FireSmokeReco::~FireSmokeReco()
{
    delete ui;
}

void FireSmokeReco::on_horizontalSlider_sliderReleased()
{
    Mat src=this->defaultImg.clone();
    ImgRecoTool::CannyThreshold(this->cannyImgThreshold,src);

    updateImage(src);
}

void FireSmokeReco::on_horizontalSlider_actionTriggered(int action)
{
    this->cannyImgThreshold=action;
    ui->lcdNumber->display(action);
}

void FireSmokeReco::updateImage(Mat &src){
    QImage img=ImgRecoTool::cvMatToQImage(src);

    // Update demonstraion image view
    this->scene= new QGraphicsScene();
    QPixmap m(QPixmap::fromImage(img));
    scene->addPixmap(m.scaled(ui->graphicsView->width()-1,ui->graphicsView->height()-1));

    ui->graphicsView->setScene(this->scene);
}

void FireSmokeReco::on_horizontalSlider_2_sliderReleased()
{
    Mat src=imread(this->defaultImgPath.toStdString());
    ImgRecoTool::contrast(src,this->contrast);
    updateImage(src);

    this->defaultImg=src.clone();
}

void FireSmokeReco::on_horizontalSlider_2_actionTriggered(int action)
{
    this->contrast=action;
    ui->lcdNumber_2->display(action);
}

void FireSmokeReco::on_pushButton_2_clicked()
{
   VideoCapt* cap=new VideoCapt(this);
   cap->exec();
   //cap->hide();
   //delete cap;
   //cap=NULL;
}

void FireSmokeReco::on_pushButton_3_clicked()
{
    Analyze3D *analyze=new Analyze3D(this);
    analyze->show();
}
