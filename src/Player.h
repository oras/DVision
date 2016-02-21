/* This player project was used OpenCV libarary written by Stephen Nneji
 * on December 2012 All Rights!
 * Ajasted by Or Asnin 2/2016 to the APMplanner project.
 *
 * This player has been created in order to simulate capturing live video
 * from UAV camera in order to proccess video frmaes.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/opencv.hpp"
#include "opencv/cv.h"
#include <string>

using namespace cv;

class Player : public QThread
{    Q_OBJECT
 private:
    bool stop;
    QMutex mutex;
    QWaitCondition condition;
    Mat frame;
    int frameRate;
    VideoCapture capture;
    Mat RGBframe;
    QImage img;

 signals:
 //Signal to output frame to be displayed
      void processedImage(const QImage &image);
 protected:
     void run();
     void msleep(int ms);
 public:
    //Constructor
    Player(QObject *parent = 0);
    //Destructor
    ~Player();
    //Load a video from memory
    bool loadVideo(String filename);
    //Play the video
    void play();
    //Stop the video
    void Stop();
    //check if the player has been stopped
    bool isStopped() const;
};
#endif // VIDEOPLAYER_H
