#include "Bullet.h"
#include "BulletHittableComponent.h"

ABullet::ABullet()
	: Super()
{
}

AActor* ABullet::GetShooter() const
{
	return _shooter;
}

void ABullet::OnShot(AActor* shooter, FVector shotOrigin, FVector shotRotation)
{
	if (ensure(IsValid(shooter)))
	{
		_shooter = shooter;
	}

	_initDir = shotRotation;

	// First send a raycast
	FCollisionQueryParams queryParams;
	FHitResult outHit;
	// Send raycast to all hittable colliders
	_hit = GetWorld()->LineTraceSingleByChannel(outHit, shotOrigin, shotOrigin + _initDir, ECC_Hittable, queryParams);

	if (_hit)
	{
		_hitPos = outHit.Location;
		_hitNormal = outHit.ImpactNormal;

		if (IsValid(outHit.GetActor))
		{
			_hitActor = outHit.GetActor;
			OnHit(_hitActor);
		}
	}
	else
	{
		OnMissed();
	}
}

void ABullet::OnHit(AActor* hitActor)
{
	EHitResult result = EHitResult::None;
	UBulletHittableComponent* hitHittable = hitActor->GetComponentByClass<UBulletHittableComponent>();
	if (IsValid(hitHittable))
	{
		result = hitHittable->OnHit(baseDamage, currPenetrationScore, currRicochetScore);
	}

	switch (result)
	{
	case EHitResult::Penetrate:
		OnPenetrate(hitHittable);
		break;
	case EHitResult::Ricochet:
		OnRicochet(hitHittable);
		break;
	case EHitResult::None:
	default:

		Reset();
		break;
	}
}

void ABullet::OnMissed()
{
}

void ABullet::OnPenetrate(UBulletHittableComponent* hittable)
{
	if (hittable->GetPenetrationScore() != -1 && penetrationScore != -1)
	{
		currPenetrationScore -= hittable->GetPenetrationScore();
	}

	OnShot(_shooter, _hitPos, _initDir);
}

void ABullet::OnRicochet(UBulletHittableComponent* hittable)
{
	if (hittable->GetRicochetScore() != -1 && ricochetScore != -1)
	{
		currRicochetScore -= hittable->GetRicochetScore();
	}
	FVector reflectedVect = UKismetLibrary::GetReflectionVector(_initDir, _hitNormal);
	OnShot(_shooter, _hitPos, reflectedVect);
}

void ABullet::Reset()
{
	_hitPos = FVector::ZeroVector;
	_initDir = FVector::ZeroVector;
	_hitNormal = FVector::ZeroVector;

	_shooter = nullptr;
	_hitActor = nullptr;

	currPenetrationScore = penetrationScore;
	currRicochetScore = ricochetScore;
}
