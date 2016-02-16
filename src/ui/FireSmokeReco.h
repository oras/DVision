#ifndef FIRESMOKERECO_H
#define FIRESMOKERECO_H

#include <QDialog>
#include <string>
#include <ImgRecoTool.h>
#include <qgraphicsscene.h>
#include <QDockWidget>


using namespace std;
using namespace irt;

namespace Ui {
class FireSmokeReco;
}

class FireSmokeReco : public QDialog
{
    Q_OBJECT

    QString defaultImgPath;     // Store the name of the default image.
    int cannyImgThreshold;
    QGraphicsScene* scene;
    int contrast;
    Mat defaultImg;

public:
    explicit FireSmokeReco(QWidget *parent = 0);

    void loadDiskDefaultImg();

    void updateImage(Mat &src);

    ~FireSmokeReco();

private slots:
    void on_buttonBox_accepted();

    void on_horizontalSlider_sliderReleased();

    void on_horizontalSlider_actionTriggered(int action);

    void on_horizontalSlider_2_sliderReleased();

    void on_horizontalSlider_2_actionTriggered(int action);

    void on_pushButton_2_clicked();

private:
    Ui::FireSmokeReco *ui;
};

#endif // FIRESMOKERECO_H
