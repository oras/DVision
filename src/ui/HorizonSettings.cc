#include "HorizonSettings.h"
#include "ui_HorizonSettings.h"
#include <qpixmap.h>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include <QTextStream>

HorizonSettings::HorizonSettings(QWidget *parent,Node *head) :
    QDialog(parent),
    ui(new Ui::HorizonSettings){
    ui->setupUi(this);

    this->hRoot=NULL;
    this->root=NULL;
    this->root=head;
    this->hRootSize=0;

    // Read default img from disk.
    loadDiskDefaultImg();

    ui->setupUi(this);
    ui->lcdNumber->display(1);
    ui->lcdNumber_2->display(1);
    ui->horizontalSlider->setRange(1,255);
    ui->horizontalSlider->setSingleStep(1);

    ui->radioButton_3->setChecked(true);
    ui->radioButton_4->setChecked(true);

    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(on_horizontalSlider_actionTriggered(int)));
    connect(ui->horizontalSlider_2,SIGNAL(valueChanged(int)),this,SLOT(on_horizontalSlider_2_actionTriggered(int)));
    connect(ui->horizontalSlider_2,SIGNAL(sliderReleased()),this,SLOT(on_horizontalSlider_2_sliderReleased()));
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(on_horizontalSlider_sliderReleased()));
    connect(ui->verticalSlider,SIGNAL(valueChanged(int)),this,SLOT(on_verticalSlider_sliderReleased()));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(on_pushButton_clicked()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(on_pushButton_3_clicked()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(on_pushButton_2_clicked()));

    this->defaultImg=imread(this->default_img.toStdString());
    this->scene = new QGraphicsScene();
    QPixmap m(this->default_img);
    scene->addPixmap(m.scaled(ui->graphicsView->width()-1,ui->graphicsView->height()-1));

    ui->verticalSlider->setRange(1,m.height()-1);
    ui->verticalSlider->setSingleStep(1);
    ui->horizontalSlider_2->setRange(1,m.width()-1);
    ui->horizontalSlider_2->setSingleStep(1);

    ui->graphicsView->setScene(this->scene);

    ui->lcdNumber->setStyleSheet("*{background-color: #69EF5A;}");
    ui->lcdNumber_2->setStyleSheet("*{background-color: #69EF5A;}");
}

void HorizonSettings::setDefaultImg(QString name){
    this->default_img=name;


}

QPixmap HorizonSettings::getDefaultImgQPixmap(){
    QPixmap temp(this->default_img);
    return temp;
}

void HorizonSettings::setBinaryImgThreshold(int val){
    this->binaryImgThreshold=val;
}

int HorizonSettings::getBinaryImgThreshold(){
    return this->binaryImgThreshold;
}

QString HorizonSettings::getDefaultImg(){
    return this->default_img;
}

void HorizonSettings::loadDiskDefaultImg(){
    QFile mFile("/home/oras/workspace/apm_planner/Data.txt");

    if(mFile.open(QIODevice::ReadOnly)){
        QTextStream in(&mFile);

        this->default_img=in.readLine();

        mFile.close();
    }
}

void HorizonSettings::on_horizontalSlider_actionTriggered(int action)
{
    setBinaryImgThreshold(action);
    ui->lcdNumber->display(action);
}

HorizonSettings::~HorizonSettings()
{
    delete ui;
}


void HorizonSettings::on_horizontalSlider_sliderReleased()
{
    this->binaryImg=imread(getDefaultImg().toStdString());

    ImgRecoTool::transferToBinaryImage(this->binaryImgThreshold,this->binaryImg);

    updateImage(this->binaryImg);
}

void HorizonSettings::on_verticalSlider_sliderReleased(){
    drawHorizon(ui->verticalSlider->value());
}

void HorizonSettings::drawHorizon(int val){
    //Horizon mark color:
    const int R=112, G=238, B=28;

    Mat src=imread(getDefaultImg().toStdString());

    for(int i=0;i<src.cols;i++){
        if(src.rows-val<src.rows-2){
            src.at<Vec3b>(src.rows-val+1,i)[0]=R;
            src.at<Vec3b>(src.rows-val+1,i)[1]=G;
            src.at<Vec3b>(src.rows-val+1,i)[2]=B;
        }

        src.at<Vec3b>(src.rows-val,i)[0]=R;
        src.at<Vec3b>(src.rows-val,i)[1]=G;
        src.at<Vec3b>(src.rows-val,i)[2]=B;

        if(src.rows-val>2){
            src.at<Vec3b>(src.rows-val-1,i)[0]=R;
            src.at<Vec3b>(src.rows-val-1,i)[1]=G;
            src.at<Vec3b>(src.rows-val-1,i)[2]=B;
        }
    }

    updateImage(src);
}

void HorizonSettings::updateImage(Mat &src){
    QImage img=ImgRecoTool::cvMatToQImage(src);

    // Update demonstraion image view
    this->scene= new QGraphicsScene();
    QPixmap m(QPixmap::fromImage(img));
    scene->addPixmap(m.scaled(ui->graphicsView->width()-1,ui->graphicsView->height()-1));

    ui->graphicsView->setScene(this->scene);
}

void HorizonSettings::on_horizontalSlider_2_actionTriggered(int action){
    this->resolution=action;
    ui->lcdNumber_2->display(action);
}

void HorizonSettings::on_pushButton_clicked(){
    if(this->root!=NULL){
        Mat src=imread(getDefaultImg().toStdString());
        ImgRecoTool::markRGBflameAreas(src,this->root,ui->verticalSlider->value());

        updateImage(src);
    }
}

void HorizonSettings::on_pushButton_3_clicked()
{
    this->close();
}

void HorizonSettings::on_pushButton_2_clicked()
{
    this->close();
}

void HorizonSettings::createHorizon(){
    const int IMG_BORDER_POINTS=2;
    const int MAX_RESOLUTION=this->resolution;
    const int IMG_WIDTH_PIX_SIZE=this->defaultImg.cols;
    const int IMG_HEIGHT_PIX_SIZE=this->defaultImg.rows;
    const int IMG_PART_SIZE=IMG_WIDTH_PIX_SIZE/(MAX_RESOLUTION+1);
    int x=0;

    if(this->hRoot!=NULL)
        releaseHRoot();

    this->hRootSize=IMG_BORDER_POINTS+MAX_RESOLUTION;

    this->hRoot=new hNode[MAX_RESOLUTION+IMG_BORDER_POINTS];

    for(int i=0;i<=MAX_RESOLUTION+1;i++){
        if(i==MAX_RESOLUTION+1)
            this->hRoot[i].x=this->defaultImg.cols-1;
        else
           this->hRoot[i].x=x;

        //Find the Y of point:
        for(int y=0;y<IMG_HEIGHT_PIX_SIZE;y++){
             if(this->binaryImg.at<uchar>(y,x)==0){
                 this->hRoot[i].y=y;
                 break;
             }
        }

        x+=IMG_PART_SIZE;
    }
}

void HorizonSettings::drawHorizon(){
    const int IMG_BORDER_POINTS=2;
    const int NUM_OF_X_POINTS=this->resolution+IMG_BORDER_POINTS;
    int y,y0,y1,x0,x1=0;

    //Horizon mark color:
    const int R=112, G=238, B=28;

    Mat src=imread(getDefaultImg().toStdString());

    /*They are two diffrent methods to draw the horizon:
     * 1. Linear interpolation.
     * 2. Polynomial interpolation.
    */

    //1

    for(int i=0;i<NUM_OF_X_POINTS-1;i++){
        for(int x=this->hRoot[i].x;x<this->hRoot[i+1].x;x++){
            y0=this->hRoot[i].y;
            x0=this->hRoot[i].x;
            y1=this->hRoot[i+1].y;
            x1=this->hRoot[i+1].x;

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

    updateImage(src);
}

void HorizonSettings::on_horizontalSlider_2_sliderReleased(){
    createHorizon();
    drawHorizon();
}

void HorizonSettings::releaseHRoot(){
   delete[] this->hRoot;
    //delete hRoot;
}
