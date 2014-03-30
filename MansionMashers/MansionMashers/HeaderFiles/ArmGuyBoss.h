#ifndef FOX_ARMBOSS
#define FOX_ARMBOSS

#include "Vector2.h"
#include "RigidBody.h"
#include "Sprite.h"
#include "CollisionBox.h"

typedef struct ArmGuyBoss
{
	Sprite *BodySprite;
	Sprite *SpinSprite;
	Sprite *ArmSprite; // Arm for armguy, hand for handguy etc
	Sprite *OffArmSprite;
	Sprite *ArmJabSprite;
	Sprite *ArmSmashSprite;
	struct ParticleSystem *ArmGuyParticle;

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
void DetectArmGuyBossCollision(ArmGuyBoss *CurrentBoss);
void ArmGuyBossCollideWeapon(ArmGuyBoss *CurrentBoss);
static void PlayerDamageResult(int damage);

#endif