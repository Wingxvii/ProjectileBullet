#include "BulletHittableComponent.h"

UBulletHittableComponent::UBulletHittableComponent()
{
}

void UBulletHittableComponent::Authority_OnBroken()
{
	if (shouldPenetrateOnBroken)
	{
		penetrationScore = 0;
	}
}

EHitResult UBulletHittableComponent::Authority_OnHit(int damage, int penetration, int ricochet)
{
	currentHealth -= damage;
	UpdateBroken();

	float penetrationDelta = penetration - penetrationScore;
	float ricochetDelta = ricochet - ricochetScore;

	if (penetration == -1  
		|| penetrationScore == -1
		|| (penetrationDelta >= 0.f && penetrationDelta >= ricochetDelta))
	{
		return EHitResult::Penetrate;
	}
	else if (ricochet == -1 
		|| ricochetScore == -1
		|| ricochetDelta >= 0.f)
	{
		return EHitResult::Ricochet;
	}

	return EHitResult::None;
}

int UBulletHittableComponent::Authority_GetPenetrationScore() const
{
	return penetrationScore;
}

int UBulletHittableComponent::Authority_GetRicochetScore() const
{
	return ricochetScore;
}

void UBulletHittableComponent::Authority_UpdateBroken()
{
	if (currentHealth <= 0)
	{
		Authority_OnBroken();
	}
}
