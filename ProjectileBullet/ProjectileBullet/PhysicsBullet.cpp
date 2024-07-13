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

void APhysicsBullet::OnCollisionHit(AActor* hitActor, FImpactInfo impactInfo)
{
	if (ensure(IsValid(hitActor)))
	{
		_hitActor = hitActor;
		_hitPos = impactInfo.Location;
		_hitNormal = impactInfo.ImpactNormal;

		OnHit(hitActor);
	}
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

void APhysicsBullet::OnLifetimeEnded()
{
	SetActorTickEnabled(false);
}

void APhysicsBullet::OnPenetrate(UBulletHittableComponent* hittable)
{
	if (hittable->GetPenetrationScore() != -1 && penetrationScore != -1)
	{
		currPenetrationScore -= hittable->GetPenetrationScore();
	}
	// We do nothing with the shot, let it continue
}

void APhysicsBullet::OnRicochet(UBulletHittableComponent* hittable)
{
	FVector reflectedVect = UKismetLibrary::GetReflectionVector(UKismetLibrary::Normalize(_currentVel), _hitNormal);

	_currentVel = (_currentVel.Size() * elasticity) * reflectedVect;
}