#ifndef DHUD_H
#define DHUD_H

#include <QWidget>
#include <MAVLinkSimulationLink.h>
#include <LinkInterface.h>
#include <QPointer>
#include <QApplication>
#include <ArduPilotMegaMAV.h>
#include <UASInterface.h>

namespace Ui {
class DHUD;
}

class DHUD : public QWidget
{
    Q_OBJECT

public:
    explicit DHUD(QPointer<MAVLinkSimulationLink> mavlink,QWidget *parent = 0);
    ~DHUD();

public slots:
    /** @brief Shows a critical message as popup or as widget */
    void showCriticalMessage(const QString& title, const QString& message);

    void connectedLink();

    /** @brief Set the widget into critical mode */
    void heartbeatTimeout(bool timeout, unsigned int ms);
private:
    void establishMAV();
    LinkInterface *link;
    Ui::DHUD *ui;
};

#endif // DHUD_H
