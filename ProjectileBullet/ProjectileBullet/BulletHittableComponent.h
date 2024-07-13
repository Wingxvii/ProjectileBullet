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

	virtual void OnBroken();

	virtual EHitResult OnHit(int damage, int penetration, int ricochet);

	int GetPenetrationScore() const;
	int GetRicochetScore() const;

protected:
	virtual void UpdateBroken();

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

