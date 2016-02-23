#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H

#include <QDialog>
#include <QMediaPlayer>
#include <QGraphicsVideoItem>
#include <qgraphicsscene.h>
#include <QMovie>
#include <QtGui/QMovie>
#include <Player.h>
#include <ImgRecoTool.h>
#include <qgraphicsscene.h>

using namespace std;
using namespace irt;


namespace Ui {
class VideoCapt;
}

class VideoCapt : public QDialog
{
private:
    Ui::VideoCapt *ui;
    QMediaPlayer player;
    Player *MyPlayer;
    QGraphicsVideoItem *item;
    QGraphicsScene *scene;
    Q_OBJECT

public:
    explicit VideoCapt(QWidget *parent = 0);
    ~VideoCapt();

private slots:
    void on_buttonBox_accepted();

    //Display video frame in player UI
    void updatePlayerUI(QImage img);
    void on_pushButton_clicked();
};

#endif // VIDEOCAPTURE_H
