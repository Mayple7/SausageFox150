#ifndef FOX_PLAYER
#define FOX_PLAYER

#define MAX_INVENTORY 20
#include "Vector2.h"
#include "RigidBody.h"
#include "Sprite.h"
#include "CollisionBox.h"

typedef struct PlayerStats
{
	//Modifiable Stats
	//Max Stats = 20?
	int Strength;
	int Agility;
	int Defense;

	//Non-Modifiable Stats
	int MaxHealth;
	int CurrentHealth;
	float MoveSpeed;
	float AttackSpeed;
	float DamageReduction;
	int Damage;

	int Money;
	/*struct Inventory
	{
		struct Weapon InvWeapons[MAX_INVENTORY];
	};*/

	//struct Weapon CurrentWeapon;
}PlayerStats;

typedef struct Player
{
	Sprite *PlayerSprite;
	RigidBody PlayerRigidBody;
	CollisionBox PlayerCollider;
	//struct CollisionInfo PlayerCollision;
	//struct Transform PlayerTransform;

	PlayerStats CurrentPlayerStats;

	Vec2 Position;
	int dropDown;  //Fake Boolean

}Player;

void InitializePlayer(struct Player *CurrentPlayer);
void UpdatePlayerPosition(struct Player *CurrentPlayer);
void InputPlayer(struct Player *CurrentPlayer);
void HandleCollision(Sprite *objHit);
void DetectPlayerCollision(void);


//Updating non-modifiable player stats
void updateMaxHealth(PlayerStats *CurrentPlayerStats);
void updateMoveSpeed(PlayerStats *CurrentPlayerStats);
void updateAttackSpeed(PlayerStats *CurrentPlayerStats);
void updateDamageReduction(PlayerStats *CurrentPlayerStats);
//void updateDamage(PlayerStats *CurrentPlayerStats, Weapon *EquippedWeapon);


#endif