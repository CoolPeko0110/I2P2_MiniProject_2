//
// Created by johnn on 2024/5/13.
//

#ifndef SHAPABULLET_HPP
#define SHAPABULLET_HPP

#include <allegro5/base.h>
#include <list>

#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine {
    struct Point;
}  // namespace Engine

class ShapaBullet : public Bullet {
protected:
    const float rotateRadian = 2 * ALLEGRO_PI;
    std::list<Bullet*>::iterator lockedBulletIterator;
public:
    explicit ShapaBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent);
    void Update(float deltaTime) override;
    void OnExplode(Enemy* enemy) override;
};

#endif //SHAPABULLET_HPP
