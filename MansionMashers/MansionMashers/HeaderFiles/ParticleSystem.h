#ifndef FOX_PARTICLES
#define FOX_PARTICLES

#define NUMBER_OF_PARTICLE_SYSTEMS 10

#include "Vector2.h"
#include "Sprite.h"
#include "CollisionBox.h"
#include "FoxObjects.h"

unsigned long lastRandomNumber;

typedef struct ParticleSystem
{
	char *ParticleSprite;
  
	Vec2 Position;

	int objID;

	int amountTotal;
	int emitAmount;

	float emitSpeed;
	float emitSpeedTimer;

	int emitAngle;					// Degrees
	int emitAngleRandom;			// Degrees

	float emitScale;

	float emitVelocity;
	float emitLife;

	int emitDisplacementX;
	int emitDisplacementY;


}ParticleSystem;

typedef struct Particle
{
	Sprite *ParticleSprite;
  
	Vec2 Position;

	Vec2 Velocity;

	int objID;

	float Lifetime;

	float LifetimeMax;

	float RotationSpeed;

}Particle;

ParticleSystem* CreateFoxParticleSystem(char* particleTexture, float posX, float posY, int amountTotal, int emitAmount, float emitSpeed, int emitAngle, int emitAngleRandom, float emitScale, int emitDisplacementX, int emitDisplacementY, float emitVelocity, float emitLife);

void CreateFoxParticle(char* particleTexture, float posX, float posY, float VelX, float VelY, float Life, float Scale);

void ParticleSystemUpdate(void);
void ParticleUpdate(void);

#endif