#ifndef FOX_ENEMY
#define FOX_ENEMY

#include "Vector2.h"
#include "RigidBody.h"
#include "Sprite.h"
#include "CollisionBox.h"

enum EnemyType { Dummy, BasicMelee, BasicRanged, AdvancedMelee, AdvancedRanged, ExpertMelee, ExpertRanged};
enum AIState { AINone, AIPassive, AIAggressive, AIIdle};

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
	float BlinkTimer;
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

typedef struct EnemySounds
{
	//Weapon sounds
	FoxSound *Poof;
	FoxSound *GetHit1;
	FoxSound *GetHit2;
	FoxSound *Swing1;
	FoxSound *Swing2;
}EnemySounds;

typedef struct Enemy
{
	int EnemyType;
	int objID;
	int CollisionData[COLLIDEAMOUNT];

	int panelId;

	float Speed;

	int isAttacking;

	int KnockBack;
	int KnockBackTime;
	enum Direction KnockBackDir;

	float LegSinValue;
	float TailSinValue;

	int dropDown;  //Fake Boolean
	float dropdownTimer;

	int idleMove;
	int idleTimer;
	int isMoveRight;
	int isMoveLeft;
	int isJumping;
	int jumpTimer;
	int isDropDown;
	int canDropDownTimer;
	int Attack;
	int canAttackTimer;
	int canAttack;
	int StateTimer;
	int findHome;
	Vec2 HomePos;
	enum AIState EnemyState;

	float HitByProjectileTimer; // timer for not letting the enemy get hit multiple times by the projectiles

	enum Direction EnemyDirection;
	Sprite *EnemySprite;
	RigidBody EnemyRigidBody;
	EnemyStats CurrentEnemyStats;
	CollisionBox EnemyCollider;
	EnemyParts EnemySpriteParts;
	struct ParticleSystem* EnemyParticleSystem;
	struct Weapon* EnemyWeapon;
	struct EnemySounds CurrentEnemySounds;
	Vec2 Position;

}Enemy;

void LoadEnemy(int enemyType);
Enemy* CreateEnemy(int enemyType, int collisionGroup, int objID, float xPos, float yPos, int panelId);
void InitializeEnemyStats(Enemy *CurrentEnemy, int maxHP, float movSpeed, float atkSpeed, float dmgReduction, int dmg, int money, int exp);
void UpdateEnemy(Enemy *CurrentEnemy);
void UpdateAllEnemies(void);
void DetectEnemyCollision(Enemy *CurrentEnemy);
void EnemyAnimation(Enemy *Object);
void CreateEnemySprites(Enemy *Object);
void EnemyAIUpdate(Enemy *Object);
void EnemyBasicMeleeUpdate(Enemy *Object);
void ResetEnemyPanelNumber(void);

void EnemyBasicRangedUpdate(Enemy *CurrentEnemy);
void EnemyAnimationBasicRanged(Enemy *Object);

float RotateToAngle(float angle, float angleTo, float speed);
#endif
