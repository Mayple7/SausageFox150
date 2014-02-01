#ifndef FOX_PLAYER
#define FOX_PLAYER

#define MAX_INVENTORY 20
#include "Vector2.h"
#include "RigidBody.h"
#include "Sprite.h"
#include "CollisionBox.h"
#include "FoxObjects.h"

typedef struct PlayerParts
{
	Sprite *LegUpper;
	Sprite *LegLower;
	Sprite *LegUpper2;
	Sprite *LegLower2;
	Sprite *Skirt;
	Sprite *Body;
	Sprite *Tail;
	Sprite *ArmUpper;
	Sprite *ArmLower;
	Sprite *ArmUpper2;
	Sprite *ArmLower2;
	Sprite *Weapon;
	int BlinkTimer;
}PlayerParts;

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
		Weapon InvWeapons[MAX_INVENTORY];
	};*/

	//struct Weapon CurrentWeapon;
}PlayerStats;

typedef struct Player
{
	Sprite *PlayerSprite;
	RigidBody PlayerRigidBody;
	CollisionBox PlayerCollider;
	int CollisionData[COLLIDEAMOUNT];

	PlayerParts PlayerSpriteParts;
	PlayerStats CurrentPlayerStats;

	float Speed;

	float LegSinValue;
	float TailSinValue;
  
	enum Direction PlayerDirection;

	Vec2 Position;
	int dropDown;  //Fake Boolean

}Player;

void InitializePlayer(struct Player *CurrentPlayer, int newID);
void UpdatePlayerPosition(struct Player *CurrentPlayer);
void InputPlayer(struct Player *CurrentPlayer);
void DetectPlayerCollision(void);
void Animation(Player *Object);
void CreatePlayerSprites(Player *Player);

//Updating non-modifiable player stats
void updateMaxHealth(PlayerStats *CurrentPlayerStats);
void updateMoveSpeed(PlayerStats *CurrentPlayerStats);
void updateAttackSpeed(PlayerStats *CurrentPlayerStats);
void updateDamageReduction(PlayerStats *CurrentPlayerStats);
//void updateDamage(PlayerStats *CurrentPlayerStats, Weapon *EquippedWeapon);


#endif