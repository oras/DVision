#ifndef HORIZONSETTINGS_H
#define HORIZONSETTINGS_H

#include <QWidget>
#include <QDialog>
#include <string>
#include <qgraphicsscene.h>
#include <ImgRecoTool.h>

using namespace std;
using namespace irt;

namespace Ui {



class HorizonSettings;
}

class HorizonSettings : public QDialog
{
    Q_OBJECT

private:
    QString default_img;     // Store the name of the default image.
    int binaryImgThreshold;
    unsigned int resolution;
    Node *root;
    hNode *hRoot;
    int hRootSize;
    QGraphicsScene* scene;
    Mat defaultImg;
    Mat binaryImg;
public:
    HorizonSettings(QWidget *parent,Node *head);
    ~HorizonSettings();

     void setDefaultImg(QString name);

     QPixmap getDefaultImgQPixmap();

     QString getDefaultImg();

     void setBinaryImgThreshold(int val);

     int getBinaryImgThreshold();

     string getPath();

     void createHorizon(hNode *root, int &rootSize);

     hNode *getHorizon();

     void drawHorizon();
private slots:

     void on_horizontalSlider_actionTriggered(int action);

     void on_horizontalSlider_sliderReleased();

     void on_verticalSlider_sliderReleased();

     void on_horizontalSlider_2_actionTriggered(int action);

     void on_pushButton_clicked();

     void on_pushButton_3_clicked();

     void on_pushButton_2_clicked();

     void on_horizontalSlider_2_sliderReleased();

private:
    void loadDiskDefaultImg();

    void drawHorizon(int val);

    void updateImage(Mat &src);

    void releaseHRoot(hNode *root);

    Ui::HorizonSettings *ui;
};

#endif // HORIZONSETTINGS_H
