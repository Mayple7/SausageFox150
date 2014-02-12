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


ParticleSystem* CreateFoxParticleSystem(char* particleTexture, float posX, float posY, int ID, int amountTotal, int emitAmount, float emitSpeed, int emitAngle, int emitAngleRandom, float emitScale, int emitDisplacementX, int emitDisplacementY, float emitVelocity, float emitLife)
{
	ParticleSystem *CurrentSystem = AddParticleSystem();

	CurrentSystem->ParticleSprite = particleTexture;
	CurrentSystem->Position.x = posX;
	CurrentSystem->Position.y = posY;
	CurrentSystem->objID = ID;
	CurrentSystem->amountTotal = amountTotal;
	CurrentSystem->emitAmount = emitAmount;
	CurrentSystem->emitSpeed = emitSpeed;
	CurrentSystem->emitSpeedTimer = emitSpeed;
	CurrentSystem->emitAngle = emitAngle;
	CurrentSystem->emitAngleRandom = emitAngleRandom;
	CurrentSystem->emitScale = emitScale;
	CurrentSystem->emitDisplacementX = emitDisplacementX;
	CurrentSystem->emitDisplacementY = emitDisplacementY;
	CurrentSystem->emitVelocity = emitVelocity;
	CurrentSystem->emitLife = emitLife;
	return CurrentSystem;
}

void CreateFoxParticle(char* particleTexture, float posX, float posY, float VelX, float VelY, float Life, float Scale, int ID)
{
	Particle *CurrentParticle = AddParticle();

	CurrentParticle->ParticleSprite = CreateSprite(particleTexture, 64.0f, 64.0f, 100, 1, 1, posX, posY);
	CurrentParticle->Position.x = posX;
	CurrentParticle->Position.y = posY;
	CurrentParticle->Velocity.x = VelX;
	CurrentParticle->Velocity.y = VelY;
	CurrentParticle->Lifetime = Life;
	CurrentParticle->LifetimeMax = Life;
	CurrentParticle->ParticleSprite->ScaleX = Scale;
	CurrentParticle->ParticleSprite->ScaleY = Scale;
	CurrentParticle->objID = ID;
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
			particleList[i].ParticleSprite->Alpha = particleList[i].Lifetime / particleList[i].LifetimeMax;

		particleList[i].ParticleSprite->ScaleX *= 0.995f;
		particleList[i].ParticleSprite->ScaleY *= 0.995f;

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

		if (AEInputCheckTriggered(VK_UP))
		{
			particleSystemList[i].amountTotal += 100;
		}
		if (particleSystemList[i].emitSpeedTimer <= 0)
		{
			int j;
			for(j = 0; j < particleSystemList[i].emitAmount; j++)
			{
				if (particleSystemList[i].amountTotal > 0)
				{
					Vec2 vel;
					float diff = (float)((int)lastRandomNumber % 100) / 100.0f + 0.5f;
					vel.x = particleSystemList[i].emitVelocity * diff;
					vel.y = 0;

					if (particleSystemList[i].emitAngleRandom == 0)
						particleSystemList[i].emitAngleRandom += 1;
					if (particleSystemList[i].emitDisplacementX == 0)
						particleSystemList[i].emitDisplacementX += 1;
					if (particleSystemList[i].emitDisplacementY == 0)
						particleSystemList[i].emitDisplacementY += 1;

					srand( lastRandomNumber );
					lastRandomNumber = rand();
					Vec2RotateDegrees(&vel, particleSystemList[i].emitAngle + ((float)((int)lastRandomNumber % particleSystemList[i].emitAngleRandom - (particleSystemList[i].emitAngleRandom/2))));

					CreateFoxParticle(	particleSystemList[i].ParticleSprite, 
										particleSystemList[i].Position.x + ((float)((int)lastRandomNumber % particleSystemList[i].emitDisplacementX - (particleSystemList[i].emitDisplacementX/2))),
										particleSystemList[i].Position.y + ((float)((int)lastRandomNumber % particleSystemList[i].emitDisplacementY - (particleSystemList[i].emitDisplacementY/2))), 
										vel.x,
										vel.y, 
										particleSystemList[i].emitLife, 
										particleSystemList[i].emitScale * (1 + (float)((int)lastRandomNumber % 50) / 100.0f - 0.25f), 
										1);

					particleSystemList[i].amountTotal--;
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