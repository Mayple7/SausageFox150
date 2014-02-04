#ifndef FOX_ENEMY
#define FOX_ENEMY

#include "Vector2.h"
#include "RigidBody.h"
#include "Sprite.h"
#include "CollisionBox.h"

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
	int objID;

	Sprite *EnemySprite;
	RigidBody EnemyRigidBody;
	EnemyStats CurrentEnemyStats;
	CollisionBox EnemyCollider;

	Vec2 Position;

}Enemy;

Enemy* CreateEnemy(char* textureName, int collisionGroup, float width, float height, int objID, float xPos, float yPos);
void UpdateEnemy(Enemy *CurrentEnemy);
void EnemyLogic(Enemy *CurrentEnemy, Player *CurrentPlayer);

#endif