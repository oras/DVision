#include <QtWidgets>
#include "Analyze3D.h"
#include "ui_Analyze3D.h"
#include "MyGLWidget.h"

Analyze3D::Analyze3D(QWidget *parent) : QWidget(parent),
    ui(new Ui::Analyze3D)
{
    ui->setupUi(this);

    connect(ui->myGLWidget, SIGNAL(xRotationChanged(int)),ui->xRotSlider,SLOT(setValue(int)));
    connect(ui->myGLWidget, SIGNAL(yRotationChanged(int)),ui->yRotSlider,SLOT(setValue(int)));
    connect(ui->myGLWidget, SIGNAL(zRotationChanged(int)),ui->zRotSlider,SLOT(setValue(int)));
}

Analyze3D::~Analyze3D(){
    delete ui;
}

void Analyze3D::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(event);
}
