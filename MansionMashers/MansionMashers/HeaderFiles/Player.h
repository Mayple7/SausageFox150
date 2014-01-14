#ifndef FOX_PLAYER
#define FOX_PLAYER

#ifndef MAX_INVENTORY
#define MAX_INVENTORY 20
#endif

struct Player
{
	struct Sprite PlayerSprite;
	//struct RigidBody PlayerRigidBody;
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


	float XPos;
	float YPos;
};

void InitializePlayer(struct Player *Player);
void DrawPlayer(struct Player *Player); //Draw player sprite, current weapon sprite
void ApplyPhysics(struct Player *Player);

#endif