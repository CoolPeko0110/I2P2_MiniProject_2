//
// Created by johnn on 2024/5/13.
//

#include "ShapaBullet.hpp"
#include <cmath>
#include <random>
#include <string>
#include <utility>

#include "UI/Animation/DirtyEffect.hpp"
#include "Enemy/Enemy.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "ShapaBullet.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"

class Turret;

ShapaBullet::ShapaBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent) :
	Bullet("play/bullet-11.png", 80, 10, position, forwardDirection, rotation + ALLEGRO_PI / 2, parent) {
}
void ShapaBullet::Update(float deltaTime) {
	if (!Target) {
		float minDistance = INFINITY;
		Enemy* enemy = nullptr;
		for (auto& it : getPlayScene()->EnemyGroup->GetObjects()) {
			Enemy* e = dynamic_cast<Enemy*>(it);
			float distance = (e->Position - Position).Magnitude();
			if (distance < minDistance) {
				minDistance = distance;
				enemy = e;
			}
		}
		if (!enemy) {
			Bullet::Update(deltaTime);
			return;
		}
		Target = enemy;
		Target->lockedBullets.push_back(this);
		lockedBulletIterator = std::prev(Target->lockedBullets.end());
	}
	Engine::Point originVelocity = Velocity.Normalize();
	Engine::Point targetVelocity = (Target->Position - Position).Normalize();
	float maxRotateRadian = rotateRadian * deltaTime;
	float cosTheta = originVelocity.Dot(targetVelocity);
	// Might have floating-point precision error.
	if (cosTheta > 1) cosTheta = 1;
	else if (cosTheta < -1) cosTheta = -1;
	float radian = acos(cosTheta);
	if (abs(radian) <= maxRotateRadian)
		Velocity = targetVelocity;
	else
		Velocity = ((abs(radian) - maxRotateRadian) * originVelocity + maxRotateRadian * targetVelocity) / radian;
	Velocity = speed * Velocity.Normalize();
	Rotation = atan2(Velocity.y, Velocity.x) + ALLEGRO_PI / 2;
	Bullet::Update(deltaTime);
}
void ShapaBullet::OnExplode(Enemy* enemy) {
	Target->lockedBullets.erase(lockedBulletIterator);
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(4, 10);
	getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-4.png", dist(rng), enemy->Position.x, enemy->Position.y));
}

