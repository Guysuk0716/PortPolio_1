
#include "Components/CWeaponComponent.h"
#include "Global.h"
#include "CStateComponent.h"
#include "GameFramework/Character.h"
#include "Weapons/CWeaponAsset.h"
#include "Weapons/CWeaponData.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CSubAction.h"
#include "Weapons/CZAction.h"
#include "Weapons/CXAction.h"
#include "GameFramework/Actor.h"
#include "Animation/AnimInstance.h"


UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
    // InGame에서 Parent를 가져 올수 있음 : 생성자(X)
    // int A[3];
    // 배열의 이름 주소임
    // A[0]의 주소 --> &A[0]
    OwnerCharacter = Cast<ACharacter>(GetOwner());
    for (int32 i = 0; i < (int32)EWeaponType::Max; i++)
    {
        if (!!DataAssets[i])
            DataAssets[i]->BeginPlay(OwnerCharacter,&Datas[i]); // Asset에 있는 데이터를 초기화
    }

}

void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!!GetDoAction())
        GetDoAction()->Tick(DeltaTime);
    if (!!GetSubAction())
        GetSubAction()->Tick(DeltaTime);


}



void UCWeaponComponent::SetUnarmedMode()
{
    CheckFalse(IsIdleMode());
    GetEquipment()->UnEquip();
    ChangeType(EWeaponType::Max);
}

void UCWeaponComponent::SetFistMode()
{
    CheckFalse(IsIdleMode());
    SetMode(EWeaponType::Fist);
}

void UCWeaponComponent::SetSwordMode()
{
    // idle이 아니면 무기장착 안됨

    CheckFalse(IsIdleMode());
    SetMode(EWeaponType::Sword);
  
}

void UCWeaponComponent::SetHammerMode()
{
    CheckFalse(IsIdleMode());
    SetMode(EWeaponType::Hammer);
}

void UCWeaponComponent::SetWarpMode()
{
    CheckFalse(IsIdleMode());
    SetMode(EWeaponType::Warp);
}

void UCWeaponComponent::SetAroundMode()
{
    CheckFalse(IsIdleMode());
    SetMode(EWeaponType::Around);
}

void UCWeaponComponent::SetBowMode()
{
    CheckFalse(IsIdleMode());
    SetMode(EWeaponType::Bow);
}

void UCWeaponComponent::DoAction()
{
    if (!!GetDoAction())
        GetDoAction()->DoAction();
   
}

bool UCWeaponComponent::IsIdleMode()
{
    return CHelpers::GetComponent<UCStateComponent>(OwnerCharacter)->IsIdleMode();
}

void UCWeaponComponent::SetMode(EWeaponType InType)
{
    if (Type == InType)
    {
        SetUnarmedMode(); 
        return;
    }
    if (IsUnarmedMode() == false)
    {

        GetEquipment()->UnEquip();
    }
    
    Datas[(int32)InType]->GetEquipment()->Equip();

    ChangeType(InType);
}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
    EWeaponType prevType = Type;
    Type = InType;
    if (OnWeaponTypeChanged.IsBound())
        OnWeaponTypeChanged.Broadcast(prevType, InType);
}

ACAttachment * UCWeaponComponent::GetAttachment()
{
    CheckTrueResult(IsUnarmedMode(), nullptr);
    CheckFalseResult(!!Datas[(int32)Type], nullptr);

    return Datas[(int32)Type]->GetAttachment();
}

UCEquipment * UCWeaponComponent::GetEquipment()
{
    CheckTrueResult(IsUnarmedMode(), nullptr);
    CheckFalseResult(!!Datas[(int32)Type], nullptr);

    return Datas[(int32)Type]->GetEquipment();
}

UCDoAction* UCWeaponComponent::GetDoAction()
{

    CheckTrueResult(IsUnarmedMode(), nullptr);
    CheckFalseResult(!!Datas[(int32)Type], nullptr);
    return Datas[(int32)Type]->GetDoAction();
}

UCSubAction*  UCWeaponComponent::GetSubAction()
{
    CheckTrueResult(IsUnarmedMode(), nullptr);
    CheckFalseResult(!!Datas[(int32)Type], nullptr);
    return Datas[(int32)Type]->GetSubAction();
}
UCZAction* UCWeaponComponent::GetZAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);
	return Datas[(int32)Type]->GetZAction();
}

UCXAction* UCWeaponComponent::GetXAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);
	return Datas[(int32)Type]->GetXAction();
}
void UCWeaponComponent::SubAction_Pressed()
{
  
    if (!!GetSubAction())
        GetSubAction()->Pressed();
}

void UCWeaponComponent::SubAction_Released()
{
    if (!!GetSubAction())
        GetSubAction()->Released();
}

void UCWeaponComponent::ZAction_Pressed()
{

	if (!!GetZAction())
		GetZAction()->Pressed();
}

void UCWeaponComponent::ZAction_Released()
{
	if (!!GetZAction())
		GetZAction()->Released();
}

void UCWeaponComponent::XAction_Pressed()
{

	if (!!GetXAction())
		GetXAction()->Pressed();
}

void UCWeaponComponent::XAction_Released()
{
	if (!!GetXAction())
		GetXAction()->Released();
}
void UCWeaponComponent::DestoryWeapon()
{
	for (int32 i = 0; i < (int32)EWeaponType::Max; i++)
	{
		if (!!Datas[i] && Datas[i]->GetAttachment())
			Datas[i]->GetAttachment()->Destroy();
	}
}



