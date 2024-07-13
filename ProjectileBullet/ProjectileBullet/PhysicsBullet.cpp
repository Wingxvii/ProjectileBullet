#include "PhysicsBullet.h"

APhysicsBullet::APhysicsBullet()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void APhysicsBullet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APhysicsBullet, _currentPos);
	DOREPLIFETIME(APhysicsBullet, _currentVel);
	DOREPLIFETIME(APhysicsBullet, _currentDir);
}

void APhysicsBullet::Reset()
{
	Super::Reset();

	authority_windDir = FVector::ZeroVector;
	authority_windVel = 0.f;
	_currentPos = FVector::ZeroVector;
	_currentVel = FVector::ZeroVector;
}

void APhysicsBullet::Authority_SetWind(FVector dir, float velocity)
{
	authority_windVel = velocity;
	authority_windDir = dir;
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
		if (HasAuthority())
		{
			// Update Velocity
			_currentVel += acceleration * GetActorForwardVector();
			_currentVel += gravity * FVector::VectorUp;
			_currentVel += authority_windDir * authority_windVel;

			// Update Position
			_currentPos += _currentVel;
		}
		else
		{
			// This will get corrected by replication
			_currentPos += _currentVel;
		}
	}
	else
	{
		OnLifetimeEnded();
	}

}

void APhysicsBullet::Authority_OnShot(AActor* shooter, FVector shotOrigin, FVector shotRotation)
{
	SetActorTickEnabled(true);
	Client_OnShot();

	_currentPos = shotOrigin;
	_currentVel = shotRotation * initVelocity;
}

void APhysicsBullet::OnLifetimeEnded()
{
	SetActorTickEnabled(false);
}

void APhysicsBullet::Authority_OnPenetrate(UBulletHittableComponent* hittable)
{
	if (hittable->GetPenetrationScore() != -1 && penetrationScore != -1)
	{
		currPenetrationScore -= hittable->GetPenetrationScore();
	}
	// We do nothing with the shot, let it continue
}

void APhysicsBullet::Authority_OnRicochet(UBulletHittableComponent* hittable)
{
	FVector reflectedVect = UKismetLibrary::GetReflectionVector(UKismetLibrary::Normalize(_currentVel), _hitNormal);

	_currentVel = (_currentVel.Size() * elasticity) * reflectedVect;
}

void APhysicsBullet::Client_OnShot()
{
	SetActorTickEnabled(true);
}
