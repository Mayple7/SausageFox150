/*****************************************************************************/
/*!
\file				Enemy.c
\author				Luke Conlon (l.conlon)
\date				Feb 10, 2014

\brief				Functions for the particle system.

\par				Functions:
\li					InitializeEnemy
\li					UpdateEnemy
\li					EnemyLogic
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#include "../HeaderFiles/ParticleSystem.h"
#include "../HeaderFiles/ObjectManager.h"
#include "../HeaderFiles/FrameRate.h"
#include <stdlib.h>
#include <time.h>


ParticleSystem* CreateFoxParticleSystem(char* particleTexture, float posX, float posY, int ZIndex, int amountTotal, int emitAmount, float emitSpeed, int emitAngle, int emitAngleRandom, float emitScale, float emitScaleSpeed, int emitDisplacementX, int emitDisplacementY, float emitVelocity, float emitLife, float StartAlpha)
{
	ParticleSystem *CurrentSystem = AddParticleSystem();

	strcpy(CurrentSystem->ParticleSprite, particleTexture);
	CurrentSystem->emitMesh = createMesh(64.0f * GetLoadRatio(), 64.0f * GetLoadRatio(), 1, 1);
	CurrentSystem->Position.x = posX;
	CurrentSystem->Position.y = posY;
	CurrentSystem->objID = 1;
	CurrentSystem->amountTotal = amountTotal;
	CurrentSystem->emitAmount = emitAmount;
	CurrentSystem->emitSpeed = emitSpeed;
	CurrentSystem->emitSpeedTimer = emitSpeed;
	CurrentSystem->emitAngle = emitAngle;
	CurrentSystem->emitAngleRandom = emitAngleRandom;
	CurrentSystem->emitScale = emitScale;
	CurrentSystem->emitScaleSpeed = emitScaleSpeed;
	CurrentSystem->emitDisplacementX = emitDisplacementX;
	CurrentSystem->emitDisplacementY = emitDisplacementY;
	CurrentSystem->emitVelocity = emitVelocity;
	CurrentSystem->emitLife = emitLife;
	CurrentSystem->ZIndex = ZIndex;
	CurrentSystem->ParticleStartAlpha = StartAlpha;
	return CurrentSystem;
}

void CreateFoxParticle(char* particleTexture, AEGfxVertexList* mesh, float posX, float posY, int ZIndex, float VelX, float VelY, float Life, float Scale, float ScaleSpeed, float StartAlpha)
{
	Particle *CurrentParticle = AddParticle();

	CurrentParticle->ParticleSprite = CreateSpriteNoMesh(particleTexture, 64.0f, 64.0f, ZIndex, 1, 1, posX, posY);
	CurrentParticle->ParticleSprite->SpriteMesh = mesh;
	CurrentParticle->Position.x = posX * GetLoadRatio();
	CurrentParticle->Position.y = posY * GetLoadRatio();
	CurrentParticle->Velocity.x = VelX;
	CurrentParticle->Velocity.y = VelY;
	CurrentParticle->Lifetime = Life;
	CurrentParticle->LifetimeMax = Life;
	CurrentParticle->ParticleSprite->ScaleX = Scale * GetLoadRatio();
	CurrentParticle->ParticleSprite->ScaleY = Scale * GetLoadRatio();
	CurrentParticle->ScaleSpeed = ScaleSpeed;
	CurrentParticle->objID = 1;
	CurrentParticle->StartAlpha = StartAlpha;
	srand( lastRandomNumber );
	lastRandomNumber = rand();
	CurrentParticle->RotationSpeed = (float)(((int)lastRandomNumber % 100 - 50) / 400.0f);

	srand( lastRandomNumber );
	lastRandomNumber = rand();
	CurrentParticle->ParticleSprite->Rotation = (float)((int)lastRandomNumber % 360) / 180.0f * (float)FOX_PI;

}

void ParticleUpdate(void)
{
	int i;
	for (i = 0; i < PARTICLEAMOUNT; i++)
	{
		if (particleList[i].objID == 0)
		{
			continue;
		}
		else if (particleList[i].objID == -1)
		{
			break;
		}

		particleList[i].Position.x += particleList[i].Velocity.x * GetDeltaTime();
		particleList[i].Position.y += particleList[i].Velocity.y * GetDeltaTime();

		particleList[i].ParticleSprite->Position.x = particleList[i].Position.x;
		particleList[i].ParticleSprite->Position.y = particleList[i].Position.y;

		particleList[i].ParticleSprite->Rotation += particleList[i].RotationSpeed;
		particleList[i].Lifetime -= GetDeltaTime();

		if (particleList[i].LifetimeMax != 0)
			particleList[i].ParticleSprite->Alpha = particleList[i].StartAlpha * (particleList[i].Lifetime / particleList[i].LifetimeMax);

		particleList[i].ParticleSprite->ScaleX *= (1 + (particleList[i].ScaleSpeed / 1000.0f));
		particleList[i].ParticleSprite->ScaleY *= (1 + (particleList[i].ScaleSpeed / 1000.0f));

		if (particleList[i].Lifetime <= 0)
		{
			particleList[i].objID = 0;
			freeObject(particleList[i].ParticleSprite);
		}
	}
}

void ParticleSystemUpdate(void)
{
	int i;
	for (i = 0; i < PARTICLESYSTEMAMOUNT; i++)
	{
		
		if (particleSystemList[i].objID == 0)
		{
			continue;
		}
		else if (particleSystemList[i].objID == -1)
		{
			break;
		}

		if (particleSystemList[i].emitSpeedTimer <= 0)
		{
			int j;
			for(j = 0; j < particleSystemList[i].emitAmount; j++)
			{
				if (particleSystemList[i].amountTotal > 0 || particleSystemList[i].amountTotal == -1)
				{
					Vec2 vel;


					float diff;

					diff = (float)((int)lastRandomNumber % 100) / 100.0f + 0.5f;

					vel.x = particleSystemList[i].emitVelocity * diff;
					vel.y = 0;

					if (particleSystemList[i].emitAngleRandom == 0)
						particleSystemList[i].emitAngleRandom++;
					if (particleSystemList[i].emitDisplacementX == 0)
						particleSystemList[i].emitDisplacementX++;
					if (particleSystemList[i].emitDisplacementY == 0)
						particleSystemList[i].emitDisplacementY++;

					srand( lastRandomNumber );
					lastRandomNumber = rand();
					Vec2RotateDegrees(&vel, particleSystemList[i].emitAngle + ((float)((int)lastRandomNumber % particleSystemList[i].emitAngleRandom - (particleSystemList[i].emitAngleRandom/2))));

					CreateFoxParticle(	particleSystemList[i].ParticleSprite, 
										particleSystemList[i].emitMesh,
										particleSystemList[i].Position.x + ((float)((int)lastRandomNumber % particleSystemList[i].emitDisplacementX - (particleSystemList[i].emitDisplacementX/2))),
										particleSystemList[i].Position.y + ((float)((int)lastRandomNumber % particleSystemList[i].emitDisplacementY - (particleSystemList[i].emitDisplacementY/2))), 
										particleSystemList[i].ZIndex,
										vel.x,
										vel.y, 
										particleSystemList[i].emitLife, 
										particleSystemList[i].emitScale * (1 + (float)((int)lastRandomNumber % 50) / 100.0f - 0.25f),
										particleSystemList[i].emitScaleSpeed,
										particleSystemList[i].ParticleStartAlpha);

					if (particleSystemList[i].amountTotal > 0)
						particleSystemList[i].amountTotal--;
					srand( rand() % (rand() + 1) );
					lastRandomNumber = rand();
				}
			}
			particleSystemList[i].emitSpeedTimer = particleSystemList[i].emitSpeed;
		}
		else
		{
			particleSystemList[i].emitSpeedTimer -= GetDeltaTime();
		}
	}

	ParticleUpdate();

	
	
}