#ifndef FOX_HANDBOSS
#define FOX_HANDBOSS

#include "Vector2.h"
#include "RigidBody.h"
#include "Sprite.h"
#include "CollisionBox.h"
#include "Platform.h"
#include "FoxObjects.h"
#include "Sound.h"


typedef struct HandGuyParts
{
	Sprite *LegUpper;
	Sprite *LegLower;
	Sprite *LegUpper2;
	Sprite *LegLower2;
	Sprite *Skirt;
	Sprite *Body;
	Sprite *Tail;
	Sprite *ArmUpper;
	Sprite *ArmLower;
	Sprite *ArmUpper2;
	Sprite *ArmLower2;
	float AttackRotationArm;
	float AttackRotationArmLower;
	float AttackRotationArm2;
	float AttackRotationArmLower2;
}HandGuyParts;

typedef struct HandGuyBoss
{
	Sprite *BodySprite;
	Sprite *JabSprite;
	
	HandGuyParts HandGuySpriteParts;

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

	FoxSound* HandGuyHit[6];
	FoxSound* HandGuyPhrase;
	FoxSound* HandGuyYell;
	int HandGuySoundsPlay;

	int numProjectiles;

	float LegSinValue;
	float TailSinValue;
	int QuestionAttackAnimation; // bool for animation

	float Speed;
	Vec2 Position;
}HandGuyBoss;

void LoadHandGuyBoss(void);
HandGuyBoss* CreateHandGuyBoss(float xPos, float yPos);
void UpdateHandGuyBoss(HandGuyBoss *CurrentBoss);
void DetectHandGuyBossCollision(HandGuyBoss *CurrentBoss);
void HandGuyBossCollideWeapon(HandGuyBoss *CurrentBoss);
static void PlayerDamageResult(int damage);
void HandGuyPlatformCollision(HandGuyBoss* CurrentBoss, Platform* CurrentPlatform);
void HandGuyAnimation( HandGuyBoss *Object);
void CreateHandGuySprites(HandGuyBoss *Object);

#endif