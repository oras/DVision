#ifndef ANALYZE3D_H
#define ANALYZE3D_H

#include <QWidget>
#include <QSlider>

namespace Ui{

class Analyze3D;
}


class Analyze3D : public QWidget{
    Q_OBJECT

public:
    explicit Analyze3D(QWidget *parent = 0);
    ~Analyze3D();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::Analyze3D *ui;
};

#endif // ANALYZE3D_H
