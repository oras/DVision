#include <string>
#include <Event.h>

using namespace std;

class EventFactory{

public:

    static Event *newEvent(const string &description){
        if(description=="Flame")
            return new Flame();
        else if(description=="Smoke")
            return new Smoke();
        else if(description=="Heat")
            return new Heat();

        return NULL;
    }
};
