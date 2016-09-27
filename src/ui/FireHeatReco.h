#ifndef FIREHEATRECO_H
#define FIREHEATRECO_H

#include <QDialog>

namespace Ui {
class FireHeatReco;
}

class FireHeatReco : public QDialog
{
    Q_OBJECT

public:
    explicit FireHeatReco(QWidget *parent = 0);
    ~FireHeatReco();

private:
    Ui::FireHeatReco *ui;
};

#endif // FIREHEATRECO_H
