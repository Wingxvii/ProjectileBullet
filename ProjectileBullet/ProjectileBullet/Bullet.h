# pragma once

#include "Actor.h"

class UBulletHittableComponent;

/*
* Hitscan Bullet Base
*/
UCLASS()
class ABullet : public AActor
{
	GENERATED_BODY()

public:
	ABullet();

	// Resets bullet runtime variables (Does not reset properties)
	virtual void Reset();

	// Returns actor who shot this bullet
	AActor* GetShooter() const;

	// Called when bullet is fired
	virtual void OnShot(AActor* shooter, FVector shotOrigin, FVector shotRotation);

	// Called when anything is hit by bullet
	virtual void OnHit(AActor* hitActor);

	// Called if nothing is hit
	virtual void OnMissed();

	// Called if bullet penetrates hittable
	virtual void OnPenetrate(UBulletHittableComponent* hittable);

	// Called if bullet Ricochets off hittable
	virtual void OnRicochet(UBulletHittableComponent* hittable);

protected:
	//damage value before any dropoff
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float baseDamage = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	int penetrationScore = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	int ricochetScore = 0;

private:
	TWeakObjectPtr<AActor> _shooter = nullptr;
	TWeakObjectPtr<AActor> _hitActor = nullptr;

	FVector _initDir = FVector::ZeroVector;
	FVector _hitPos = FVector::ZeroVector;
	FVector _hitNormal = FVector::ZeroVector;

	int currPenetrationScore = 0;
	int currRicochetScore = 0;

	bool _hit = false;
};