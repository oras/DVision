/*This video capture tool was written by Asnin Or 2016 all rights!
 */

#include "VideoCapt.h"
#include "ui_VideoCapt.h"

#include <QString>
#include <QFileDialog>
#include <QVideoSurfaceFormat>
#include <qpixmap.h>
#include <QGraphicsPixmapItem>
#include <gpl.h>
#include <QGraphicsVideoItem>
#include <QMediaPlaylist>
#include <QDataStream>
#include <QMessageBox>
#include <string>

VideoCapt::VideoCapt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VideoCapt)
    //,player(0, QMediaPlayer::VideoSurface)
{
    ui->setupUi(this);

    this->MyPlayer=new Player();

    QObject::connect(MyPlayer, SIGNAL(processedImage(QImage)),
                              this, SLOT(updatePlayerUI(QImage)));
    connect(ui->capButton,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_clicked()));

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
    delete MyPlayer;
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

void VideoCapt::on_pushButton_clicked(){
    QMessageBox msgBox;

    MyPlayer->Stop();
    QPixmap tempImg=ui->label->pixmap()->copy();

    msgBox.setText("Image has been captured.");
    msgBox.setInformativeText("Do you want to transfer the image into working board?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();

    Mat mat;

    switch (ret) {
      case QMessageBox::Save:
          // Save was clicked
          mat=ImgRecoTool::QImageToCvMat(tempImg.toImage());
          imwrite("/home/or/workspace/demo.jpg",mat);
          break;
      case QMessageBox::Discard:
          // Don't Save was clicked
          break;
      case QMessageBox::Cancel:
          // Cancel was clicked
          break;
      default:
          // should never be reached
          break;
    }
}
