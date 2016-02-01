/*=============================================================================
 * EVENTS DITECTION USING THE GROUNDCONTROL PROJECT
 *
 * This code is part of the JCE Software & Computer Enginnering Department
 * research project, Written by Asnin Or All rights 2015-2016.
 *
 * www.jce.ac.il
===============================================================================*/

#include "configuration.h"
#include "FireFlameReco.h"
#include "ui_FireFlameReco.h"
#include <QMessageBox>
#include <qpixmap.h>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include <QTextStream>


FireFlameReco::FireFlameReco(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FireFlameReco)
{
    this->root=NULL;

    // Read default img from disk.
    loadDiskDefaultImg();

    ui->setupUi(this);
    ui->lcdNumber->display(1);
    ui->horizontalSlider->setRange(1,255);
    ui->horizontalSlider->setSingleStep(1);
    ui->rbDiagonal->setChecked(true);

    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(on_horizontalSlider_actionTriggered(int)));
    connect(ui->horizontalSlider_2,SIGNAL(valueChanged(int)),this,SLOT(on_horizontalSlider_2_actionTriggered(int)));
    connect(ui->horizontalSlider_3,SIGNAL(valueChanged(int)),this,SLOT(on_horizontalSlider_3_actionTriggered(int)));
    connect(ui->pushButton_4,SIGNAL(valueChanged(int)),this,SLOT(on_pushButton_4_clicked()));

    this->scene = new QGraphicsScene();
    QPixmap m(this->default_img);
    scene->addPixmap(m.scaled(ui->graphicsView->width()-1,ui->graphicsView->height()-1));

    ui->graphicsView->setScene(this->scene);

    ui->lcdNumber->setStyleSheet("*{background-color: #69EF5A;}");
    ui->lcdNumber_2->setStyleSheet("*{background-color: #69EF5A;}");
    ui->lcdNumber_3->setStyleSheet("*{background-color: #69EF5A;}");
}

void FireFlameReco::setDefaultImg(QString name){
    this->default_img=name;


}

QPixmap FireFlameReco::getDefaultImgQPixmap(){
    QPixmap temp(this->default_img);
    return temp;
}

void FireFlameReco::setBinaryImgThreshold(int val){
    this->binaryImgThreshold=val;
}

int FireFlameReco::getBinaryImgThreshold(){
    return this->binaryImgThreshold;
}

QString FireFlameReco::getDefaultImg(){
    return this->default_img;
}

void FireFlameReco::on_horizontalSlider_actionTriggered(int action)
{
    setBinaryImgThreshold(action);
    ui->lcdNumber->display(action);
}

bool FireFlameReco::getScanMethod(){
    if(ui->rbDiagonal->isChecked())
        return true;

    return false;
}
FireFlameReco::~FireFlameReco()
{
    delete ui;
}


void FireFlameReco::on_horizontalSlider_sliderReleased(){
    if(this->root!=NULL){
        freeNodeLinkedList(this->root);
    }

    Mat src=imread(getDefaultImg().toStdString());

    this->root=ImgRecoTool::getSaliencyMap(src,getScanMethod(),getBinaryImgThreshold());

    ImgRecoTool::drawSaliencyDatas(this->root,src);

    QImage img=ImgRecoTool::cvMatToQImage(src);


    // Update demonstraion image view
    this->scene= new QGraphicsScene();
    QPixmap m(QPixmap::fromImage(img));
    scene->addPixmap(m.scaled(ui->graphicsView->width()-1,ui->graphicsView->height()-1));

    ui->graphicsView->setScene(this->scene);
}

void FireFlameReco::updateDemoImage(){

}

void FireFlameReco::loadDiskDefaultImg(){
    QFile mFile("/home/oras/workspace/apm_planner/Data.txt");

    if(mFile.open(QIODevice::ReadOnly)){
        QTextStream in(&mFile);

        this->default_img=in.readLine();

        mFile.close();
    }
}

void FireFlameReco::on_pushButton_2_clicked(){
    QString imgFileName=QFileDialog::getOpenFileName(this,tr("Get an image file"),"/home");

    setDefaultImg(imgFileName);

    // Update demonstraion image view
    this->scene= new QGraphicsScene();
    QPixmap m(getDefaultImg());
    scene->addPixmap(m.scaled(ui->graphicsView->width()-1,ui->graphicsView->height()-1));

    ui->graphicsView->setScene(this->scene);
}

void FireFlameReco::on_pushButton_3_clicked(){
    // Save image name + path to disk.
    QFile mFile("/home/oras/workspace/apm_planner/Data.txt");

    if(mFile.open(QIODevice::ReadWrite | QIODevice::Truncate)){
        QTextStream stream(&mFile);
        stream<<this->default_img;

        mFile.close();

        QMessageBox::information(
                    this,
                    tr("APMPlanner 2.0"),
                    tr("The settings has been saved"));
    }
}

void FireFlameReco::on_pushButton_5_clicked(){
   this->close();
}

void FireFlameReco::freeNodeLinkedList(Node *p){
    Node* toDelete = p;
    while(toDelete != NULL) {
        Node* next = toDelete->next;
        delete toDelete;
        toDelete = next;
    }
}

void FireFlameReco::on_pushButton_4_clicked(){
    Mat src=imread(getDefaultImg().toStdString());
    ImgRecoTool::markRGBflameAreas(src,this->root,src.rows);

    updateImage(src);
}

void FireFlameReco::updateImage(Mat &src){
    QImage img=ImgRecoTool::cvMatToQImage(src);

    // Update demonstraion image view
    this->scene= new QGraphicsScene();
    QPixmap m(QPixmap::fromImage(img));
    scene->addPixmap(m.scaled(ui->graphicsView->width()-1,ui->graphicsView->height()-1));

    ui->graphicsView->setScene(this->scene);
}

void FireFlameReco::on_horizontalSlider_2_actionTriggered(int action)
{
     ui->lcdNumber_2->display(action);
}

void FireFlameReco::on_horizontalSlider_3_actionTriggered(int action)
{
    ui->lcdNumber_3->display(action);
}

Node *FireFlameReco::getSaliencyMapRoot(){
    return this->root;
}
