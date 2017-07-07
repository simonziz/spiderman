// events.cpp

#include <irrlicht.h>

#include "events.h"
#include "iostream"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

/**************************************************************************\
 * EventReceiver::EventReceiver                                           *
\**************************************************************************/
EventReceiver::EventReceiver(int len_x, int len_y)
  : perso(nullptr), mouse_pressed(false), Nx(len_x), Ny(len_y)
{
    mouseReset();
}

/*------------------------------------------------------------------------*\
 * EventReceiver::keyboard                                                *
\*------------------------------------------------------------------------*/
bool EventReceiver::keyboard(const SEvent &event)
{
    // push button
  if (event.KeyInput.PressedDown)
  {
    switch (event.KeyInput.Key)
    {
      case KEY_ESCAPE:
        exit(0);
      case KEY_KEY_Z: // Avance
        perso->run_forward();
        break;
      case KEY_KEY_S: // Avance
        perso->walk_back();
        break;
      case KEY_SPACE: // saute
        perso->jump_press();
        break;
      default:;
    }
  }
  // release button
  else
  {
      switch (event.KeyInput.Key)
      {
      case KEY_KEY_Z: // Tourne à gauche
          perso->stop();
        break;
      case KEY_KEY_S: // Tourne à gauche
          perso->stop();
        break;
      default:;
      }
  }

  // Remember whether each key is down or up
  if (event.EventType == irr::EET_KEY_INPUT_EVENT)
      perso->KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;


  return false;
}


/*------------------------------------------------------------------------*\
 * EventReceiver::mouse                                                   *
\*------------------------------------------------------------------------*/
bool EventReceiver::mouse(const SEvent &event)
{
  switch(event.MouseInput.Event)
  {
  case EMIE_LMOUSE_PRESSED_DOWN:
    break;
    case EMIE_LMOUSE_LEFT_UP:
      break;
  case EMIE_RMOUSE_PRESSED_DOWN:
      perso->web();
    break;
    case EMIE_MOUSE_MOVED:
    {
        ic::vector3df rotation = perso->node->getRotation();
        int mvt = event.MouseInput.X - old_x;

        if(abs(mvt) < 3)
            rotation.Y += (event.MouseInput.X - old_x);
        else if(abs(mvt) < 20)
            rotation.Y += (event.MouseInput.X - old_x)/4;
        else
            rotation.Y += (event.MouseInput.X - old_x)/10;

        perso->node->setRotation(rotation);

        perso->position_camera_y -= (event.MouseInput.Y - old_y)/2;
        if(perso->position_camera_y > 0)
        {
            perso->position_camera_y = std::min(perso->position_camera_y,1000);
        }
        else
        {
            perso->position_camera_y = std::max(perso->position_camera_y,-1000);
        }


    }
      break;
    default:
      ;
  }

  return false;
}


void EventReceiver::mouseReset()
{
    old_x = Nx/2;
    old_y = Ny/4;
}

/**************************************************************************\
 * EventReceiver::OnEvent                                                 *
\**************************************************************************/
bool EventReceiver::OnEvent(const SEvent &event)
{
  if (!perso) return false;
  switch (event.EventType)
  {
    case EET_KEY_INPUT_EVENT:
      return keyboard(event);
    case EET_MOUSE_INPUT_EVENT:
      return mouse(event);
    default:;
  }

  return false;
}

/**************************************************************************\
 * EventReceiver::set_perso                                                *
\**************************************************************************/
void EventReceiver::set_perso(character *p)
{
  perso = p;
}
