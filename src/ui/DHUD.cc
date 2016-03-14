#include "DHUD.h"
#include "ui_DHUD.h"
#include "configuration.h"
#include "VStreamSimulator.h"


DHUD::DHUD(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DHUD)
{
    ui->setupUi(this);
    ui->label->setStyleSheet("qproperty-alignment: 'AlignCenter';font-size: 40px;");
    this->scene=new QGraphicsScene();
    ui->graphicsView->setScene(this->scene);
    ui->graphicsView->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->graphicsView->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->graphicsView->show();

    connect(VStreamSimulator::instance(),SIGNAL(streamImage(const QImage&)), this, SLOT(streamImage(const QImage&)));
}

void DHUD::streamImage(const QImage &image)
{
    ui->label->hide();

    if (!image.isNull())
    {
        // Refresh scene & update next video frame.
        scene->clear();
        scene->addPixmap(QPixmap::fromImage(image).scaled(ui->graphicsView->size(),
                                                          Qt::KeepAspectRatio, Qt::FastTransformation));

    }
    else
        ui->label->show();
}

DHUD::~DHUD(){
    delete ui;
}
