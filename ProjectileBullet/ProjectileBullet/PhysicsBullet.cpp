#include "PhysicsBullet.h"

APhysicsBullet::APhysicsBullet()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void APhysicsBullet::Reset()
{
	Super::Reset();

	_windDir = FVector::ZeroVector;
	_windVel = 0.f;
	_currentPos = FVector::ZeroVector;
	_currentVel = FVector::ZeroVector;
}

void APhysicsBullet::SetWind(FVector dir, float velocity)
{
	_windVel = velocity;
	_windDir = dir;
}

void APhysicsBullet::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	lifetime -= deltaSeconds;
	if (lifetime > 0.f)
	{
		// Update Velocity
		_currentVel += acceleration * GetActorForwardVector();
		_currentVel += gravity * FVector::VectorUp;

		// Update Position
		_currentPos += _currentVel;
	}
	else
	{
		OnLifetimeEnded();
	}
}

void APhysicsBullet::OnShot(AActor* shooter, FVector shotOrigin, FVector shotRotation)
{
	SetActorTickEnabled(true);

	_currentPos = shotOrigin;
	_currentVel = shotRotation * initVelocity;
}

void APhysicsBullet::OnHit(AActor* hitActor)
{
	SetActorTickEnabled(false);
}

void APhysicsBullet::OnLifetimeEnded()
{
	SetActorTickEnabled(false);
}

void APhysicsBullet::OnPenetrate(UBulletHittableComponent* hittable)
{
	Super::OnPenetrate(hittable);


}

void APhysicsBullet::OnRicochet(UBulletHittableComponent* hittable)
{
	Super::OnRicochet(hittable);
}
