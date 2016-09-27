#ifndef IR_H
#define IR_H

#include <QWidget>
#include <qgraphicsview.h>
#include <ImgRecoTool.h>

using namespace cv;
using namespace irt;

namespace Ui {
class IR;
}

class IR : public QWidget
{
    Q_OBJECT

public:
    explicit IR(QWidget *parent = 0);
    ~IR();

private:
    QImage img;
    Ui::IR *ui;
    QGraphicsScene *scene;
    Mat imGray,colorMap;
public slots:
    /** @brief Update image from video stream */
    void streamImageIR(const QImage &image);
    /** @brief Video stream has been disconnected */
    void videoStreamDisconnected();
};

#endif // IR_H
