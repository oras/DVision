/*
 *
*/

#ifndef VSTREAMSIMULATOR_H
#define VSTREAMSIMULATOR_H

#include <QObject>
#include <QMediaPlayer>
#include <QGraphicsVideoItem>
#include <qgraphicsscene.h>
#include <QMovie>
#include <QtGui/QMovie>
#include <Player.h>
#include <ImgRecoTool.h>
#include <qgraphicsscene.h>
#include <UASInterface.h>
#include <LinkInterface.h>

using namespace cv;

class VStreamSimulator : public QThread{
    Q_OBJECT
private:
    bool con;
    VideoCapture capture;
    QMutex mutex;
    QWaitCondition condition;
    Mat frame;
    int frameRate;
    Mat RGBframe;
    QImage img;
    UASInterface* uasMain;
    bool go;

 public:
    static VStreamSimulator* instance();
    ~VStreamSimulator();
 public slots:
    /** @brief Set the video stream into disconnected mode */
    void heartbeatTimeout(bool timeout, unsigned int ms);
    /** @brief A new system was created */
    void UASCreated(UASInterface* UAS);
 signals:
    // Signal to output frame to be displayed
     void streamImage(const QImage &image);
 protected:
    VStreamSimulator(QObject *parent=0);
    void streamVideo();
    bool openVideoStream(String fileName);
    void msleep(int ms);
    LinkInterface* link;
    void run();
};

#endif // VSTREAMSIMULATOR_H
