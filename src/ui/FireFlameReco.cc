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
    // Read default img from disk.
    loadDiskDefaultImg();

    ui->setupUi(this);
    ui->lcdNumber->display(1);
    ui->horizontalSlider->setRange(1,255);
    ui->horizontalSlider->setSingleStep(1);

    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(on_horizontalSlider_actionTriggered(int)));

    this->scene = new QGraphicsScene();
    QPixmap m(this->default_img);
    scene->addPixmap(m.scaled(ui->graphicsView->width()-1,ui->graphicsView->height()-1));

    ui->graphicsView->setScene(this->scene);

    ui->lcdNumber->setStyleSheet("*{background-color: #69EF5A;}");
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


void FireFlameReco::on_horizontalSlider_sliderReleased()
{
    Mat src=imread(getDefaultImg().toStdString());

    Node *root=ImgRecoTool::getSaliencyMap(src,getScanMethod(),getBinaryImgThreshold());

    ImgRecoTool::drawSaliencyDatas(root,src);

    QImage img=ImgRecoTool::cvMatToQImage(src);


    // Update demonstraion image view
    this->scene= new QGraphicsScene();
    QPixmap m(QPixmap::fromImage(img));
    scene->addPixmap(m.scaled(ui->graphicsView->width()-1,ui->graphicsView->height()-1));

    ui->graphicsView->setScene(this->scene);

    freeNodeLinkedList(root);

    delete root;
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

void FireFlameReco::freeNodeLinkedList(Node *root){
    Node *temp1 = root->next;

    while(temp1!=NULL) // as I am considering tail->next = NULL
    {
        root->next = temp1->next;
        temp1->next = NULL;
        free(temp1);
        temp1 = root->next;
    }
}
