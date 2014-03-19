#ifndef FOX_ARMBOSS
#define FOX_ARMBOSS

#include "Vector2.h"
#include "RigidBody.h"
#include "Sprite.h"
#include "CollisionBox.h"
#include "Platform.h"

typedef struct HandGuyBoss
{
	Sprite *BodySprite;
	Sprite *JabSprite;

	int CurrentState;
	int InnerState;
	int PositionState;

	float ShoutRadius;

	RigidBody HandGuyRigidBody;
	int dropDown;

	CollisionBox JabAttack;
	CollisionBox BossCollider;
	int playerHit; // No need for a collision list
	float cooldownTimer;
	int JabDamage;
	int QuestionDamage;
	int ShoutDamage;

	int MaxHealth;
	int CurrentHealth;

	Vec2 Position;
}HandGuyBoss;

void LoadHandGuyBoss(void);
HandGuyBoss* CreateHandGuyBoss(float xPos, float yPos, int *objID);
void UpdateHandGuyBoss(HandGuyBoss *CurrentBoss);
void DetectHandGuyBossCollision(HandGuyBoss *CurrentBoss);
void HandGuyBossCollideWeapon(HandGuyBoss *CurrentBoss);
static void PlayerDamageResult(int damage);
void HandGuyPlatformCollision(HandGuyBoss* CurrentBoss, Platform* CurrentPlatform);

#endif