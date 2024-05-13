//
// Created by johnn on 2024/5/13.
//

#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Bullet/ShapaBullet.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"
#include "ShapaTurret.hpp"

const int ShapaTurret::Price = 2000;
ShapaTurret::ShapaTurret(float x, float y) :
    Turret("play/tower-base.png", "play/turret-8.png", x, y, 1000, Price, 1) {
    Anchor.y += 11.0f / GetBitmapHeight();
}
void ShapaTurret::CreateBullet() {
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    Engine::Point normal = Engine::Point(-normalized.y, normalized.x);
    // Change bullet position to the front of the gun barrel.
    getPlayScene()->BulletGroup->AddNewObject(new ShapaBullet(Position + normalized * 10 - normal * 6, diff, rotation, this));
    getPlayScene()->BulletGroup->AddNewObject(new ShapaBullet(Position + normalized * 10 + normal * 6, diff, rotation, this));
    AudioHelper::PlayAudio("shapa_out.wav");
}
