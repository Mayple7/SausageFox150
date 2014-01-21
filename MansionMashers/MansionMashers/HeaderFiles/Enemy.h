#ifndef FOX_ENEMY
#define FOX_ENEMY

#include "Vector2.h"
#include "RigidBody.h"
#include "Sprite.h"

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

	//struct Weapon CurrentWeapon;
}EnemyStats;

typedef struct Enemy
{
	int EnemyType;

	Sprite *EnemySprite;
	RigidBody EnemyRigidBody;
	EnemyStats CurrentEnemyStats;
	//struct CollisionInfo EnemyCollision;
	//struct Transform EnemyTransform;

	Vec2 Position;

}Enemy;

void InitializeEnemy(Enemy *CurrentEnemy);
void DrawEnemy(Enemy *CurrentEnemy); //Draw player sprite, current weapon sprite
void UpdateEnemyPosition(Enemy *CurrentEnemy);
void EnemyLogic(Enemy *CurrentEnemy, Player *CurrentPlayer);

#endif