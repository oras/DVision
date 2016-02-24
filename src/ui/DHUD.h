#ifndef DHUD_H
#define DHUD_H

#include <QDialog>
#include <MAVLinkSimulationLink.h>
#include <QPointer>

namespace Ui {
class DHUD;
}

class DHUD : public QDialog
{
    Q_OBJECT

public:
    explicit DHUD(QPointer<MAVLinkSimulationLink> mavlink,QWidget *parent = 0);
    ~DHUD();

private:
    void establishMAV();
    Ui::DHUD *ui;
};

#endif // DHUD_H
