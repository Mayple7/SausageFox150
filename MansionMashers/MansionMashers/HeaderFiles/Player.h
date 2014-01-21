#ifndef FOX_PLAYER
#define FOX_PLAYER

#define MAX_INVENTORY 20
#include "Vector2.h"
#include "RigidBody.h"

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
	//struct CollisionInfo PlayerCollision;
	//struct Transform PlayerTransform;

	PlayerStats CurrentPlayerStats;

	Vec2 Position;

}Player;

void InitializePlayer(struct Player *CurrentPlayer);
void DrawPlayer(struct Player *CurrentPlayer); //Draw player sprite, current weapon sprite
void UpdatePosition(struct Player *CurrentPlayer);
void InputPlayer(struct Player *CurrentPlayer);

//Updating non-modifiable player stats
void updateMaxHealth(PlayerStats *CurrentPlayerStats);
void updateMoveSpeed(PlayerStats *CurrentPlayerStats);
void updateAttackSpeed(PlayerStats *CurrentPlayerStats);
void updateDamageReduction(PlayerStats *CurrentPlayerStats);
//void updateDamage(PlayerStats *CurrentPlayerStats, Weapon *EquippedWeapon);


#endif