#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>

class MyGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent=0);
    ~MyGLWidget();
protected:
    void initializeGL();

    void paintGL();

    void resizeGL(int width,int height);

    QSize minimumSizeHint() const;

    QSize sizeHint() const;

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);
private:
    int xRot;

    int yRot;

    int zRot;

    void draw();

    QPoint lastPos;
signals:
    // Signaling rotation from mouse movement.
    void xRotationChanged(int angle);

    void yRotationChanged(int angle);

    void zRotationChanged(int angle);
public slots:
    // Slots for xyz-rotation sliders.
    void setXRotation(int angle);

    void setYRotation(int angle);

    void setZRotation(int angle);
};

#endif // MYGLWIDGET_H
