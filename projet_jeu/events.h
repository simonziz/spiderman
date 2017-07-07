// events.h

#ifndef EVENTS_H
#define EVENTS_H

#include <vector>

#include <irrlicht.h>
#include "character.h"
#include "ICursorControl.h"

using namespace irr;

class EventReceiver : public irr::IEventReceiver
{
public:
    EventReceiver(int len_x, int len_y);
    bool OnEvent(const irr::SEvent &event);
    void set_perso(character *perso);
    void mouseReset();


private:
    bool keyboard(const irr::SEvent &event);
    bool mouse(const irr::SEvent &event);


    character *perso;
    bool mouse_pressed;
    int  old_x, old_y;
    int Nx, Ny;


};

#endif
