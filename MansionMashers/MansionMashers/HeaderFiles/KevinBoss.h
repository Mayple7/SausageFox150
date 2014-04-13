#ifndef FOX_KEVINBOSS
#define FOX_KEVINBOSS

#include "Vector2.h"
#include "RigidBody.h"
#include "Sprite.h"
#include "CollisionBox.h"
#include "Sound.h"

typedef struct KevinBoss
{
	Sprite *BodySprite;
	Sprite *JabSprite;

	int CurrentState;
	int InnerState;
	int PositionState;

	RigidBody KevinRigidBody;

	CollisionBox JabAttack;
	CollisionBox BossCollider;

	int playerHit; // No need for a collision list
	float cooldownTimer;
	int SpinDamage;
	int SmashDamage;
	int JabDamage;

	int MaxHealth;
	int CurrentHealth;

	float Speed;
	Vec2 Position;
}KevinBoss;

void LoadKevinBoss(void);
KevinBoss* CreateKevinBoss(float xPos, float yPos);
void UpdateKevinBoss(KevinBoss *CurrentBoss);
void DetectKevinBossCollision(KevinBoss *CurrentBoss);
void KevinBossCollideWeapon(KevinBoss *CurrentBoss);
static void PlayerDamageResult(int damage);
void FreeKevinBoss(KevinBoss* CurrentBoss);

#endif