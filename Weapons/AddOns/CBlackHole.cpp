// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AddOns/CBlackHole.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACBlackHole::ACBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Sphere);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Explosion, "Explosion", Sphere);

	Explosion->SetVisibility(false);


	Sphere->SetSphereRadius(1000.0f);

	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	Sphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	AttractionStrength = 2000.0f;
	ExplosionStrength = 5000.0f; // ���� ���� �ʱ�ȭ

}

// Called when the game starts or when spawned
void ACBlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACBlackHole::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Explode();
}

// Called every frame
void ACBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AttractObjects();

}

void ACBlackHole::AttractObjects()
{
	TArray<AActor*> OverlappingActors;
	Sphere->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor != this)
		{
			UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
			if (PrimComp && PrimComp->IsSimulatingPhysics())
			{
				FVector Direction = GetActorLocation() - Actor->GetActorLocation();
				Direction.Normalize();
				PrimComp->AddForce(Direction * AttractionStrength * PrimComp->GetMass());
				UE_LOG(LogTemp, Warning, TEXT("Applying Force to Physics Actor: %s"), *Actor->GetName());
			}
			else if (ACharacter* Character = Cast<ACharacter>(Actor))
			{
				FVector Direction = GetActorLocation() - Character->GetActorLocation();
				Direction.Normalize();
				UCharacterMovementComponent* CharMoveComp = Character->GetCharacterMovement();
				if (CharMoveComp)
				{
					CharMoveComp->AddForce(Direction * AttractionStrength * Character->GetCharacterMovement()->Mass);
					UE_LOG(LogTemp, Warning, TEXT("Applying Force to Character: %s"), *Actor->GetName());
				}
			}

		}
	}


}

void ACBlackHole::Explode()
{
	// ���� ȿ�� ����
	if (Explosion)
	{
		Explosion->SetVisibility(true);
	}

	// ���͵��� �ָ� ���� ������
	TArray<AActor*> OverlappingActors;
	Sphere->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor != this)
		{
			UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
			if (PrimComp && PrimComp->IsSimulatingPhysics())
			{
				FVector Direction = Actor->GetActorLocation() - GetActorLocation();
				Direction.Normalize();
				PrimComp->AddImpulse(Direction * ExplosionStrength * PrimComp->GetMass());
				UE_LOG(LogTemp, Warning, TEXT("Applying Explosion Force to Physics Actor: %s"), *Actor->GetName());
			}
		}
		else if (ACharacter* Character = Cast<ACharacter>(Actor))
		{
				// ���� �ùķ��̼� Ȱ��ȭ
				Character->GetCapsuleComponent()->SetSimulatePhysics(true);

			FVector Direction = Actor->GetActorLocation() - GetActorLocation();
			Direction.Normalize();
			Character->GetMesh()->AddImpulse(Direction * ExplosionStrength * Character->GetMesh()->GetMass());
			UE_LOG(LogTemp, Warning, TEXT("Applying Explosion Force to Character: %s"), *Actor->GetName());

			// ���� �ð� �� ���� �ùķ��̼� ��Ȱ��ȭ (���� ����)
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [Character]()
				{
					Character->GetMesh()->SetSimulatePhysics(false);
				}, 3.0f, false); // 3�� �� ���� �ùķ��̼� ��Ȱ��ȭ
		}
	}
}

