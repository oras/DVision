#include "WarnSound.h"

WarnSound::WarnSound()
{
    lock=new QSound("/home/or/workspace/apm_planner/Sound/lcokFiltered2.wav");
    lock->setLoops(QSound::Infinite);
    alarm=new QSound("/home/or/workspace/apm_planner/Sound/warn4.wav");
    alarm->setLoops(QSound::Infinite);
    attention=new QSound("/home/or/workspace/apm_planner/Sound/OUTERMK.WAV");
}

void WarnSound::run(){
    switch(num){
        case 0:
        {

            lock->play();
            alarm->play();

            break;
        }
        case 1:
        {
            lock->stop();
            alarm->stop();

            break;
        }
        case 2:
        {
            attention->play();
            break;
        }
    }
}

