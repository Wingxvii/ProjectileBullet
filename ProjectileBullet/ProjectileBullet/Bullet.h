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
	virtual void Authority_OnShot(AActor* shooter, FVector shotOrigin, FVector shotRotation);

	// Called when anything is hit by bullet
	virtual void Authority_OnHit(AActor* hitActor);

	// Called if nothing is hit
	virtual void OnMissed();

	// Called if bullet penetrates hittable
	virtual void Authority_OnPenetrate(UBulletHittableComponent* hittable);

	// Called if bullet Ricochets off hittable
	virtual void Authority_OnRicochet(UBulletHittableComponent* hittable);

protected:
	//damage value before any dropoff
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float baseDamage = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	int penetrationScore = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	int ricochetScore = 0;

private:
	UPROPERTY(Replicated)
	TWeakObjectPtr<AActor> _shooter = nullptr;

	UPROPERTY(Replicated)
	TWeakObjectPtr<AActor> _hitActor = nullptr;

	UPROPERTY(Replicated)
	FVector _initDir = FVector::ZeroVector;

	UPROPERTY(Replicated)
	FVector _hitPos = FVector::ZeroVector;

	FVector authority_hitNormal = FVector::ZeroVector;

	int authority_currPenetrationScore = 0;
	int authority_currRicochetScore = 0;

	bool _hit = false;
};