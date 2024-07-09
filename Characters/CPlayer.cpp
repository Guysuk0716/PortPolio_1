#include "Characters/CPlayer.h"

#include "CEnemy_Boss.h"
#include "CGameInstance.h"
#include "CPlayerController.h"
#include "Global.h"
#include "Characters/CAnimInstance.h"
#include "Weapons/AddOns/CArrow.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Weapons/XActions/CXAction_Around.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/InputComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CFeetComponent.h"
#include "Components/CZoomComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Weapons/AddOns/CGhostTrail.h"
#include "Characters/CEnemy_AI.h"
#include "Widgets/CUserWidget_F.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_Stamina.h"
#include "DrawDebugHelpers.h"
#include "IDetailTreeNode.h"
#include "ToolContextInterfaces.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "Components/InventoryComponent.h"
#include "Widgets/CSTutorialHUD.h"
#include "World/Pickup.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create Components
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);


	// Set Mesh & Anim Instance
	{
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

		USkeletalMesh* mesh;
		CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
		GetMesh()->SetSkeletalMesh(mesh);

		TSubclassOf<UCAnimInstance> animInstance;
		CHelpers::GetClass<UCAnimInstance>(&animInstance, "AnimBlueprint'/Game/ABP_Character.ABP_Character_C'");
		GetMesh()->SetAnimClass(animInstance);

		SpringArm->SetRelativeLocation(FVector(0, 0, 140));
		SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
		SpringArm->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
		SpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller
		SpringArm->bDoCollisionTest = false;

	}

	// ActorComponent
	{
		CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
		CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
		CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
		CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
		CHelpers::CreateActorComponent<UCParkourComponent>(this, &Parkour, "Parkour");
		CHelpers::CreateActorComponent<UCFeetComponent>(this, &Feet, "Feet");
	//	CHelpers::CreateActorComponent<UCZoomComponent>(this, &Zoom, "Zoom");
		CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	}
	CreateParkourArrow();


	{
		CHelpers::CreateComponent<UWidgetComponent>(this, &assassinationWidget, "assassination", GetMesh());

		TSubclassOf<UCUserWidget_F> assassinationclass;
		CHelpers::GetClass<UCUserWidget_F>(&assassinationclass, "WidgetBlueprint'/Game/Widgets/WB_F.WB_F_C'");

		assassinationWidget->SetWidgetClass(assassinationclass);
		assassinationWidget->SetRelativeLocation(FVector(0, 0, 220));
		assassinationWidget->SetDrawSize(FVector2D(120, 0));
		assassinationWidget->SetWidgetSpace(EWidgetSpace::Screen);
		assassinationWidget->SetVisibility(true);
	}
	
	{
		InteractionCheckFrequency = 0.1f;
		InteractionCheckDistance  = 225.0f;
		BaseEyeHeight = 74.0f;

		PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
		PlayerInventory->SetSlotsCapacity(20);
		PlayerInventory->SetWeightCapacity(50.0f);
	}

	bIsBlocking = false;
	BlockingMontage = nullptr;
	SuccessBlockingMontage = nullptr;
	
}




void ACPlayer::CreateParkourArrow()
{
	// Create Arrows
	CHelpers::CreateComponent<USceneComponent>(this, &ArrowGroup, "ArrowGroup", GetCapsuleComponent());

	for (int32 i = 0; i < (int32)EParkourArrowType::Max; i++)
	{
		// Enum�� ����� index ��°�� name�� FString���� �����´�.
		FString name = StaticEnum<EParkourArrowType>()->GetNameStringByIndex(i);
		CHelpers::CreateComponent<UArrowComponent>(this, &Arrows[i], FName(name), ArrowGroup);

		switch ((EParkourArrowType)i)
		{
		case EParkourArrowType::Center:
			Arrows[i]->ArrowColor = FColor::Red;
			break;
		case EParkourArrowType::Ceil:
			Arrows[i]->ArrowColor = FColor::Green;
			Arrows[i]->SetRelativeLocation(FVector(0, 0, 100));
			break;
		case EParkourArrowType::Floor:
			Arrows[i]->ArrowColor = FColor::Blue;
			Arrows[i]->SetRelativeLocation(FVector(0, 0, -80));
			break;
		case EParkourArrowType::Left:
			Arrows[i]->ArrowColor = FColor::Magenta;
			Arrows[i]->SetRelativeLocation(FVector(0, -30, 0));
			break;
		case EParkourArrowType::Right:
			Arrows[i]->ArrowColor = FColor::Magenta;
			Arrows[i]->SetRelativeLocation(FVector(0, 30, 0));
			break;
		case EParkourArrowType::Land:
			Arrows[i]->ArrowColor = FColor::Yellow;
			Arrows[i]->SetRelativeLocation(FVector(200, 0, 100));
			Arrows[i]->SetRelativeRotation(FRotator(-90, 0, 0));
			break;
		}
	}


}



void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	UCGameInstance* GameInstance = Cast<UCGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		// Check if player data is initialized, if not initialize it with default values
		if (!GameInstance->bIsPlayerDataInitialized)
		{
			TArray<UItemBase*> DefaultInventory;
			GameInstance->InitializePlayerData(Status->GetMaxHealth(), Movement->GetMaxStamina(), DefaultInventory);
		}
		LoadPlayerData();
	}
	Movement->SetSpeed(ESpeedType::Run);
//	Movement->DisableControlRotation();
	// Biding In Game���� ���־���Ѵ�
	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains(TEXT("StartLevel")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Skipping widget creation for Portal map"));
		return;
	}

	UClass* WidgetClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/Widgets/WB_HPBar.WB_HPBar_C'"));
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	
	if (Widget)
	{
		Widget->AddToViewport();

		HPWidget = Cast<UCUserWidget_Stamina>(Widget);
		
	}
	
	if (HPWidget)
	{
		UpdateHealthAndStaminaOnWidget(Status->GetHealth(), Status->GetMaxHealth(), Movement->GetStamina(), Movement->GetMaxStamina());
		GetWorld()->GetTimerManager().SetTimer(CooldownUpdateTimerHandle, this, &ACPlayer::UpdateCooldowns, 0.1f, true);
	}
	
	HUD = Cast<ACSTutorialHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (Weapon)
	{
		Weapon->OnWeaponTypeChanged.AddDynamic(this, &ACPlayer::OnWeaponTypeChanged);
	}
}

void ACPlayer::UpdateHealthAndStaminaOnWidget(float CurrentHealth, float MaxHealth, float CurrentStamina, float MaxStamina)
{
	// HPWidget�� ��ȿ���� Ȯ��
	if (HPWidget)
	{
		// Health �� Stamina ������Ʈ �Լ� ȣ��
		HPWidget->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
		HPWidget->UpdateStamina(Movement->GetStamina(),Movement->GetMaxStamina());
	}
}


void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement Binging : �Լ����������̱� ������ Serialixe���� �ʴ´�
	PlayerInputComponent->BindAxis("MoveForward", Movement, &UCMovementComponent::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", Movement, &UCMovementComponent::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", Movement, &UCMovementComponent::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", Movement, &UCMovementComponent::OnVerticalLook);

	// Zoom ���ε�
	PlayerInputComponent->BindAction("Zoom", EInputEvent::IE_Pressed, this, &ACPlayer::SetZooming);

	// Avoid & Sprint
	PlayerInputComponent->BindAction("Avoid", EInputEvent::IE_Released, this, &ACPlayer::OnAvoid);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::OnSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, Movement, &UCMovementComponent::OnRun);

	// ���� ���ε�
	{
		PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetFistMode);
		PlayerInputComponent->BindAction("Sword", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetSwordMode);
		PlayerInputComponent->BindAction("Hammer", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetHammerMode);
		PlayerInputComponent->BindAction("Warp", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetWarpMode);
		PlayerInputComponent->BindAction("Around", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetAroundMode);
		PlayerInputComponent->BindAction("Bow", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetBowMode);

	}

	// Action ���ε�
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::DoAction);



	// Dash ���ε�
	PlayerInputComponent->BindAction("Dash", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::OnDashing);
	PlayerInputComponent->BindAction("Dash", EInputEvent::IE_Released, Movement, &UCMovementComponent::OffDashing);

	// SubAction ���ε�
	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnRightButton);
	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Released, this, &ACPlayer::OffRightButton);

	PlayerInputComponent->BindAction("ZAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnZButton);
	PlayerInputComponent->BindAction("ZAction", EInputEvent::IE_Released, this, &ACPlayer::OffZButton);


	PlayerInputComponent->BindAction("XAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnXButton);
	PlayerInputComponent->BindAction("XAction", EInputEvent::IE_Released, this, &ACPlayer::OffXButton);

	PlayerInputComponent->BindAction("Block", EInputEvent::IE_Pressed, this, &ACPlayer::StartBlock);
	PlayerInputComponent->BindAction("Block", EInputEvent::IE_Released, this, &ACPlayer::EndBlock);

	//Assassination ���ε�
	PlayerInputComponent->BindAction("Assassination", EInputEvent::IE_Pressed, this, &ACPlayer::PerformStealthKill);



	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &ACPlayer::BeginInteract);
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Released, this, &ACPlayer::EndInteract);
	
	PlayerInputComponent->BindAction("ToggleMenu", EInputEvent::IE_Pressed, this, &ACPlayer::ToggleMenu);
	
	PlayerInputComponent->BindAction("MainMenu", EInputEvent::IE_Pressed,this,  &ACPlayer::OnGamePause);
}


void ACPlayer::Landed(const FHitResult & Hit)
{
	Parkour->DoParkour(true);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), LandingSound, GetActorLocation());



}
void  ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Idle:      break;
	case EStateType::Equip:     break;
	case EStateType::Dead:     Dead(); break;
	case EStateType::Action:    break;
	case EStateType::BackStep: BackStep(); break;
	case EStateType::Roll:     Roll();     break;
	case EStateType::Hit:      Hitted();   break;
	default:
		//        CLog::Print("Undefined");
		break;

	}

}

void  ACPlayer::OnAvoid()
{
	// idle�̰� moving�����ϸ� ȸ�ǵ���
	CheckFalse(State->IsIdleMode());
	CheckFalse(Movement->CanMove());

	float value = InputComponent->GetAxisValue("MoveForward");
	if (value > 0.0f)
		State->SetRollMode();
	else
		State->SetBackStepMode();


}
void  ACPlayer::BackStep()
{
	Movement->EnableControlRotation();
	// ��Ÿ�� play
	Montages->PlayBackStepMode();
}
void  ACPlayer::Roll()
{
	Movement->EnableControlRotation();
	Montages->PlayRollMode();
}

float ACPlayer::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (bIsBlocking)
	{
		// 여기에서 블락되었음을 알리는 로직을 추가할 수 있습니다.
		PlayAnimMontage(SuccessBlockingMontage,1.0,NAME_None);

		// 공격자에게 그로기 몽타주 재생
		ACharacter* AttackerCharacter = Cast<ACharacter>(EventInstigator->GetPawn());
		if (AttackerCharacter && AttackerCharacter->GetMesh())
		{
			UAnimInstance* AttackerAnimInstance = AttackerCharacter->GetMesh()->GetAnimInstance();
			if (AttackerAnimInstance && BossGroggyMontage)
			{
				AttackerAnimInstance->Montage_Play(BossGroggyMontage);
			}
		}

		
		UE_LOG(LogTemp, Warning, TEXT("Damage blocked!"));
		// 데미지를 0으로 설정
		return 0.0f;
	}

	ACEnemy_Boss* Boss = Cast<ACEnemy_Boss>(DamageCauser);
	if (Boss && Boss->IsInBossRageMode())
	{
		DamageAmount *= 1.5f; // RageMode일 때 데미지 증가
	}
	
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damage.power = damage;
	Damage.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	Damage.Causer = DamageCauser;
	Damage.Event = (FActionDamageEvent*)&DamageEvent;

	State->SetHitMode();
	
	return damage;
}
void ACPlayer::Hitted()
{
	// Health 
	Status->Damage(Damage.power);
	Damage.power = 0.0f;


	if (!!Damage.Event && !!Damage.Event->HitData)
	{
		// Montage
		Damage.Event->HitData->PlayMontage(this);
		// HitStop
		Damage.Event->HitData->PlayHitStop(GetWorld());
		// Sound
		Damage.Event->HitData->PlaySoundWave(this);
		// Effect
		Damage.Event->HitData->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());

		/*if (!Status->IsDead())
		{
			FVector start = GetActorLocation();
			FVector target = Damage.Character->GetActorLocation();
			FVector direction = target - start;

			direction = direction.GetSafeNormal();
			LaunchCharacter(-direction * Damage.Event->HitData->Launch, false, false);
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
		}*/

		if (Status->IsDead())
		{
			//   PrintLine();
			State->SetDeadMode();
			return;
		}
	}


	Damage.Character = nullptr;
	Damage.Causer = nullptr;
	Damage.Event = nullptr;

}

void ACPlayer::Dead()
{
	Montages->PlayDeadMode();
}


void ACPlayer::StartBlock()
{
	bIsBlocking = true;

	// 몽타주 재생
	if (BlockingMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(BlockingMontage);
		}
	}
}

void ACPlayer::EndBlock()
{
	bIsBlocking = false;

	// 몽타주 중지
	if (BlockingMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && AnimInstance->Montage_IsPlaying(BlockingMontage))
		{
			AnimInstance->Montage_Stop(0.2f, BlockingMontage);
		}
	}
}

void ACPlayer::UpdateInteractionWidget() const
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}

void ACPlayer::End_Avoid()
{
	State->SetIdleMode();
}
void ACPlayer::End_Hit()
{
	State->SetIdleMode();
}
void ACPlayer::End_Dead()
{
	Destroy();
}
void ACPlayer::OnRightButton()
{
	if (Weapon->IsBowMode())
	{
		Weapon->SubAction_Pressed();
	}

	if (Weapon->IsUnarmedMode())
	{
		Parkour->DoParkour();
		return;
	}
	
	if (IsSkillOnCooldown("RightButton"))
		return;
	
	
	
	
	if(Weapon->IsFistMode())
	{
		Weapon->SubAction_Pressed();
		SetSkillCooldown("RightButton", 3.0f);
	}
	if(Weapon->IsSwordMode())
	{
		Weapon->SubAction_Pressed();
		SetSkillCooldown("RightButton", 4.0f);
	}
	if(Weapon->IsHammerMode())
	{
		Weapon->SubAction_Pressed();
		SetSkillCooldown("RightButton", 5.0f);
	}
	if(Weapon->IsWarpMode())
	{
		Weapon->SubAction_Pressed();
		SetSkillCooldown("RightButton", 6.0f);
	}
	if(Weapon->IsAroundMode())
	{
		Weapon->SubAction_Pressed();
		SetSkillCooldown("RightButton", 7.0f);
	}

	
}
void ACPlayer::OffRightButton()
{
	// Bow ����� ���
	// >>>>>>

	Weapon->SubAction_Released();
}

void ACPlayer::OnZButton()
{

	if (IsSkillOnCooldown("ZKey"))
		return;
	
	if (Weapon->IsSwordMode()) {
		// �� ����� �� �� ���� ZKeyAction_Pressed() ȣ��
		Weapon->ZAction_Pressed();
		SetSkillCooldown("ZKey", 3.0f);
	}

	if (Weapon->IsFistMode()) {
		// �� ����� �� �� ���� ZKeyAction_Pressed() ȣ��
		Weapon->ZAction_Pressed();
		SetSkillCooldown("ZKey", 5.0f); 
	}
	if (Weapon->IsAroundMode()) {
		// �� ����� �� �� ���� ZKeyAction_Pressed() ȣ��
		Weapon->ZAction_Pressed();
		SetSkillCooldown("ZKey", 7.0f); 
	}
	
	if (Weapon->IsHammerMode())
	{
		bZKeyHeld = true;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ACPlayer::RepeatZKeyAction, 0.5f, true);
		RepeatZKeyAction();
		SetSkillCooldown("ZKey", 5.0f);
	}

	if (Weapon->IsBowMode()) 
	{
		Weapon->ZAction_Pressed();
		SetSkillCooldown("ZKey", 1.0f);
	}
	
}

void ACPlayer::OffZButton()
{


	bZKeyHeld = false;

	// Ÿ�̸Ӹ� ����
	GetWorldTimerManager().ClearTimer(TimerHandle);

	// �ظ� ����� ��� ZKeyAction_Released �޼ҵ带 ȣ��
	if (Weapon->IsHammerMode()) {
		Weapon->ZAction_Released();
	}
}

void  ACPlayer::RepeatZKeyAction()
{
	if (bZKeyHeld && Weapon->IsHammerMode()) {
		// MovementComponent�� ���� ������ �����ɴϴ�.
		UCMovementComponent* MovementComponent = Cast<UCMovementComponent>(Movement);

		// MovementComponent�� �����ϰ�, ���� ���¹̳��� ��ų ���࿡ �ʿ��� ��ŭ ����� ���
		if (MovementComponent && MovementComponent->CurrentStamina >= MovementComponent->HammerStaminaCost)
		{

			Weapon->ZAction_Pressed();

			// ��ų ���� �� ���¹̳� ����
			MovementComponent->CurrentStamina -= MovementComponent->HammerStaminaCost;
		}
		else {
			// ���¹̳��� �����ϸ� Ÿ�̸� ����
			GetWorldTimerManager().ClearTimer(TimerHandle);
		}
	}
	else {
		// Z Ű�� �����Ǿ��ų� �ظ� ��尡 �ƴ� ��� Ÿ�̸� ����
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}
}

void ACPlayer::RepeatXKeyAction()
{

	if (bXKeyHeld && Weapon->IsAroundMode())
	{
		// MovementComponent�� ���� ������ �����ɴϴ�.
		UCMovementComponent* MovementComponent = Cast<UCMovementComponent>(Movement);

		// MovementComponent�� �����ϰ�, ���� ���¹̳��� ��ų ���࿡ �ʿ��� ��ŭ ����� ���
		if (MovementComponent && MovementComponent->CurrentStamina >= MovementComponent->AroundStaminaCost)
		{

			Weapon->XAction_Pressed();

			// ��ų ���� �� ���¹̳� ����
			MovementComponent->CurrentStamina -= MovementComponent->AroundStaminaCost;
		}
		else {
			// ���¹̳��� �����ϸ� Ÿ�̸� ����
			GetWorldTimerManager().ClearTimer(TimerHandle);
		}
	}
	else {
		// Z Ű�� �����Ǿ��ų� �ظ� ��尡 �ƴ� ��� Ÿ�̸� ����
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}
}

void ACPlayer::OnXButton()
{
	if (IsSkillOnCooldown("XKey"))
		return;
	if (Weapon->IsFistMode()) 
	{
		Weapon->XAction_Pressed();
		SetSkillCooldown("XKey", 3.0f);
	}
	if (Weapon->IsSwordMode())
	{
		Weapon->XAction_Pressed();
		SetSkillCooldown("XKey", 4.0f);
	}
	if (Weapon->IsHammerMode())
	{
		Weapon->XAction_Pressed();
		SetSkillCooldown("XKey", 5.0f);
	}
	if (Weapon->IsAroundMode()) 
	{
		Weapon->XAction_Pressed();
		SetSkillCooldown("XKey", 6.0f);
	}
}

void ACPlayer::OffXButton()
{
	bXKeyHeld = false;
	// Ÿ�̸Ӹ� ����
	GetWorldTimerManager().ClearTimer(TimerHandle);

	if (Weapon->IsAroundMode())
	{
		Weapon->XAction_Released();
	}
}

void ACPlayer::SetZooming()
{
	//SpringArm->TargetArmLength = 1000;
	

}
void ACPlayer::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (TeamID != NewTeamID)
		TeamID = NewTeamID;
}

void ACPlayer::OnWeaponTypeChanged(EWeaponType PrevType, EWeaponType NewType)
{
	if (HPWidget)
		{
		// 모든 이미지를 초기화 (비활성화)
		HPWidget->HighlightFistImage(false);
		HPWidget->HighlightSwordImage(false);
		HPWidget->HighlightHammerImage(false);
		HPWidget->HighlightMagicImage(false);
		HPWidget->HighlightBowImage(false);
	
		// 새로운 무기 유형만 활성화
		switch (NewType)
		{
		case EWeaponType::Fist:
			HPWidget->HighlightFistImage(true);
			break;
		case EWeaponType::Sword:
			HPWidget->HighlightSwordImage(true);
			break;
		case EWeaponType::Hammer:
			HPWidget->HighlightHammerImage(true);
			break;
		case EWeaponType::Around:
		case EWeaponType::Warp:
			HPWidget->HighlightMagicImage(true);
			break;
		case EWeaponType::Bow:
			HPWidget->HighlightBowImage(true);
			break;
		default:
			break;
		}
	}
	
}







void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UCUserWidget_F* fkey = Cast<UCUserWidget_F>(assassinationWidget->GetUserWidgetObject());
	if (!!fkey)
	{
		// 가장 가까운 적을 찾습니다.
		ACEnemy_AI* nearestEnemy = nullptr;
		float closestDistance = FLT_MAX;

		TArray<AActor*> allEnemies;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACEnemy_AI::StaticClass(), allEnemies);

		for (AActor* enemyActor : allEnemies)
		{
			ACEnemy_AI* enemyAI = Cast<ACEnemy_AI>(enemyActor);
			if (enemyAI)
			{
				FVector toEnemy = enemyAI->GetActorLocation() - GetActorLocation();
				float distance = toEnemy.Size();
				FVector forwardVector = GetActorForwardVector();
				float dotProduct = FVector::DotProduct(forwardVector, toEnemy.GetSafeNormal());

				// Forward 방향으로 100 이내에 있는지 확인
				if (distance < closestDistance && distance <= 100.0f && dotProduct > 0.9f)  // 0.9f는 약 25도 이내를 의미
				{
					closestDistance = distance;
					nearestEnemy = enemyAI;
				}
			}
		}

		// 가장 가까운 적이 forward 방향 100 이내에 있는 경우 위젯을 표시
		//if (nearestEnemy && closestDistance <= 100.0f)
		//{
		//	fkey->SetVisibility(ESlateVisibility::Visible);
		//}
		//else
		//{
			fkey->SetVisibility(ESlateVisibility::Collapsed);
		//}
	}

	if (Movement)
	{
		Movement->RecoverStamina(DeltaTime);
	}

	if (HPWidget)
	{
		UpdateHealthAndStaminaOnWidget(Status->GetHealth(), Status->GetMaxHealth(), Movement->GetStamina(), Movement->GetMaxStamina());
	}

	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}

	{
		FVector StartLocation = SpringArm->GetComponentLocation();
		FVector EndLocation = Camera->GetComponentLocation();
		FVector Direction = (EndLocation - StartLocation).GetSafeNormal();
		float TargetArmLength = MaxCameraArmLength;
	
		FHitResult OutHit;
		FVector TraceEnd = StartLocation + (Direction * MaxCameraArmLength);
	
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
	
		bool bHit = GetWorld()->SweepSingleByChannel(
			OutHit,
			StartLocation,
			TraceEnd,
			FQuat::Identity,
			ECC_Camera,
			FCollisionShape::MakeSphere(ProbeSize),
			CollisionParams
		);
	
		if (bHit)
		{
			TargetArmLength = FMath::Clamp(OutHit.Distance, 5.0f, MaxCameraArmLength);
		}
	
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, TargetArmLength, DeltaTime, 10.0f);
	
	}
	
}

void ACPlayer::ToggleMenu()
{
	HUD->ToggleMenu();
}

void ACPlayer::OnGamePause()
{
	HUD->TogglePauseMenu();
}

void ACPlayer::DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	if(PlayerInventory->FindMatchingItem(ItemToDrop))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const FVector SpawnLocation{GetActorLocation()+(GetActorForwardVector() * 50.0f)};
		const FTransform SpawnTransform(GetActorRotation(),SpawnLocation);

		const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop,QuantityToDrop);

		APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(),SpawnTransform,SpawnParams);

		Pickup->InitializeDrop(ItemToDrop,RemovedQuantity);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Item to drop was somehow null!"));
	}
}

void ACPlayer::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart{ GetPawnViewLocation() };
	FVector TraceEnd{ TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance) };

	float LookDirection = FVector::DotProduct(GetActorForwardVector(),GetViewRotation().Vector());

	if(LookDirection > 0)
	{

		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);


		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		FHitResult TraceHit;

		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{
				const float Distance = (TraceStart - TraceHit.ImpactPoint).Size();

				if (TraceHit.GetActor() != InteractionData.CurrentInteractable && Distance <= InteractionCheckDistance)
				{
					FoundInteractable(TraceHit.GetActor());
					return;
				}

				if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
				{
					return;
				}
			}
		}
	}

	NoInteractableFound();
}

void ACPlayer::FoundInteractable(AActor * NewInteractable)
{
	if (IsInteracting())
	{
		EndInteract();
	}

	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	if (HUD)
	{
		HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
	
	TargetInteractable->BeginFocus();
}

void ACPlayer::NoInteractableFound()
{
	if (IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}
		if (HUD)
		{
			HUD->HideInteractionWidget();
		}

		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void ACPlayer::BeginInteract()
{
	// verify noting has changed with the iteractable state since beginning interaction
	PerformInteractionCheck();

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();

			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction, this, &ACPlayer::Interact, TargetInteractable->InteractableData.InteractionDuration, false);
			}
		}
	}
}

void ACPlayer::EndInteract()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

void ACPlayer::Interact()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(this);
	}
}

void ACPlayer::SavePlayerData()
{
	UCGameInstance* GameInstance = Cast<UCGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->SavePlayerData(Status->GetHealth(), Movement->GetStamina(), PlayerInventory->GetInventoryContents());
	}
}

void ACPlayer::LoadPlayerData()
{
	UCGameInstance* GameInstance = Cast<UCGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		float Health, Stamina;
		TArray<UItemBase*> Inventory;
		GameInstance->LoadPlayerData(Health, Stamina, Inventory);
		Status->SetHealth(Health);
		Movement->SetStamina(Stamina);

		// Clear the current inventory
		PlayerInventory->GetInventoryContents().Empty();

		// Add loaded items to the inventory
		for (UItemBase* Item : Inventory)
		{
			PlayerInventory->HandleAddItem(Item);
		}
	}
}

void ACPlayer::SetSkillCooldown(FName SkillName, float CooldownTime)
{
	SkillCooldowns.Add(SkillName, CooldownTime);
	FTimerHandle& CooldownTimer = CooldownTimers.FindOrAdd(SkillName);
	GetWorld()->GetTimerManager().SetTimer(CooldownTimer, [this, SkillName]() { ResetCooldown(SkillName); }, CooldownTime, false);
	if (HPWidget)
	{
		HPWidget->ShowCooldown(SkillName);
		HPWidget->UpdateCooldown(SkillName, CooldownTime);
	}
}

bool ACPlayer::IsSkillOnCooldown(FName SkillName) const
{
	return SkillCooldowns.Contains(SkillName);
}

void ACPlayer::ResetCooldown(FName SkillName)
{
	SkillCooldowns.Remove(SkillName);
	CooldownTimers.Remove(SkillName);
	if (HPWidget)
	{
		HPWidget->HideCooldown(SkillName);
	}
}

void ACPlayer::UpdateCooldowns()
{
	TArray<FName> Keys;
	SkillCooldowns.GetKeys(Keys);

	for (FName Key : Keys)
	{
		if (SkillCooldowns[Key] > 0.0f)
		{
			SkillCooldowns[Key] -= 0.1f;
			if (HPWidget)
			{
				HPWidget->UpdateCooldown(Key, SkillCooldowns[Key]);
			}
		}
		else
		{
			ResetCooldown(Key); // ResetCooldown 함수 호출
		}
	}
}

void ACPlayer::StealthKill()
{
	
	PlayAnimMontage(StealthMontage, 1.0, NAME_None);
	
}

void ACPlayer::PerformStealthKill()
{
	FHitResult HitResult;
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * 100.0f; // Example range

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, CollisionParams);
	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit something"));

		// HitResult.Actor의 클래스 이름을 로그로 출력
		if (HitResult.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit actor class: %s"), *HitResult.GetActor()->GetClass()->GetName());
		}

		ACEnemy* Target = Cast<ACEnemy>(HitResult.GetActor());
		if (Target)
		{
			// 적과 캐릭터가 같은 방향을 보고 있는지 확인
			FVector PlayerForward = GetActorForwardVector();
			FVector EnemyForward = Target->GetActorForwardVector();

			// Dot product로 방향 비교
			float DotProduct = FVector::DotProduct(PlayerForward, EnemyForward);
			if (DotProduct > 0.9f) // 0.9f는 약 25도 이내를 의미
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit actor is an ACEnemy and is facing the same direction"));
				StealthKill();
				Target->StealthDead();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit actor is an ACEnemy but not facing the same direction"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit actor is not an ACEnemy"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace did not hit anything"));
	}

	// 디버그 라인 그리기 (옵션)
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 1.0f);
}

bool ACPlayer::CheckForCameraObstruction(FHitResult& HitResult)
{
	FVector Start = SpringArm->GetComponentLocation();
	FVector End = Start + (SpringArm->GetForwardVector() * -1 * DesiredArmLength);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	return GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Camera, CollisionParams);
}

void ACPlayer::AdjustCameraBoomLength(float DeltaTime)
{
	FHitResult HitResult;
	if (CheckForCameraObstruction(HitResult))
	{
		float HitDistance = (HitResult.Location - SpringArm->GetComponentLocation()).Size();
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, HitDistance, DeltaTime, 10.0f);
	}
	else
	{
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, DesiredArmLength, DeltaTime, 10.0f);
	}
}

