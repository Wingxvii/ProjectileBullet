#pragma once

#include "ActorComponent.h"

UENUM()
enum class EHitResult : uint8
{
	None,
	Penetrate,
	Ricochet
};

/*
* Component used to react to a Bullet Hit
*
*/
UCLASS()
class UBulletHittableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBulletHittableComponent();

	// Called when hittable loses all its health
	virtual void Authority_OnBroken();

	// Called when hittable is hit; returns result of hit
	virtual EHitResult Authority_OnHit(int damage, int penetration, int ricochet);

	int Authority_GetPenetrationScore() const;
	int Authority_GetRicochetScore() const;

protected:
	virtual void Authority_UpdateBroken();

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	int baseHealth = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	int penetrationScore = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	int ricochetScore = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	bool shouldPenetrateOnBroken = false;

private:
	int currentHealth = -1;
};

