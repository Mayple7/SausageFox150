#ifndef FOX_ENEMY
#define FOX_ENEMY

#include "Vector2.h"
#include "RigidBody.h"
#include "Sprite.h"
#include "CollisionBox.h"

enum EnemyType { Dummy, BasicMelee, BasicRanged, AdvancedMelee, AdvancedRanged, ExpertMelee, ExpertRanged};

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

	Sprite *EnemySprite;
	RigidBody EnemyRigidBody;
	EnemyStats CurrentEnemyStats;
	CollisionBox EnemyCollider;
	struct ParticleSystem* EnemyParticleSystem;
	struct Weapon* EnemyWeapon;

	Vec2 Position;

}Enemy;

Enemy* CreateEnemy(int enemyType, int collisionGroup, int objID, float xPos, float yPos);
void InitializeEnemyStats(Enemy *CurrentEnemy, int maxHP, float movSpeed, float atkSpeed, float dmgReduction, int dmg, int money, int exp);
void UpdateEnemy(Enemy *CurrentEnemy);
void DetectEnemyCollision(Enemy *CurrentEnemy);

#endif