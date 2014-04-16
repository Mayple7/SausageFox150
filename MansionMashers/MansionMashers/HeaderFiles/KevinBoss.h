#ifndef FOX_KEVINBOSS
#define FOX_KEVINBOSS

#include "Vector2.h"
#include "RigidBody.h"
#include "Sprite.h"
#include "CollisionBox.h"
#include "Sound.h"

typedef struct KevinParts
{
	Sprite *LegUpper;
	Sprite *LegLower;
	Sprite *LegUpper2;
	Sprite *LegLower2;
	Sprite *Body;
	Sprite *ArmUpper;
	Sprite *ArmLower;
	Sprite *ArmUpper2;
	Sprite *ArmLower2;
	float AttackRotationArm;
	float AttackRotationArmLower;
	float AttackRotationArm2;
	float AttackRotationArmLower2;
}KevinParts;

typedef struct KevinBoss
{
	Sprite *BodySprite;
	Sprite *JabSprite;

	int CurrentState;
	int InnerState;
	int PositionState;

	float LegSinValue;

	RigidBody KevinRigidBody;

	KevinParts KevinSpriteParts;

	CollisionBox JabAttack;
	CollisionBox BossCollider;

	float jumpTimer;
	int playerHit; // No need for a collision list
	float cooldownTimer;
	int SpinDamage;
	int SmashDamage;
	int JabDamage;

	int MaxHealth;
	int CurrentHealth;

	FoxSound* KevinStart;
	FoxSound* KevinHit[4];
	FoxSound* KevinPhrase[4];
	int KevinSoundsPlay;

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
void CreateKevinSprites(KevinBoss *Object);
void KevinAnimation(KevinBoss *Object);
void PoofSelf(Sprite *Boss);

#endif