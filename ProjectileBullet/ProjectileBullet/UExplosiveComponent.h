#pragma once

#include "ActorComponent.h"
#include "Bullet.h"

UENUM(BlueprintableType)
enum class EExplosionTrigger : uint8
{
	ExplodeOnHit,
	ExplodeOnLifetimeEnd,
	ExplodeOnNonRicochetOrPenetrate		// This will explode only when a hit is not Ricochet or Penetrate
};

UCLASS()
class UExplosiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UExplosiveComponent();

	virtual void BeginPlay() override;

protected:
	UFUNCTION();
	void OnHit(AActor* hitActor);

	// Reason for explosion trigger
	UPROPERTY(EditDefaultsOnly, Category = "Properties");
	EExplosionTrigger trigger = EExplosionTrigger::ExplodeOnHit;

	// Radius of explosion in Meters
	UPROPERTY(EditDefaultsOnly, Category = "Properties");
	float explosionRadius = 1.0f;

	// Damage dealt by explosion (Additive to direct hit damage)
	UPROPERTY(EditDefaultsOnly, Category = "Properties");
	int explosionDamage = -1;

private:

	
};

