#ifndef DHUD_H
#define DHUD_H

#include <QDialog>
#include <QObject>
#include <QPointer>
#include <QApplication>
#include <QGraphicsScene>

namespace Ui {
class DHUD;
}

class DHUD : public QDialog
{
    Q_OBJECT

public:
    explicit DHUD(QWidget *parent = 0);
    ~DHUD();

public slots:
    /** @brief Update image from video stream */
    void streamImage(const QImage &image);
private:
    Ui::DHUD *ui;
    QGraphicsScene *scene;
};

#endif // DHUD_H
