#ifndef DETECTION_HUD
#define DETECTION_HUD

#include <GraphicsCompute.h>
#include <QtWidgets/QWidget>
#include <ImgRecoTool.h>
#include <qgraphicsview.h>
#include <VStreamSimulator.h>
#include <time.h>
#include <QThread>
#include <math.h>

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
private:
    const int NUM_SECONDS=pow(10,6);
    Ui::DetectionHUD *ui;
    QGraphicsScene *scene;
    int binaryImgThreshold;
    unsigned int resolution;
    hNode *hRoot;
    clock_t thisTime, lastTime;
    QThread thread;


    // Set the detection layers
    void setDHUD(QImage &img);
public slots:
    /** @brief Update image from video stream */
    void streamImage(const QImage &image);
    /** @brief Video stream has been disconnected */
    void videoStreamDisconnected();
    /** @brief Video stream frame ready for update */
    void imageReady(const QImage &img);
signals:
    void startCompute();

};

#endif // DETECTION_HUD
