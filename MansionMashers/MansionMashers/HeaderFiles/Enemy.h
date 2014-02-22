#ifndef FOX_ENEMY
#define FOX_ENEMY

#include "Vector2.h"
#include "RigidBody.h"
#include "Sprite.h"
#include "CollisionBox.h"

enum EnemyType { Dummy, BasicMelee, BasicRanged, AdvancedMelee, AdvancedRanged, ExpertMelee, ExpertRanged};

typedef struct EnemyParts
{
	Sprite *LegUpper;
	Sprite *LegLower;
	Sprite *LegUpper2;
	Sprite *LegLower2;
	Sprite *Body;
	Sprite *Tail;
	Sprite *ArmUpper;
	Sprite *ArmLower;
	Sprite *ArmUpper2;
	Sprite *ArmLower2;
	Sprite *Weapon;
	Sprite *Skirt;
	float AttackRotation;
	float AttackRotationArm;
	float AttackRotationArmLower;
	int BlinkTimer;
}EnemyParts;

typedef struct EnemyStats
{
	int MaxHealth;
	int CurrentHealth;
	float MoveSpeed;
	float AttackSpeed;
	float DamageReduction;
	int Damage;

	//Defeated enemy rewards
	int Money;
	int Experience;
}EnemyStats;

typedef struct Enemy
{
	int EnemyType;
	int objID;
	int CollisionData[COLLIDEAMOUNT];

	float Speed;

	int isAttacking;

	float LegSinValue;
	float TailSinValue;

	int dropDown;  //Fake Boolean

	Sprite *EnemySprite;
	RigidBody EnemyRigidBody;
	EnemyStats CurrentEnemyStats;
	CollisionBox EnemyCollider;
	EnemyParts EnemySpriteParts;
	struct ParticleSystem* EnemyParticleSystem;
	struct Weapon* EnemyWeapon;

	Vec2 Position;

}Enemy;

Enemy* CreateEnemy(int enemyType, int collisionGroup, int objID, float xPos, float yPos);
void InitializeEnemyStats(Enemy *CurrentEnemy, int maxHP, float movSpeed, float atkSpeed, float dmgReduction, int dmg, int money, int exp);
void UpdateEnemy(Enemy *CurrentEnemy);
void DetectEnemyCollision(Enemy *CurrentEnemy);
void EnemyAnimation(Enemy *Object);
void CreateEnemySprites(Enemy *Object);

float RotateToAngle(float angle, float angleTo, float speed);
#endif