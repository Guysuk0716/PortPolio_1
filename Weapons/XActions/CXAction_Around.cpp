#include "CXAction_Around.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Characters/CEnemy.h"


UCXAction_Around::UCXAction_Around()
{

}


void UCXAction_Around::Pressed()
{
	Super::Pressed();

	CheckTrue(State->IsXActionMode());
	State->OnXActionMode();

	ActionData.DoAction(Owner);
}

void UCXAction_Around::Released()
{
	Super::Released();

}

void UCXAction_Around::Begin_XAction_Implementation()
{
	Super::Begin_XAction_Implementation();
	TransformEnemiesToCubes();
	
}

void UCXAction_Around::End_XAction_Implementation()
{
	Super::End_XAction_Implementation();

	State->OffXActionMode();
	State->SetIdleMode();

	Movement->Move();

	
}

void UCXAction_Around::TransformEnemiesToCubes()
{
	if (!Owner) return;

	APlayerController* PlayerController = Cast<APlayerController>(Owner->GetController());
	if (!PlayerController) return;

	UCameraComponent* CameraComponent = Owner->FindComponentByClass<UCameraComponent>();
	if (!CameraComponent) return;

	FVector CameraLocation = CameraComponent->GetComponentLocation();
	FRotator CameraRotation = CameraComponent->GetComponentRotation();

	TArray<AActor*> AllEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACEnemy::StaticClass(), AllEnemies);

	for (AActor* Enemy : AllEnemies)
	{
		FVector DirectionToEnemy = Enemy->GetActorLocation() - CameraLocation;
		float DistanceToEnemy = DirectionToEnemy.Size();
		DirectionToEnemy.Normalize();

		float DotProduct = FVector::DotProduct(DirectionToEnemy, CameraRotation.Vector());

		if (DistanceToEnemy <= Range && FMath::Acos(DotProduct) <= FMath::DegreesToRadians(HalfAngle))
		{
			UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(Enemy->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			if (MeshComponent)
			{
				static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("StaticMesh'/Game/Environment/Meshes/Simple_Block_2x2.Simple_Block_2x2'"));
				if (CubeMesh.Succeeded())
				{
					MeshComponent->SetStaticMesh(CubeMesh.Object);

					// Play transformation effect if any
					if (CubeTransformationEffect)
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CubeTransformationEffect, Enemy->GetActorLocation());
					}
				}
			}
		}
	}
}