
#include "Components/CStatusComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"


UCStatusComponent::UCStatusComponent()
{

}

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

    OwnerCharacter = Cast<ACharacter>(GetOwner());
    Health = MaxHealth;
	
}

void UCStatusComponent::Damage(float InAmount)
{
    Health += (InAmount*-1.0f);
    Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}

void UCStatusComponent::SetHealth(float InHealth)
{
	Health = FMath::Clamp(InHealth, 0.0f, MaxHealth);
}
void UCStatusComponent::RestoreHealth(float InAmount)
{
	Health = FMath::Clamp(Health + InAmount, 0.0f, MaxHealth);
}