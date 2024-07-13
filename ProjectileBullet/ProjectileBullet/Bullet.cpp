#include "Bullet.h"
#include "BulletHittableComponent.h"

ABullet::ABullet()
	: Super()
{
}

void ABullet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABullet, _shooter);
	DOREPLIFETIME(ABullet, _hitActor);
	DOREPLIFETIME(ABullet, _initDir);
	DOREPLIFETIME(ABullet, _hitPos);
}

AActor* ABullet::GetShooter() const
{
	return _shooter;
}

void ABullet::Authority_OnShot(AActor* shooter, FVector shotOrigin, FVector shotRotation)
{
	if (ensure(HasAuthority()))
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
			authority_hitNormal = outHit.ImpactNormal;

			if (IsValid(outHit.GetActor))
			{
				_hitActor = outHit.GetActor;
				Authority_OnHit(_hitActor);
			}
		}
		else
		{
			OnMissed();
		}
	}
}

void ABullet::Authority_OnHit(AActor* hitActor)
{
	EHitResult result = EHitResult::None;
	UBulletHittableComponent* hitHittable = hitActor->GetComponentByClass<UBulletHittableComponent>();
	if (IsValid(hitHittable))
	{
		result = hitHittable->Authority_OnHit(baseDamage, currPenetrationScore, currRicochetScore);
	}

	switch (result)
	{
	case EHitResult::Penetrate:
		Authority_OnPenetrate(hitHittable);
		break;
	case EHitResult::Ricochet:
		Authority_OnRicochet(hitHittable);
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

void ABullet::Authority_OnPenetrate(UBulletHittableComponent* hittable)
{
	if (ensure(HasAuthority()))
	{
		if (hittable->GetPenetrationScore() != -1 && penetrationScore != -1)
		{
			_currPenetrationScore -= hittable->GetPenetrationScore();
		}

		Authority_OnShot(_shooter, _hitPos, _initDir);
	}
}

void ABullet::Authority_OnRicochet(UBulletHittableComponent* hittable)
{
	if (ensure(HasAuthority()))
	{
		if (hittable->GetRicochetScore() != -1 && ricochetScore != -1)
		{
			_currRicochetScore -= hittable->GetRicochetScore();
		}
		FVector reflectedVect = UKismetLibrary::GetReflectionVector(_initDir, authority_hitNormal);
		Authority_OnShot(_shooter, _hitPos, reflectedVect);
	}
}

void ABullet::Reset()
{
	_hitPos = FVector::ZeroVector;
	_initDir = FVector::ZeroVector;
	_hitNormal = FVector::ZeroVector;

	_shooter = nullptr;
	_hitActor = nullptr;

	_currPenetrationScore = penetrationScore;
	_currRicochetScore = ricochetScore;
}
