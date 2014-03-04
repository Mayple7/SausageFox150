#include "../AEEngine.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxObjects.h"

void EasyEditSprite(Sprite *obj, int mult)
{
	if(FoxInput_KeyTriggered(VK_UP))
		obj->Position.y += 1 * GetLoadRatio() * mult;
	if(FoxInput_KeyTriggered(VK_DOWN))
		obj->Position.y -= 1 * GetLoadRatio() * mult;
	if(FoxInput_KeyTriggered(VK_LEFT))
		obj->Position.x -= 1 * GetLoadRatio() * mult;
	if(FoxInput_KeyTriggered(VK_RIGHT))
		obj->Position.x += 1 * GetLoadRatio() * mult;

	if(FoxInput_KeyTriggered('P'))
	{
		printf("OBJ X : %f\n", obj->Position.x / GetLoadRatio());
		printf("OBJ Y : %f\n", obj->Position.y / GetLoadRatio());
	}

}

void EasyEditPlatform(Platform *obj, int mult)
{
	if(FoxInput_KeyTriggered(VK_UP))
		obj->PlatformCollider.Position.y += 1 * GetLoadRatio() * mult;
	if(FoxInput_KeyTriggered(VK_DOWN))
		obj->PlatformCollider.Position.y -= 1 * GetLoadRatio() * mult;
	if(FoxInput_KeyTriggered(VK_LEFT))
		obj->PlatformCollider.Position.x -= 1 * GetLoadRatio() * mult;
	if(FoxInput_KeyTriggered(VK_RIGHT))
		obj->PlatformCollider.Position.x += 1 * GetLoadRatio() * mult;

	if(FoxInput_KeyTriggered('P'))
	{
		printf("OBJ X : %f\n", obj->PlatformCollider.Position.x / GetLoadRatio());
		printf("OBJ Y : %f\n", obj->PlatformCollider.Position.y) / GetLoadRatio();
	}
}


void EasyEditWall(Wall *obj, int mult)
{
	if(FoxInput_KeyTriggered(VK_UP))
		obj->WallCollider.Position.y += 1 * GetLoadRatio() * mult;
	if(FoxInput_KeyTriggered(VK_DOWN))
		obj->WallCollider.Position.y -= 1 * GetLoadRatio() * mult;
	if(FoxInput_KeyTriggered(VK_LEFT))
		obj->WallCollider.Position.x -= 1 * GetLoadRatio() * mult;
	if(FoxInput_KeyTriggered(VK_RIGHT))
		obj->WallCollider.Position.x += 1 * GetLoadRatio() * mult;

	if(FoxInput_KeyTriggered('P'))
	{
		printf("OBJ X : %f\n", obj->WallCollider.Position.x / GetLoadRatio());
		printf("OBJ Y : %f\n", obj->WallCollider.Position.y / GetLoadRatio());
	}
}
