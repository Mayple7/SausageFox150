#ifndef FOX_PLAYER
#define FOX_PLAYER

#define MAX_INVENTORY 20
#define PLAYER_WIDTH 100
#define PLAYER_HEIGHT 200

#include "Vector2.h"
#include "RigidBody.h"
#include "Sprite.h"
#include "CollisionBox.h"
#include "FoxObjects.h"
#include "Sound.h"

enum Character { Mayple, Ginko, Holly, Kaya};

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
	float AttackRotation;
	float AttackRotationArm;
	float AttackRotationArmLower;
	float BlinkTimer;
}PlayerParts;

typedef struct PlayerSounds
{
	//Weapon sounds
	FoxSound *Swing1;
	FoxSound *Swing2;

	//Character sounds
	FoxSound *GetHit1;
	FoxSound *GetHit2;
	FoxSound *YeahBebeGurrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrl;
}PlayerSounds;

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

	float AgilityTimer;
	float StrengthTimer;
	float DefenseTimer;
	float HasteTimer;

	int Money;

	//Level
	int Rank;	    //Wanted to name this "Echelon" but seemed unnecessary
	int Experience; //XP
	int Upgrades;   //How many times the player ranked up
}PlayerStats;

typedef struct Player
{
	enum Character Princess;
	int CurrentLevel;
	int PlayerActive;

	int levelClearBitFlags;
	int armUnlock;
	int handUnlock;
	int armClear;
	int handClear;

	RigidBody PlayerRigidBody;
	CollisionBox PlayerCollider;
	struct Weapon* PlayerWeapon;
	int CollisionData[COLLIDEAMOUNT];

	PlayerParts PlayerSpriteParts;
	PlayerStats CurrentPlayerStats;
	PlayerSounds CurrentPlayerSounds;

	int Zindex;
	int BuffHeld[4];
	int BuffSelected;

	float dropdownTimer;

	float Speed;

	int isAttacking;
	int AttackType; //0 = hitting, 1 = projectile

	float LegSinValue;
	float TailSinValue;

	int FlipX;
	int FlipY;
	int FlipXPrev;
	int FlipYPrev;
	
	enum Direction PlayerDirection;

	Vec2 Position;
	int dropDown;  //Fake Boolean

}Player;

//Player Global
Player CurrentPlayer;

//Functions
void InitializePlayer(struct Player *CurrentPlayer, enum Character Princess, float xPos, float yPos);
void UpdatePlayerPosition(struct Player *CurrentPlayer);
void DestroyPlayer(Player *CurrentPlayer);
void InputPlayer(struct Player *CurrentPlayer);
void DetectPlayerCollision(void);
void Animation(Player *Object);
void CreatePlayerSprites(Player *Player);
void SavePlayer(Player *CurrentPlayer);
int LoadPlayer(Player *CurrentPlayer);
void LoadNewPlayer(Player *CurrentPlayer, enum Character Princess);

//Updating non-modifiable player stats
void updateMaxHealth(PlayerStats *CurrentPlayerStats);
void updateMoveSpeed(PlayerStats *CurrentPlayerStats);
void updateAttackSpeed(PlayerStats *CurrentPlayerStats);
void updateDamageReduction(PlayerStats *CurrentPlayerStats);
void updateDamage(Player *CurrentPlayer);
void UpdateBuffTimers(Player* CurrentPlayer);

float RotateToAngle(float angle, float angleTo, float speed);

#endif
