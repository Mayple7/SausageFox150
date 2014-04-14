#ifndef FOX_YEAHBOSS
#define FOX_YEAHBOSS

#include "Vector2.h"
#include "RigidBody.h"
#include "Sprite.h"
#include "CollisionBox.h"
#include "Platform.h"
#include "Sound.h"

typedef struct YeahGuyParts
{
	Sprite *LegUpper;
	Sprite *LegLower;
	Sprite *LegUpper2;
	Sprite *LegLower2;
	Sprite *Body;
	Sprite *HeadBlue;
	Sprite *HeadRed;
	Sprite *HeadGreen;
	Sprite *Tail;
	float AttackRotationArm;
	float AttackRotationArmLower;
	float AttackRotationArm2;
	float AttackRotationArmLower2;
}YeahGuyParts;

typedef struct YeahGuyBoss
{
	Sprite *BodySprite;

	YeahGuyParts YeahGuySpriteParts;
	float LegSinValue;
	float TailSinValue;

	int CurrentState;
	int InnerState;
	int PositionState;

	int redHead;
	int greenHead;
	int blueHead;

	struct ParticleSystem *DamageParticle;
	struct ParticleSystem *PoundParticle[2];
	struct ParticleSystem *AOEParticle;

	enum HeadColor HeadAttacking;

	float Speed;

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

	FoxSound* YeahGuyHit[4];		//When boss gets hit
	FoxSound* YeahGuyYell;			//Saying for large attack
	FoxSound* YeahGuyDie;			//When a head dies
	FoxSound* SmashSFX;
	int YeahGuySoundsPlay;			//bool

	Vec2 Position;
}YeahGuyBoss;

void LoadYeahGuyBoss(void);
YeahGuyBoss* CreateYeahGuyBoss(float xPos, float yPos);
void UpdateYeahGuyBoss(YeahGuyBoss *CurrentBoss);
void DetectYeahGuyBossCollision(YeahGuyBoss *CurrentBoss, int CurrentBuff);
void YeahGuyBossCollideWeapon(YeahGuyBoss *CurrentBoss, int CurrentBuff);
static void PlayerDamageResult(int damage);
void YeahGuyPlatformCollision(YeahGuyBoss* CurrentBoss, Platform* CurrentPlatform);
void CreateYeahGuySprites(YeahGuyBoss *Object);
void FreeYeahGuyBoss(YeahGuyBoss* CurrentBoss);
void YeahGuyAnimation(YeahGuyBoss *Object);
void PoofSelf(Sprite *Boss);
void PoofHead(Sprite *Head);

#endif