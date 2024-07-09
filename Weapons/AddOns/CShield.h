// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraDataInterfaceExport.h"
#include "NiagaraSystem.h"
#include "Weapons/CWeaponstructures.h"
#include "CShield.generated.h"

UCLASS()
class U06_BATTLE_API ACShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACShield();


private: // Component
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Root;
	UPROPERTY(VisibleDefaultsOnly)
		class UNiagaraComponent* Niagara;
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* Box;



private:
	UPROPERTY(EditDefaultsOnly, Category = "Hit")
		FHitData HitData;

	UPROPERTY(VisibleAnywhere)
		class UParticleSystem* Particle;


private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);


private:
	TArray<class ACharacter*> Hit;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
