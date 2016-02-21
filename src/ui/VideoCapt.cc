#include "VideoCapt.h"
#include "ui_VideoCapt.h"

#include <QString>
#include <QFileDialog>
#include <QVideoSurfaceFormat>
#include <QPixmap>
#include <gpl.h>
#include <QGraphicsVideoItem>
#include <QMediaPlaylist>
#include <QDataStream>
#include <QMessageBox>

VideoCapt::VideoCapt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VideoCapt)
    //,player(0, QMediaPlayer::VideoSurface)
{
    ui->setupUi(this);

    this->MyPlayer=new Player();

    QObject::connect(MyPlayer, SIGNAL(processedImage(QImage)),
                              this, SLOT(updatePlayerUI(QImage)));

    QString filename="/home/or/Videos/SampleVideo_720x480_1mb.mp4";

    if (!filename.isEmpty()){
        if (!MyPlayer->loadVideo(filename.toStdString()))
        {
            QMessageBox msgBox;
            msgBox.setText("The selected video could not be opened!");
            msgBox.exec();
        }
    }

    MyPlayer->play();

}
VideoCapt::~VideoCapt()
{
    delete ui;

}

void VideoCapt::on_buttonBox_accepted()
{
    this->close();
}

void VideoCapt::updatePlayerUI(QImage img)
{
    if (!img.isNull())
    {
        ui->label->setAlignment(Qt::AlignCenter);
        ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size(),
                                           Qt::KeepAspectRatio, Qt::FastTransformation));
    }
}
