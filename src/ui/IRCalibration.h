/*Thise Class written by Asnin Or 2016 All rights.
*/
#ifndef IRCALIBRATION_H
#define IRCALIBRATION_H

#include <QWidget>
#include <QGraphicsScene>>
#include <ImgRecoTool.h>
#include <QMouseEvent>
#include <QGraphicsSceneMoveEvent>
#include <QPainter>

using namespace cv;


class CalScene : public QGraphicsScene
{
    Q_OBJECT

public:
    // Text label color (light green).
    const int R=112, G=238, B=28;
    CalScene(QString format,QObject* parent=0);
    QPainter painter;
    QPen pen;
    QGraphicsTextItem *text;
    QColor myColor;
    QString format;
    QObject *par;

private:
    void drawPoint(QPointF point);
public slots:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);

};

namespace Ui {
class IRCalibration;
}

class IRCalibration : public QWidget
{
    Q_OBJECT

public:
    explicit IRCalibration(QWidget *parent = 0);
    ~IRCalibration();

    void addNewRGBPoint(QPointF p);

    void addNewIRPoint(QPointF p);

    int getRGBPointsNum();

    int getIRPointsNum();
private:
    Ui::IRCalibration *ui;
    const int MAX_POINTS=10;
    CalScene *scene,*scene2;
    QImage img,imgIR;
    QPointF *IRPoints,*RGBPoints;
    double ratio;                    // The image ratio differnece between IR & RGB.
    cv::Point pDiff;                 // The point differnce between IR & RGB.
    QImage frameRGB,FrameIR;
    bool calibMode,testMode;
    int RGBCounter,IRCounter;
    int diffX,diffY;
    cv::Size *size;
    Mat dst,src,colorMap;
    bool mc_pauseRequired;
    QMutex mc_continue;
    QWaitCondition mc_pauseManager;
    cv::Point *po;

    void drawHotZones(Mat &src, Mat &dst);

    bool isFlameTexture(int r, int g, int b);

    void loadDiskDefaultImg();
public slots:
    /** @brief Update image from video stream */
    void streamImage(const QImage &image);
     /** @brief Update image from video IR stream */
    void streamImageIR(const QImage &image);
private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void resetCalibration();

    void on_pushButton_2_clicked();
signals:
     void updateImageIRDiff(const cv::Point p);
};

#endif // IRCALIBRATION_H
