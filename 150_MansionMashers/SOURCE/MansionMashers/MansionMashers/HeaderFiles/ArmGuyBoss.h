/*****************************************************************************/
/*!
\file				ArmGuyBoss.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_ARMBOSS
#define FOX_ARMBOSS

#include "Vector2.h"
#include "RigidBody.h"
#include "Sprite.h"
#include "CollisionBox.h"
#include "Sound.h"

typedef struct ArmGuyBoss
{
	Sprite *BodySprite;
	Sprite *SpinSprite;
	Sprite *ArmSprite; // Arm for armguy, hand for handguy etc
	Sprite *OffArmSprite;
	Sprite *ArmJabSprite;
	Sprite *ArmSmashSprite;

	struct ParticleSystem *ArmGuyParticle;
	struct ParticleSystem *DamageParticle;

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

	FoxSound* ArmGuyHit[4];			//When boss gets hit
	FoxSound* ArmGuyPhrase[4];		//To taunt player
	FoxSound* ArmGuyPhraseSmash;	//Saying for smash attacj
	int ArmGuySoundsPlay;			//Bool for if sounds are playing

	FoxSound* JumpSFX;
	FoxSound* PunchSFX;
	FoxSound* SmashSFX;
	FoxSound* SpinSFX;
	FoxSound* Spin2SFX;
	FoxSound* SwooshSFX;


	Vec2 Position;
}ArmGuyBoss;

void LoadArmGuyBoss(void);
ArmGuyBoss* CreateArmGuyBoss(float xPos, float yPos);
void UpdateArmGuyBoss(ArmGuyBoss *CurrentBoss);
void DetectArmGuyBossCollision(ArmGuyBoss *CurrentBoss);
void ArmGuyBossCollideWeapon(ArmGuyBoss *CurrentBoss);
static void PlayerDamageResult(int damage);
void FreeArmGuyBoss(ArmGuyBoss* CurrentBoss);

#endif