#include "VideoCapt.h"
#include "ui_VideoCapt.h"

#include <QString>
#include <QFileDialog>
#include <QVideoSurfaceFormat>
#include <QPixmap>

VideoCapt::VideoCapt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VideoCapt)
{
    ui->setupUi(this);


    this->scene=new QGraphicsScene(this);
    this->player=new QMediaPlayer(this);
    this->item = new QGraphicsVideoItem;


    ui->graphicsView->setScene(this->scene);
    this->player->setVideoOutput(this->item);
this->scene->setSceneRect(-1.25, -1.25, 2.5, 2.5);
        ui->graphicsView->scene()->addItem(this->item);
        this->player->setMedia(QUrl::fromLocalFile("/home/oras/Videos/SampleVideo_640x360_1mb.mp4"));
        this->player->play();

         ui->graphicsView->show();

}
VideoCapt::~VideoCapt()
{
    delete ui;
}

void VideoCapt::on_buttonBox_accepted()
{
    this->close();
}
