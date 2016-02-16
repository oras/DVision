#include "VideoCapt.h"
#include "ui_VideoCapt.h"

#include <QString>
#include <QFileDialog>
#include <QVideoSurfaceFormat>
#include <QPixmap>

VideoCapt::VideoCapt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VideoCapt)
    ,player(0, QMediaPlayer::VideoSurface)
{
    ui->setupUi(this);

    QVideoWidget *videoWidget = new QVideoWidget;

    //this->scene=new QGraphicsScene(this);
    //this->item = new QGraphicsVideoItem;


    ui->graphicsView->setViewport(videoWidget);
    player.setVideoOutput(videoWidget);


        player.setMedia(QUrl::fromLocalFile("/home/or/Videos/SampleVideo_720x480_1mb.mp4"));
        player.play();

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
