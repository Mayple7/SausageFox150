#ifndef FOX_PLAYER
#define FOX_PLAYER

#define MAX_INVENTORY 20
#include "Vector2.h"
#include "RigidBody.h"

typedef struct Player
{
	Sprite *PlayerSprite;
	RigidBody PlayerRigidBody;
	//struct CollisionInfo PlayerCollision;
	//struct Transform PlayerTransform;

	struct PlayerStats
	{
		//Modifiable Stats
		int Strength;
		int Agility;
		int Defense;

		//Non-Modifiable Stats
		int MaxHealth;
		int CurrentHealth;
		int MoveSpeed;
		int AttackSpeed;
		int DamageReduction;
		int Damage;

		int Money;
		/*struct Inventory
		{
			struct Weapon InvWeapons[MAX_INVENTORY];
		};*/

		//struct Weapon CurrentWeapon;
	};

	Vec2 Position;

}Player;



void InitializePlayer(struct Player *Player);
void DrawPlayer(struct Player *Player); //Draw player sprite, current weapon sprite
void UpdatePosition(struct Player *Player);
void InputPlayer(struct Player *Player, int key);

#endif