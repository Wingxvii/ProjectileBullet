#include "UExplosiveComponent.h"

UExplosiveComponent::UExplosiveComponent()
{
}

void UExplosiveComponent::BeginPlay()
{
	Super::BeginPlay();

	// register to OnHit Event from owning ABullet
}

void UExplosiveComponent::OnHit(AActor* hitActor)
{

}
