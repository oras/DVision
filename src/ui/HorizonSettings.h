#ifndef HORIZONSETTINGS_H
#define HORIZONSETTINGS_H

#include <QWidget>

namespace Ui {
class HorizonSettings;
}

class HorizonSettings : public QWidget
{
    Q_OBJECT

public:
    explicit HorizonSettings(QWidget *parent = 0);
    ~HorizonSettings();

private:
    Ui::HorizonSettings *ui;
};

#endif // HORIZONSETTINGS_H
