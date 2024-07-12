#include "Bullet.h"

ABullet::ABullet()
	: Super()
{
}

AActor* ABullet::GetShooter() const
{
	return _shooter;
}

void ABullet::OnShot()
{
	// First send a raycast
	FCollisionQueryParams queryParams;
	FHitResult outHit;
	// Send raycast to all hittable colliders
	_hit = GetWorld()->LineTraceSingleByChannel(outHit, _initPos, _initPos + _initDir, ECC_Hittable, queryParams);

	if (_hit)
	{
		_hitPos = outHit.Location;
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
}

void ABullet::OnMissed()
{
}

void ABullet::Reset()
{
	_initPos = FVector::ZeroVector;
	_initDir = FVector::ZeroVector;
	_hitPos = FVector::ZeroVector;

	_shooter = nullptr;
	_hitActor = nullptr;
}
