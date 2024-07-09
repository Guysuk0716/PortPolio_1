#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/CWeaponstructures.h"
#include "CZone.generated.h"

UCLASS()
class U06_BATTLE_API ACZone : public AActor
{
	GENERATED_BODY()
    
public:    
	ACZone();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:    
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Homing")
	FName EnemyTag = "Player";
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	float DamageInterval = 0.1f;



private:
	AActor* Target;

	void FindTarget();
	FTimerHandle  TimerHandle; // SendMessage()을 Timer로 바인딩시 핸들
	TArray<ACharacter*> Hit;
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FHitData HitData;
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* Capsule;


	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void SendMessage();

};