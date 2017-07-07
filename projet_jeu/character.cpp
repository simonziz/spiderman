#include "character.h"
#include "quaternion.h"

#include "iostream"
character::character()
{
}

character::character(is::IAnimatedMeshSceneNode *newNode , is:: ISceneCollisionManager *newCollisionManager, scene::ISceneNodeAnimatorCollisionResponse *newAnim, iv::ITexture *newTexture,
                     scene::ICameraSceneNode *newCamera, is::IMeshSceneNode *newWeb, int len_x, int len_y)
    :node(newNode), position_camera_y(0), collision_manager(newCollisionManager), anim(newAnim), texture(newTexture),
      camera(newCamera), nodeWeb(newWeb), jump_speed(0), Nx(len_x), Ny(len_y), isjumping(false), isWeb(false), isHook(false),targetWeb(0,0,0),intersection(0,0,0)
{
    for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
        KeyIsDown[i] = false;

    node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    node->setMD2Animation(irr::scene::EMAT_STAND);
    node->setMaterialTexture(0, texture);

}

void character::action(const f32 speed, const f32 dt)
{
    if (isjumping)
        jump();
    if (isWeb)
    {
        if(isHook)
            hook();
        pendulum();
    }
    if (!isHook)
        handlekeyDown(speed, dt);
    update_camera();
}

void character::handlekeyDown(const f32 speed, const f32 dt)
{
    ic::vector3df position = node->getPosition();
    ic::vector3df rotation = node->getRotation();
    if (IsKeyDown(KEY_KEY_Z))
    {
        position.X += speed * dt * cos(rotation.Y * M_PI / 180.0);
        position.Z += -speed * dt * sin(rotation.Y * M_PI / 180.0);
    }
    // Recul
    if (IsKeyDown(KEY_KEY_S))
    {
        position.X += -1 * cos(rotation.Y * M_PI / 180.0);
        position.Z += 1 * sin(rotation.Y * M_PI / 180.0);
    }
    // tourne à droite
    if (IsKeyDown(KEY_KEY_D))
    {
        position.X += 1  * cos((rotation.Y * M_PI) / 180.0 + M_PI/2);
        position.Z += -1 * sin((rotation.Y * M_PI)/ 180.0 + M_PI/2);
    }
    // tourne à gauche
    if (IsKeyDown(KEY_KEY_Q))
    {
        position.X += -1  * cos((rotation.Y * M_PI) / 180.0 + M_PI/2);
        position.Z += 1 * sin((rotation.Y * M_PI)/ 180.0 + M_PI/2);
    }
    node->setPosition(position);
    node->setRotation(rotation);
}


// This is used to check whether a key is being held down
// keyboard
bool character::IsKeyDown(EKEY_CODE keyCode) const
{
    return KeyIsDown[keyCode];
}

void character::jump()
{
    if (jump_speed >= 0)
    {
        ic::vector3df position = node->getPosition();
        position.Y += jump_speed;
        jump_speed -= 0.1;

        node->setPosition(position);
    }
    else
    {
        isjumping = false;
        if (IsKeyDown(KEY_KEY_Z))
            node->setMD2Animation(irr::scene::EMAT_RUN);
        else
            node->setMD2Animation(irr::scene::EMAT_STAND);

    }
}


void character::jump_press()
{
    if (!isjumping && !isHook)
    {
        isjumping = true;
        jump_speed = 7.0;
        node->setMD2Animation(irr::scene::EMAT_JUMP);
    }
    // Si on appuie sur sauter pendant que le personnage est accroché, il lache la toile // en cours
    else if(isHook)
    {
        ic::vector3df antiG(0,-10,0);
        anim->setGravity(antiG);
        isHook = false;
        isWeb = false;
    }
}

void character::run_forward()
{
    if (!IsKeyDown(KEY_KEY_Z) && !isHook)
        node->setMD2Animation(irr::scene::EMAT_RUN);
}

void character::walk_back()
{
    if (!IsKeyDown(KEY_KEY_S) && !isHook)
        node->setMD2Animation(irr::scene::EMAT_RUN);
        node->setAnimationSpeed(20);
}

void character::stop()
{
    node->setMD2Animation(irr::scene::EMAT_STAND);
}

// tire la toile
void character::web()
{
    node->setMD2Animation(irr::scene::EMAT_POINT);

    ic::vector3df position = node->getPosition();
    ic::vector3df rotationWeb(0,1,0);

    irr::core::line3d <f32 > ray;
    ray = collision_manager->getRayFromScreenCoordinates(irr::core::position2d <s32 >(Nx/2 , Ny/4 ));

    irr::core:: triangle3df  hit_triangle;
    irr::scene :: ISceneNode *selected_scene_node;
    std::cout<<"intersect before = "<<intersection.Y<<"  "<<intersection.Y<<"  "<<intersection.Z<<std::endl;
    selected_scene_node = collision_manager->getSceneNodeAndCollisionPointFromRay(ray ,
                                                                                   intersection , // On récupère ici  les  coordonnées 3D de l’intersection
                                                                                   hit_triangle); // et le  triangle  intersecté


    std::cout<<"intersect after = "<<intersection.X<<"  "<<intersection.Y<<"  "<<intersection.Z<<std::endl;
    selected_scene_node->getAbsolutePosition(); // enlever les warnings d'inutilisation

    targetWeb = intersection - position;

    irr::core::quaternion quat;
    quat.rotationFromTo(rotationWeb,targetWeb);

    ic::vector3df rot_final;
    quat.toEuler(rot_final);

    nodeWeb->setRotation(180/M_PI * rot_final);
    nodeWeb->setPosition(position + targetWeb);

    const core::vector3df scale( 1, -targetWeb.getLength(),1 );

    nodeWeb->setScale( scale );


    //node->setPosition(position + (intersection-position)/4.0);
    isWeb = true;
    isHook = true;

    ic::vector3df antiG(0,0,0);
    anim->setGravity(antiG);


}

// fait balancer spider-man quand il a accroché sa toile
void character::pendulum()
{/*
    ic::vector3df rotation_init = node->getRotation();
    ic::vector3df position = node->getPosition();

    ic::vector3df rot(0,0,0);
    rot.X = cos((rotation_init.Y * M_PI) / 180.0 + M_PI/2);
    rot.Z = - sin((rotation_init.Y * M_PI)/ 180.0 + M_PI/2);
    rot.normalize();


  //  ic::vector3df new_position = position;


    node->setPosition(intersection);
    node->setRotation(10*rot);
    node->setPosition(position - targetWeb);

    if(!isHook && !isWeb)
    {
        ic::vector3df antiG(0,-10,0);
        anim->setGravity(antiG);
    }*/
}

// fait avancer spider-man le long de la toile
void character::hook()
{

    /*std::cout<<"targetweb : "<<targetWeb.getLength()<<std::endl;

    std::cout<<"diff : "<<(intersection - node->getPosition()).getLength()<<std::endl;*/

    ic::vector3df dir = targetWeb;
    dir.normalize();
    node->setPosition(node->getPosition() + 3*dir);

    // QUand on atteint les 2/3 de la toile, le personnage arrete de grimper automatiquement
    if ((intersection - node->getPosition()).getLength() < 0.33 * targetWeb.getLength())
    {
        isWeb = false;
        isHook = false;
    }


    if(!isHook && !isWeb)
    {
        ic::vector3df antiG(0,-10,0);
        anim->setGravity(antiG);
    }


}


void character::update_camera()
{

    ic::vector3df position = node->getPosition();
    ic::vector3df rotation = node->getRotation();

    position.X += 1 * cos(rotation.Y * M_PI / 180.0);
    position.Z += - 1 * sin(rotation.Y * M_PI / 180.0);
    position.Y += 0.1 * position_camera_y;
    camera->setTarget(position);

}
