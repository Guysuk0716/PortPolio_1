#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/CWeaponstructures.h"
#include "CSword.generated.h"

UCLASS()
class U06_BATTLE_API ACSword : public AActor
{
	GENERATED_BODY()
    
public:    
	ACSword();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:    
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Homing")
	float Speed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Homing")
	float RotateSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Homing")
	FName EnemyTag = "Player";
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	float DamageInterval = 0.1f;


	float HomingDuration = 1.0f;  // 유도 시간 (초)
	float HomingTimeElapsed = 0.0f;  // 경과 시간

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