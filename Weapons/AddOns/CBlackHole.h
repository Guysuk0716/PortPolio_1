// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBlackHole.generated.h"

UCLASS()
class U06_BATTLE_API ACBlackHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACBlackHole();
 
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:  // Around�� ��ƼŬ
	UPROPERTY(VisibleAnywhere)
		class USphereComponent* Sphere;
	UPROPERTY(VisibleAnywhere)
		class UParticleSystemComponent* Particle;
	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* Explosion;

private:
	void AttractObjects();
	void Explode();
	UPROPERTY(EditAnywhere, Category = "Black Hole")
		float AttractionStrength;
	UPROPERTY(EditAnywhere)
		float ExplosionStrength;



};
