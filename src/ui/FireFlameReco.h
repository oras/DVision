/*=============================================================================
 * EVENTS DITECTION USING THE GROUNDCONTROL PROJECT
 *
 * This code is part of the JCE Software & Computer Enginnering Department
 * research project, Written by Asnin Or All rights 2015-2016.
 *
 * www.jce.ac.il
===============================================================================*/

#ifndef FIRFLAMEERECO_H
#define FIREFLAMERECO_H

#include <QDialog>
#include <string>
#include <ImgRecoTool.h>
#include <qgraphicsscene.h>

using namespace std;
using namespace irt;

namespace Ui {
    class FireFlameReco;
}

class FireFlameReco : public QDialog
{
    Q_OBJECT

private:
    QString default_img;     // Store the name of the default image.
    int binaryImgThreshold;
    float resolution;
    bool scanMethode;
public:
    irt::Node *root;
    QGraphicsScene* scene;
    explicit FireFlameReco(QWidget *parent);
    ~FireFlameReco();

    void setDefaultImg(QString name);

    QPixmap getDefaultImgQPixmap();

    QString getDefaultImg();

    void setBinaryImgThreshold(int val);

    int getBinaryImgThreshold();

    string getPath();

    bool getScanMethod();

    irt::Node *getSaliencyMapRoot();
private slots:
    void on_horizontalSlider_actionTriggered(int action);

    void on_horizontalSlider_sliderReleased();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_horizontalSlider_2_actionTriggered(int action);

    void on_horizontalSlider_3_actionTriggered(int action);
private:
    Ui::FireFlameReco *ui;

    void loadDiskDefaultImg();

    void updateDemoImage();

    void freeNodeLinkedList(irt::Node *root);

    void updateImage(Mat &src);
};
#endif // FIREFLAMERECO_H
