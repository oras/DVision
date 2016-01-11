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
    QGraphicsScene* scene;
public:
    explicit FireFlameReco(QWidget *parent = 0);
    ~FireFlameReco();

    void setDefaultImg(QString name);

    QPixmap getDefaultImgQPixmap();

    QString getDefaultImg();

    void setBinaryImgThreshold(int val);

    int getBinaryImgThreshold();

    string getPath();

    bool getScanMethod();

private slots:
    void on_horizontalSlider_actionTriggered(int action);

    void on_horizontalSlider_sliderReleased();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::FireFlameReco *ui;

    void loadDiskDefaultImg();

    void updateDemoImage();

    void freeNodeLinkedList(Node *root);
};
#endif // FIREFLAMERECO_H
