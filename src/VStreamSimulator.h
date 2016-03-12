/*
 *
*/

#ifndef VSTREAMSIMULATOR_H
#define VSTREAMSIMULATOR_H

#include <QDialog>
#include <QMediaPlayer>
#include <QGraphicsVideoItem>
#include <qgraphicsscene.h>
#include <QMovie>
#include <QtGui/QMovie>
#include <Player.h>
#include <ImgRecoTool.h>
#include <qgraphicsscene.h>
#include <UASInterface.h>

using namespace cv;

class VStreamSimulator : QThread{
    Q_OBJECT
private:
    bool connect;
    VideoCapture capture;
    QMutex mutex;
    QWaitCondition condition;
    Mat frame;
    int frameRate;
    Mat RGBframe;
    QImage img;

 public:
    static VStreamSimulator* instance();
    ~VStreamSimulator();
 public slots:
    /** @brief Set the video stream into disconnected mode */
    void heartbeatTimeout(bool timeout, unsigned int ms);
    /** @brief The stram is connected **/
    void connected();
 signals:
    // Signal to output frame to be displayed
     void processedImage(const QImage &image);
 protected:
    VStreamSimulator(QObject *parent=0);
    void streamVideo();
    bool openVideoStream(String fileName);
    void msleep(int ms);
};

#endif // VSTREAMSIMULATOR_H
