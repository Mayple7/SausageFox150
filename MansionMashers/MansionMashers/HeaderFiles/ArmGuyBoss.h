#ifndef FOX_BOSS
#define FOX_BOSS

#include "Vector2.h"
#include "RigidBody.h"
#include "Sprite.h"
#include "CollisionBox.h"

typedef struct ArmGuyBoss
{
	int BossType;

	Sprite *BodySprite;
	Sprite *ArmSprite; // Arm for armguy, hand for handguy etc

	int CurrentState;
	int InnerState;

	CollisionBox SpinAttack;
	CollisionBox JabAttack;
	CollisionBox SmashAttack;
	CollisionBox BossCollider;
	int playerHit; // No need for a collision list
	float cooldownTimer;
	int SpinDamage;
	int SmashDamage;
	int JabDamage;

	int MaxHealth;
	int CurrentHealth;

	Vec2 Position;
}ArmGuyBoss;

void LoadArmGuyBoss(void);
ArmGuyBoss* CreateArmGuyBoss(float xPos, float yPos, int *objID);
void UpdateArmGuyBoss(ArmGuyBoss *CurrentBoss);
void DetectBossCollision(ArmGuyBoss *CurrentBoss);
void BossCollideWeapon(ArmGuyBoss *CurrentBoss);

#endif