# pragma once

#include "Actor.h"

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
	virtual void OnShot();

	// Called when anything is hit by bullet
	virtual void OnHit(AActor* hitActor);

	// Called if nothing is hit
	virtual void OnMissed();

protected:
	//damage value before any dropoff
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float baseDamage = 0.f;

	//0-1 percent accuracy for shot path
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float accuracty = 0.f;

	//Distance for the raycast (m)
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float maxRange = 1000.f;

private:
	TWeakObjectPtr<AActor> _shooter = nullptr;
	TWeakObjectPtr<AActor> _hitActor = nullptr;

	FVector _initPos = FVector::ZeroVector;
	FVector _initDir = FVector::ZeroVector;
	FVector _hitPos = FVector::ZeroVector;

	bool _hit = false;
};