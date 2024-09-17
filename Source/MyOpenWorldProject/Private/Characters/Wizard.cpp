// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Wizard.h"
#include "Items/Item.h"
#include "Items/Weapon.h"

//EnhancedInput
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
//EnhancedInput

//Camera and SpringArm component
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
//

//UCharacterMovementComponent
#include "GameFramework/CharacterMovementComponent.h"	 
//
 
//UCapsuleComponent
#include "Components/CapsuleComponent.h"
//

#include "Math/UnrealMathUtility.h"

AWizard::AWizard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	WizardPState = WizardPlayerState::Created;
	
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	

	GetCharacterMovement()->bOrientRotationToMovement = true;		//change character rotation to direction of acceleration
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);	//rotation rate
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));	//assign SpringArm object
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bUsePawnControlRotation = true;		//change spring arm's rotation by controller

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));		//assign Camera object
	ViewCamera->SetupAttachment(SpringArm);	//attach to spring arm component
				
	AutoPossessPlayer = EAutoReceiveInput::Player0;		//set possession of Player0
}

void AWizard::BeginPlay()
{
	Super::BeginPlay();
	SetWizardPState(WizardPlayerState::Idling);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);	//add InputMappingContext field to enhanced input player subsystem
		}
	}
	
}

void AWizard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	deltaTime = DeltaTime;
	SetRunningTime(runningTime + DeltaTime);
	FString Message = FString::Printf(TEXT("RunningTime: %f - DeltaTime: %f - Sin(100): %f"), this->runningTime, DeltaTime, FMath::Sin(100.0f));
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Green, Message);  //on screen message
	}
	
}

void AWizard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWizard::Move);
		
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWizard::Look);
		
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AWizard::JumpCustom); //Character jump function
		//EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &AWizard::JumpCustom);
		
		EnhancedInput->BindAction(RunAction, ETriggerEvent::Triggered, this, &AWizard::Run);
		EnhancedInput->BindAction(RunAction, ETriggerEvent::Completed, this, &AWizard::Run);
		
		EnhancedInput->BindAction(EquipItemAction, ETriggerEvent::Triggered, this, &AWizard::EquipItem);
		//EnhancedInput->BindAction(EquipItemAction, ETriggerEvent::Completed, this, &AWizard::EquipItem);	//may be completed action function different function
		
		EnhancedInput->BindAction(UnequipItemAction, ETriggerEvent::Triggered, this, &AWizard::UnequipItem);
		//EnhancedInput->BindAction(UnequipItemAction, ETriggerEvent::Completed, this, &AWizard::UnequipItem);

		EnhancedInput->BindAction(DropEquippedItemAction, ETriggerEvent::Triggered, this, &AWizard::DropEquippedItem);
		//EnhancedInput->BindAction(DropEquippedItemAction, ETriggerEvent::Completed, this, &AWizard::DropEquippedItem);

		EnhancedInput->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AWizard::Attack);
		//EnhancedInput->BindAction(AttackAction, ETriggerEvent::Completed, this, &AWizard::Attack);

		EnhancedInput->BindAction(AimAction, ETriggerEvent::Triggered, this, &AWizard::Aim);
		EnhancedInput->BindAction(AimAction, ETriggerEvent::Completed, this, &AWizard::Aim);
	}
}

void AWizard::Move(const FInputActionValue& value) 
{
	bool isMoveValid = GetController() && GetWizardPState() != WizardPlayerState::Jumping;
	
	if (isMoveValid) {
		const FVector2D MovementVector = value.Get<FInputActionValue::Axis2D>();
		
		const FRotator ControllerRotation = GetControlRotation();
		const FRotator ControllerYawRotation(0.0f, ControllerRotation.Yaw, 0.0f);
		const FVector ForwardVector = FRotationMatrix(ControllerYawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardVector, MovementVector.X);

		const FVector RightVector = FRotationMatrix(ControllerYawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightVector, MovementVector.Y);

		SetWizardPState(WizardPlayerState::Moving);
	}
}

void AWizard::Run(const FInputActionValue& value)
{
	bool isRunValid = GetController() && value.Get<bool>() && GetWizardPState() == WizardPlayerState::Moving 
	&& GetWizardAState() != WizardActionState::Aiming && GetWizardAState() != WizardActionState::Attacking;

	if (isRunValid) {
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		SetWizardPState(WizardPlayerState::Running);
	}

	else {
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
}

void AWizard::Look(const FInputActionValue& value) 
{
	bool isLookValid = GetController() ? true : false;

	if (isLookValid) {
		const FVector2D MouseMovementVector = value.Get<FInputActionValue::Axis2D>();
		AddControllerPitchInput(MouseMovementVector.Y);
		AddControllerYawInput(MouseMovementVector.X);
	}
}

void AWizard::JumpCustom(const FInputActionValue& value) 
{
	bool isJumpValid = GetController() ? true : false;

	if (isJumpValid) {
		if (value.Get<bool>()){
			ACharacter::Jump();
			SetWizardPState(WizardPlayerState::Jumping);
		}
	}
}

void AWizard::EquipItem(const FInputActionValue& value)		//doesn't have animation yet
{
	bool isEquipNewItemValid = GetController() && GetCollidingItem() != nullptr && (!GetPlayerItem() || GetPlayerWeapon());
	bool isEquipItemBackValid = GetController() && (GetPlayerItem() || GetPlayerWeapon()) && GetWizardIState() == WizardItemState::None;
	
	if (isEquipNewItemValid) {
		CollidingItem->Equip(this, FName("HandRSocket"));		//using HandRSocket to attach weapons
		SetWizardIState(WizardItemState::EquippedWeapon);
	}

	else if (isEquipItemBackValid) {
		//equip item back
	}
}

void AWizard::UnequipItem(const FInputActionValue& value)	//doesn't have animation yet
{
	bool isUnequipValid = GetController() && value.Get<bool>() && (!GetPlayerItem() || !GetPlayerItem()) && GetWizardAState() == WizardActionState::None && GetWizardIState() != WizardItemState::None;
	if (isUnequipValid) {
		//GetPlayerItem()->Unequip(this, FName("SomeSocketName"));
		//SetWizardIState(WizardItemState::None);
	}
}

void AWizard::DropEquippedItem(const FInputActionValue& value) {	//doesn't have animation yet
	bool isDropEquippedItemValid = GetController() && (GetPlayerItem()|| GetPlayerWeapon());
	if (isDropEquippedItemValid) {
		if (GetPlayerWeapon() && GetPlayerWeapon()->GetMyItemState() == ItemState::Equipped) {
			PlayerWeapon->Drop(this, FName("HandRSocket"));
		}
		else if (GetPlayerItem() && GetPlayerItem()->GetMyItemState() == ItemState::Equipped) {
			PlayerItem->Drop(this, FName("HandRSocket"));		//using HandRSocket to detach weapons
		}

		SetWizardIState(WizardItemState::None);
	}
}


void AWizard::Aim(const FInputActionValue& value)
{
	bool isAimValid = GetController() && value.Get<bool>() && GetPlayerWeapon() && GetWizardAState() != WizardActionState::Attacking  && GetWizardPState() != WizardPlayerState::Jumping;
	if (isAimValid) {
		SetWizardAState(WizardActionState::Aiming);
		//UE_LOG(LogTemp, Warning, TEXT("AIMING"));

		FRotator CapsuleRotation = GetCapsuleComponent()->GetComponentRotation();	//current rotation
		FRotator ControllerRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);							//target rotation
		
		bool isCameraRotationOver = RotateCameraToAim(CapsuleRotation, ControllerRotation);
		
		if (isCameraRotationOver && !isCameraLocked) {
			LockCamera(true);
			UE_LOG(LogTemp, Warning, TEXT("LockCamera(true)"));
		}
		//zoom camera after interp to aiming rotation
		
	}

	else if (!value.Get<bool>()) // may be cancel attack if aiming cancelled? if(notAiming && Attacking) cancel attack
	{
		//set camera to default position here	
		SetWizardAState(WizardActionState::None);
		LockCamera(false);
	}
}

void AWizard::Attack(const FInputActionValue& value) //animation gets stuck when player spams attack key
{																																	//currently player can attack only while aiming but it can be both while aiming or not aiming
	bool isAttackValid = GetController() && value.Get<bool>() && GetPlayerWeapon() && GetWizardAState() == WizardActionState::Aiming /* || GetWizardAState() != WizardActionState::Attacking*/ && GetWizardPState() != WizardPlayerState::Jumping;
	if (isAttackValid) {
		SetWizardAState(WizardActionState::Attacking);
		LockCamera(true);			//lock camera while attacking
		PlayerWeapon->Fire(this, ViewCamera);	//fire weapon
		FTimerHandle AttackCooldown;
		FTimerDelegate AttackCooldownDelegate = FTimerDelegate::CreateUObject(this, &AWizard::SetWizardAState, WizardActionState::None); 
		GetWorldTimerManager().SetTimer(AttackCooldown, AttackCooldownDelegate, 1.5f, false);	//set cooldown for next attack
	}

	else if (!value.Get<bool>() && GetWizardAState() != WizardActionState::Aiming) {
		if (isCameraLocked) {
			LockCamera(false);
		}
	}
}

void AWizard::LockCamera(bool lock)
{
	if (lock) {
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	else {
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	isCameraLocked = lock;
}

bool AWizard::RotateCameraToAim(FRotator CurrentRotation, FRotator TargetRotation) //it rotates capsule component to aiming pos
{
	if (!CurrentRotation.Equals(TargetRotation, 1)) {
		GetCharacterMovement()->bOrientRotationToMovement = false;
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, deltaTime, 10.0f);
		GetCapsuleComponent()->SetWorldRotation(NewRotation);	//rotate capsule component to new rotation
		return false;
	}

	else {
		return true;
	}
}
