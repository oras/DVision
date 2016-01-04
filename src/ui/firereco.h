#ifndef FIRERECO_H
#define FIRERECO_H

#include <QWidget>

namespace Ui {
class FireReco;
}

class FireReco : public QWidget
{
    Q_OBJECT

public:
    explicit FireReco(QWidget *parent = 0);
    ~FireReco();

private:
    Ui::FireReco *ui;
};

#endif // FIRERECO_H
