#ifndef FOX_PARTICLES
#define FOX_PARTICLES

#define NUMBER_OF_PARTICLE_SYSTEMS 10
#define SPRITE_NAME_LENGTH 50

#include "Vector2.h"
#include "Sprite.h"
#include "CollisionBox.h"
#include "FoxObjects.h"

unsigned long lastRandomNumber;

typedef struct ParticleSystem
{
	char ParticleSprite[SPRITE_NAME_LENGTH];
  
	Vec2 Position;

	int objID;

	int amountTotal;
	int emitAmount;

	float emitSpeed;
	float emitSpeedTimer;

	int emitAngle;					// Degrees
	int emitAngleRandom;			// Degrees

	float emitScale;
	float emitScaleSpeed;

	float emitVelocity;
	float emitLife;

	int emitDisplacementX;
	int emitDisplacementY;

	struct AEGfxVertexList* emitMesh;

	float ParticleStartAlpha;

	int ZIndex;

	int FadeIn;

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

	float ScaleSpeed;

	float StartAlpha;

	int FadeIn;

}Particle;

ParticleSystem* CreateFoxParticleSystem(char* particleTexture, float posX, float posY, int ZIndex, int amountTotal, int emitAmount, float emitSpeed, int emitAngle, int emitAngleRandom, float emitScale, float emitScaleSpeed, int emitDisplacementX, int emitDisplacementY, float emitVelocity, float emitLife, float StartAlpha);

void CreateFoxParticle(char* particleTexture, AEGfxVertexList* mesh, float posX, float posY, int ZIndex, float VelX, float VelY, float Life, float Scale, float ScaleSpeed, float StartAlpha, int FadeIn);

void ParticleSystemUpdate(void);
void ParticleUpdate(void);

#endif