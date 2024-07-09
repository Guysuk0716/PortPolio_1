#pragma once

#include "CoreMinimal.h"
#include "Weapons/CZAction.h"
#include "CZAction_Sword.generated.h"

/**
 *
 */
	UCLASS()
	class U06_BATTLE_API UCZAction_Sword : public UCZAction
{
	GENERATED_BODY()


private:  // Sword에서는 HitData대신 Aura를 이용
	UPROPERTY(EditAnywhere, Category = "ZAura")
		TSubclassOf<class ACZAura> ZAuraClass;
	UPROPERTY(EditAnywhere, Category = "ZAura")
		FVector ZAuarLocation;
	UPROPERTY(EditAnywhere, Category = "Action")
		TArray<FHitData> HitDatas;


private:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FDoActionData ActionData;
	//    UPROPERTY(EditDefaultsOnly, Category = "Action")
	 //       FHitData	  HitData;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Add-On")
		TSubclassOf<class ACGhostTrail> GhostTrailClass;


public:
	virtual void Pressed() override;

public:
	virtual void Begin_ZAction_Implementation() override;
	virtual void End_ZAction_Implementation() override;


private:
	class ACGhostTrail* GhostTrail;
};
