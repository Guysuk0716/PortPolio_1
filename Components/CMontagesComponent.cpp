#include "Components/CMontagesComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
//https://forums.unrealengine.com/t/play-montage-in-c-with-onblendout-oninterrupted-etc/447184
UCMontagesComponent::UCMontagesComponent()
{
	// �ʱ�ȭ
// �ε��ؾ� �� ��Ÿ�� �ּ� ��θ� �����մϴ�.
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FKeyMontageAsset(TEXT("AnimMontage'/Game/Character/Montages/Sword/Sword_F.Sword_F'"));
	if (FKeyMontageAsset.Succeeded())
	{
		FKeyMontage = FKeyMontageAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> BlockMontageAsset(TEXT("AnimMontage'/Game/Character/Montages/Common/Block_Player_V.Block_Player_V'"));
	if (BlockMontageAsset.Succeeded())
	{
		BlockMontage = BlockMontageAsset.Object;
	}
}


// Called when the game starts
void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();
    
        if (DataTable == nullptr)
        {
            CLog::Warning("Data table not defined!!!!!");
            return;
        }
    
        OwnerCharacter = Cast<ACharacter>(GetOwner());
    
        TArray<FMontagesData*> OutRowArray;
        DataTable->GetAllRows<FMontagesData>("", OutRowArray);
    
        // 초기 상태 디버깅 출력
        UE_LOG(LogTemp, Warning, TEXT("Initial Datas array state:"));
        for (int32 i = 0; i < (int32)EStateType::Max; i++)
        {
            if (Datas[i] != nullptr)
            {
                UE_LOG(LogTemp, Warning, TEXT("Datas[%d] = %s"), i, *Datas[i]->Montage->GetName());
            }
        }
    
        for (FMontagesData* data : OutRowArray)
        {
            if (data == nullptr)
                continue;
    
            Datas[(int32)data->Type] = data;
    
            // 업데이트 상태 디버깅 출력
            UE_LOG(LogTemp, Warning, TEXT("Updating Datas[%d] to %s"), (int32)data->Type, *data->Montage->GetName());
        }
    
        // 최종 상태 디버깅 출력
        UE_LOG(LogTemp, Warning, TEXT("Final Datas array state:"));
        for (int32 i = 0; i < (int32)EStateType::Max; i++)
        {
            if (Datas[i] != nullptr)
            {
                UE_LOG(LogTemp, Warning, TEXT("Datas[%d] = %s"), i, *Datas[i]->Montage->GetName());
            }
        }
}
void UCMontagesComponent::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload & BranchingPointPayload)
{
	FString str = NotifyName.ToString();
	
}
void UCMontagesComponent::OnMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	
}
void UCMontagesComponent::PlayBackStepMode()
{
	PlayAnimMontage(EStateType::BackStep);
}
void UCMontagesComponent::PlayRollMode()
{
	PlayAnimMontage(EStateType::Roll);
}
void UCMontagesComponent::PlayDeadMode()
{
	PlayAnimMontage(EStateType::Dead);
}
void UCMontagesComponent::PlayAnimMontage(EStateType InType)
{
	CheckNull(OwnerCharacter);
	FMontagesData* data = Datas[(int32)InType];
	if (data == nullptr)
	{
		//        CLog::Warning("Montage data not founded!!!");
		return;
	}

	//   OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);

	OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(data->Montage, data->PlayRate);
}

void UCMontagesComponent::PlayFKeyMontage()
{
	ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
	if (CharacterOwner)
	{
		// ĳ���� �������� �޽��� �����ɴϴ�.
		USkeletalMeshComponent* Mesh = CharacterOwner->GetMesh();

		// UAnimInstance�� �����ɴϴ�.
		UAnimInstance* AnimInstance = Mesh->GetAnimInstance();

		// �ִϸ��̼��� ����մϴ�.
		if (AnimInstance && FKeyMontage)
		{
			AnimInstance->Montage_Play(FKeyMontage);
		}
	}
}

void UCMontagesComponent::PlayBlockAnimation()
{
	ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
	if (CharacterOwner)
	{
		// ĳ���� �������� �޽��� �����ɴϴ�.
		USkeletalMeshComponent* Mesh = CharacterOwner->GetMesh();

		// UAnimInstance�� �����ɴϴ�.
		UAnimInstance* AnimInstance = Mesh->GetAnimInstance();

		// �ִϸ��̼��� ����մϴ�.
		if (AnimInstance && BlockMontage)
		{
			AnimInstance->Montage_Play(BlockMontage);
		}
	}
}
