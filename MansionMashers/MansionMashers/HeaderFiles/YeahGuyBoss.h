#ifndef FOX_YEAHBOSS
#define FOX_YEAHBOSS

#include "Vector2.h"
#include "RigidBody.h"
#include "Sprite.h"
#include "CollisionBox.h"
#include "Platform.h"

typedef struct YeahGuyBoss
{
	Sprite *BodySprite;

	int CurrentState;
	int InnerState;
	int PositionState;

	int redHead;
	int greenHead;
	int blueHead;

	CollisionBox BossCollider;
	RigidBody YeahGuyRigidBody;
	int dropDown;

	int playerHit; // No need for a collision list
	float cooldownTimer;

	int YeahAOEDamage;
	float YeahAOERadius;
	int YeahPoundDamage;
	int YeahProjectileDamage;

	int MaxHealth;
	int CurrentRedHealth;
	int CurrentGreenHealth;
	int CurrentBlueHealth;

	int numHeads;

	Vec2 Position;
}YeahGuyBoss;

void LoadYeahGuyBoss(void);
YeahGuyBoss* CreateYeahGuyBoss(float xPos, float yPos);
void UpdateYeahGuyBoss(YeahGuyBoss *CurrentBoss);
void DetectYeahGuyBossCollision(YeahGuyBoss *CurrentBoss, int CurrentBuff);
void YeahGuyBossCollideWeapon(YeahGuyBoss *CurrentBoss, int CurrentBuff);
static void PlayerDamageResult(int damage);
void YeahGuyPlatformCollision(YeahGuyBoss* CurrentBoss, Platform* CurrentPlatform);

#endif