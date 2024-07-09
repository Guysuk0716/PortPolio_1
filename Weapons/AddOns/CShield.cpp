#include "CShield.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

ACShield::ACShield()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UNiagaraComponent>(this, &Niagara, "Niagara", Root);
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Root);

	// 초기 콜리전 설정
	Box->SetCollisionProfileName("BlockAllDynamic");
	Box->SetGenerateOverlapEvents(true);
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACShield::OnComponentBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACShield::OnComponentEndOverlap);
}

void ACShield::BeginPlay()
{
	Super::BeginPlay();

	// 충돌 설정
	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Box->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
}

void ACShield::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	PrintLine();
	
}

void ACShield::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Shield Ended Overlap with %s"), *OtherActor->GetName());
	}
}



