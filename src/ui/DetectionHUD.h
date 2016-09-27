#ifndef DETECTION_HUD
#define DETECTION_HUD


#include <QtWidgets/QWidget>
#include <ImgRecoTool.h>
#include <qgraphicsview.h>
#include <VStreamSimulator.h>
#include <CameraCapture.h>
#include <time.h>
#include <QThread>
#include <math.h>
#include <WarnSound.h>

using namespace std;
using namespace irt;

namespace Ui {
class DetectionHUD;
}

class DetectionHUD : public QWidget
{
    Q_OBJECT

public:
    explicit DetectionHUD(QWidget *parent = 0);
    ~DetectionHUD();
    QImage* getImage();
    QImage img;
    Mat matImg;
private:
    const int NUM_SECONDS=pow(10,6);
    Ui::DetectionHUD *ui;
    QGraphicsScene *scene;
    int binaryImgThreshold;
    unsigned int resolution;
    cv::Point *hRoot;
    clock_t thisTime, lastTime;
    QThread thread;
    WarnSound* warn;
    vector<Circ> vCircle;


    // Set the detection layers
    void setDHUD(QImage &img);
public slots:
    /** @brief Update image from video stream */
    void streamImage(const QImage &image);
    /** @brief Video stream has been disconnected */
    void videoStreamDisconnected();
    /** @brief image graphic information ready for update */
    void imgInfoReady();
signals:
    void startCompute();

};

#endif // DETECTION_HUD
