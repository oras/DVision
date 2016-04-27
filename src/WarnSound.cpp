#include "WarnSound.h"

WarnSound::WarnSound()
{
    lock=new QSound("/home/or/workspace/apm_planner/Sound/lcokFiltered2.wav");
    attention=new QSound("/home/or/workspace/apm_planner/Sound/OUTERMK.WAV");
}

void WarnSound::run(){
    switch(num){
        case 0:
        {
            lock->setLoops(QSound::Infinite);
            lock->play();

            break;
        }
        case 1:
        {
            lock->stop();
            break;
        }
        case 2:
        {
            attention->play();
            break;
        }
    }
}
