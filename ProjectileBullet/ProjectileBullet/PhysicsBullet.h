#pragma once

#include "Bullet.h"

/*
* Physics Bullet
*/

UCLASS()
class APhysicsBullet : public ABullet
{
	GENERATED_BODY()

public:
	APhysicsBullet();

	// Resets bullet runtime variables (Does not reset properties)
	virtual void Reset() override;

	// Updates the wind for the bullet
	void SetWind(FVector dir, float velocity);

	// Called externally by collision system
	void OnCollisionHit(AActor* hitActor, FImpactInfo impactInfo);

	virtual void Tick(float deltaSeconds) override;

	// Called when bullet is fired
	virtual void Authority_OnShot(AActor* shooter, FVector shotOrigin, FVector shotRotation) override;

	// Called if nothing is hit before lifetime ends
	virtual void OnLifetimeEnded();

	// Called if bullet penetrates hittable
	virtual void Authority_OnPenetrate(UBulletHittableComponent* hittable) override;

	// Called if bullet Ricochets off hittable
	virtual void Authority_OnRicochet(UBulletHittableComponent* hittable) override;

	UFUNCTION(Client, Reliable)
	void Client_OnShot();

protected:

	// Initial Velocity, this is how fast the projectile should be fired at
	UPROPERTY(EditDefaultsOnly, Category = "Physics Properties")
	float initVelocity = 1.0f;

	// Acceleration, set to negative if we want bullets to slow down over time
	UPROPERTY(EditDefaultsOnly, Category = "Physics Properties")
	float acceleration = 0.0f;

	// Gravity (should be negative)
	UPROPERTY(EditDefaultsOnly, Category = "Physics Properties")
	float gravity = -1.f;

	// Lifetime before bullet stops 
	UPROPERTY(EditDefaultsOnly, Category = "Physics Properties")
	float lifetime = -1.f;

	// How much velocity is conserved upon ricochet
	UPROPERTY(EditDefaultsOnly, Category = "Physics Properties")
	float elasticity = 1.f;

private:
	FVector authority_windDir = FVector::ZeroVector;
	float authority_windVel = 0.f;

	UPROPERTY(Replicated)
	FVector _currentPos = FVector::ZeroVector;

	UPROPERTY(Replicated)
	FVector _currentVel = FVector::ZeroVector;

	UPROPERTY(Replicated)
	FVector _currentDir = FVector::ZeroVector;

};