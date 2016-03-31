#ifndef DHUD_H
#define DHUD_H

#include <QDialog>
#include <string>
//#include <ImgRecoTool.h>
//#include <qgraphicsscene.h>

using namespace std;
//using namespace irt;


namespace Ui {
class DHUD;
}

class DHUD : public QDialog
{
    Q_OBJECT

public:
    explicit DHUD(QWidget *parent);
    ~DHUD();

public slots:
    /** @brief Update image from video stream */
    void streamImage(const QImage &image);
    /** @brief Video stream has been disconnected */
    void videoStreamDisconnected();
private:
    Ui::DHUD* ui;
    //QGraphicsScene* scene;
    //Horizon* horizon;
    //int binaryImgThreshold;
    //unsigned int resolution;
    //hNode *hRoot;
};

#endif // DHUD_H
