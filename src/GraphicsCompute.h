#ifndef GRAPHICSDETECTCOMPUTE_H
#define GRAPHICSDETECTCOMPUTE_H

#include <QThread>
#include <QObject>
#include <ImgRecoTool.h>
#include <qimage.h>
#include <QMutex>
#include <DetectionHUD.h>


using namespace irt;
using namespace cv;


class GraphicsCompute : public QThread
{
    Q_OBJECT

private:
    int binaryImgThreshold;
    unsigned int resolution;
    cv::Point *hRoot;
    QImage *img;
    clock_t thisTime, lastTime;
    QMutex mutex;
    QMutex mutexUpdateImg;
    Node* root;
public:
    GraphicsCompute(QObject *parent=0);

    void run();



public slots:
    void startCompute(){
        run();
    }

signals:
    void imageReady(const QImage &img);

    void playWarningSound();

    void stopWarningSound();
};

#endif // GRAPHICSDETECTCOMPUTE_H
