#include "PhysicsBullet.h"

APhysicsBullet::APhysicsBullet()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void APhysicsBullet::Reset()
{
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

void APhysicsBullet::OnShot()
{
	SetActorTickEnabled(true);

	_currentPos = _initPos;
	_currentVel = _initDir * initVelocity;
}

void APhysicsBullet::OnHit(AActor* hitActor)
{
	SetActorTickEnabled(false);
}

void APhysicsBullet::OnLifetimeEnded()
{
	SetActorTickEnabled(false);
}
