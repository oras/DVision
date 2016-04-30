#ifndef WARNSOUND_H
#define WARNSOUND_H

#include <QSound>
#include <QThread>


class WarnSound : public QThread
{
private:
    QSound* lock;
    QSound* attention;
    QSound* alarm;
    int num;            // Sound name to play
public:
    static const int LOCK=0;
    static const int STOP_LOCK=1;

    WarnSound();

    void run();

public slots:
    void playWarnSound(const int &num){
        this->num=num;
        run();
    }
};

#endif // WARNSOUND_H
