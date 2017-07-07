// main.cpp

#include <irrlicht.h>
#include <iostream>

#include "events.h"
#include "character.h"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;


int main()
{
    //int Nx = 1280, Ny = 960;
    int Nx = 960, Ny = 640;
  // Le gestionnaire d'événements
  EventReceiver receiver(Nx, Ny);
  std::vector<iv::ITexture*> textures;

  // Création de la fenêtre et du système de rendu.
  IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
                                        ic::dimension2d<u32>(Nx, Ny),
                                        16, false, false, false, &receiver);

  iv::IVideoDriver  *driver = device->getVideoDriver();
  is::ISceneManager *smgr = device->getSceneManager();

  // Ajout de l'archive qui contient entre autres un niveau complet
  device->getFileSystem()->addFileArchive("data/map-20kdm2.pk3");
  gui::SCursorSprite iconMouse;
  device->getCursorControl()->changeIcon(irr::gui::ECI_CROSS, iconMouse);

  //cursor.cur

  // On charge un bsp (un niveau) en particulier :
  is::IAnimatedMesh *mesh_decor = smgr->getMesh("20kdm2.bsp");
  is::IMeshSceneNode *node = smgr->addOctreeSceneNode(mesh_decor->getMesh(0));
  // Translation pour que nos personnages soient dans le décor
  node->setPosition(core::vector3df(-1300,-104,-1249));

  // Chargement de notre personnage
  is::IAnimatedMesh *mesh = smgr->getMesh("data/tris.md2");


  // node tampon, car on en a besoin pour creer la camera avant le personnage
  is::IAnimatedMeshSceneNode *tmp = smgr->addAnimatedMeshSceneNode(mesh);


  // Création du triangle selector
  scene::ITriangleSelector *selector;
  selector = smgr->createOctreeTriangleSelector(node->getMesh(), node);
  node->setTriangleSelector(selector);


  ic::vector3df position = tmp->getPosition();
  ic::vector3df rotation = tmp->getRotation();
  position.X += -50 * cos(rotation.Y * M_PI / 180.0);
  position.Z += 50 * sin(rotation.Y * M_PI / 180.0);
  position.Y += 40;


  ic::vector3df target = tmp->getPosition();
  rotation = tmp->getRotation();
  target.X += 30 * cos(rotation.Y * M_PI / 180.0);
  target.Z += -30 * sin(rotation.Y * M_PI / 180.0);


  // Création de la caméra
  scene::ICameraSceneNode* camera =
  smgr->addCameraSceneNode(tmp,
                                position,         // Vitesse de rotation
                                target,          // Vitesse de déplacement
                                -1,
                                true
                                );          // Vitesse saut

  /// creation de la toile
  is::IMesh *mesh_web = smgr->getGeometryCreator()->createCylinderMesh(0.2f, 1.0f, 4, 0xffffff);
  // node tampon, car on en a besoin pour creer la camera avant le personnage
  is::IMeshSceneNode *node_web = smgr->addMeshSceneNode(mesh_web);
  node_web->setMaterialFlag(irr::video::EMF_LIGHTING, false);

  /// A mettre dans la classe character
  is::ISceneNodeAnimatorCollisionResponse *anim_perso1;
  anim_perso1 = smgr->createCollisionResponseAnimator(selector,
                                               tmp,  // Le noeud que l'on veut gérer
                                               ic::vector3df(20, 20, 20), // "rayons" de la caméra
                                               ic::vector3df(0, -10, 0),  // gravité
                                               ic::vector3df(0, 5, 0));  // décalage du centre


  character hero( tmp, smgr->getSceneCollisionManager(),anim_perso1, driver->getTexture("data/blue_texture.pcx"), camera, node_web, Nx, Ny);
  receiver.set_perso(&hero);


  //perso1->addAnimator(anim_perso1);
  hero.node->addAnimator(anim_perso1);




  // In order to do framerate independent movement, we have to know
  // how long it was since the last frame
  u32 then = device->getTimer()->getTime();

  // This is the movemen speed in units per second.
  const f32 MOVEMENT_SPEED = 150.f;

  while(device->run())
  {
    // Work out a frame delta time.
    const u32 now = device->getTimer()->getTime();
    const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
    then = now;

    hero.action(MOVEMENT_SPEED, frameDeltaTime);

    device->getCursorControl()->setPosition(0.5f,0.25f);

    driver->beginScene(true, true, iv::SColor(0,50,100,255));

    // Dessin de la scène :
    smgr->drawAll();
    // 
    driver->endScene();
  }
  device->drop();

  return 0;
}
