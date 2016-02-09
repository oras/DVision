#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H

#include <QDialog>
#include <QMediaPlayer>
#include <QGraphicsVideoItem>
#include <qgraphicsscene.h>
#include <QMovie>

namespace Ui {
class VideoCapt;
}

class VideoCapt : public QDialog
{
    QMediaPlayer *player;
    QGraphicsVideoItem *item;
    QGraphicsScene *scene;
    Q_OBJECT

public:
    explicit VideoCapt(QWidget *parent = 0);
    ~VideoCapt();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::VideoCapt *ui;
};

#endif // VIDEOCAPTURE_H
