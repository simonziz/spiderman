#ifndef CHARACTER_H
#define CHARACTER_H

#include <irrlicht.h>

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;


class character
{
public:
    character();
    character(is::IAnimatedMeshSceneNode *newNode, is::ISceneCollisionManager *newCollisionManager, is::ISceneNodeAnimatorCollisionResponse *newAnim, iv::ITexture *newTextures, scene::ICameraSceneNode* newCamera,
              scene::IMeshSceneNode *newWeb, int len_x, int len_y);

    void action(const f32 speed, const f32 dt);
    void jump_press();
    void run_forward();
    void walk_back();
    void stop();
    void web();

    is::IAnimatedMeshSceneNode *node;
    bool KeyIsDown[KEY_KEY_CODES_COUNT];    // We use this array to store the current state of each key

    int position_camera_y; // position de la camera verticalement
private:
    void jump();
    void update_camera();
    bool IsKeyDown(EKEY_CODE keyCode) const;
    void handlekeyDown(const f32 speed, const f32 dt);
    void pendulum();
    void hook();

    is::ISceneCollisionManager *collision_manager;
    is::ISceneNodeAnimatorCollisionResponse *anim;
    iv::ITexture *texture;
    scene::ICameraSceneNode* camera;
    is::IMeshSceneNode *nodeWeb;

    float jump_speed; // integral de la vitesse pendant le saut (ralenti jusqu'a s'annuler, puis fait retomber ke personnage
    int Nx, Ny;
    bool isjumping;
    bool isWeb;
    bool isHook;

    ic::vector3df targetWeb;
    ic::vector3df intersection;
};

#endif // CHARACTER_H
