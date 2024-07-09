#pragma once

#include "CoreMinimal.h"
#include "Weapons/AddOns/CShield.h"
#include "Weapons/CXAction.h"
#include "Weapons/CWeaponstructures.h"
#include "CXAction_Around.generated.h"


UCLASS()
class U06_BATTLE_API UCXAction_Around : public UCXAction
{
	GENERATED_BODY()

public:
	UCXAction_Around();	

private:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FDoActionData ActionData;






	// ���콺 �Է� �� ȣ��Ǵ� �Լ�
	virtual void Pressed() override;
	virtual void Released() override;

	// ���� �׼� ���� �� ȣ��Ǵ� �Լ�
	virtual void Begin_XAction_Implementation() override;

	// ���� �׼� ���� �� ȣ��Ǵ� �Լ�
	virtual void End_XAction_Implementation() override;

private:
	void TransformEnemiesToCubes();

	UPROPERTY(EditAnywhere)
		float Range = 1000.0f;

	UPROPERTY(EditAnywhere)
		float HalfAngle = 30.0f; // Degrees

	UPROPERTY(EditAnywhere)
		class UParticleSystem* CubeTransformationEffect;





};