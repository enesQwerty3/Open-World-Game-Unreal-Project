// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Wizard.h"
#include "Items/Item.h"
#include "Items/Weapon.h"

//EnhancedInput
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
//

//Camera and SpringArm component
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
//

//Components
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"	 
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
//

//RInterp
#include "Math/UnrealMathUtility.h"
//

//Attribute Component
#include "Components/AttributeComponent.h"


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
	
	CameraZoomInPos = CreateDefaultSubobject<USceneComponent>(TEXT("CameraZoomInPos"));
	CameraZoomInPos->SetupAttachment(GetRootComponent());
	
	CameraZoomOutPos = CreateDefaultSubobject<USceneComponent>(TEXT("CameraZoomOutPos"));
	CameraZoomOutPos->SetupAttachment(GetRootComponent());

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attribute Component"));

	CrosshairWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Crosshair Widget Component"));
	CrosshairWidgetComponent->SetupAttachment(RootComponent);

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

	WizardTimerManager = &GetWorld()->GetTimerManager();
	
	if (UUserWidget* Crosshair = CrosshairWidgetComponent->GetWidget()) {
		Crosshair->AddToViewport();
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

void AWizard::JumpCustom(const FInputActionValue& value) //doesn't finish yet
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
		CollidingItem->Equip(this, FName("HandRSocket"), this, this);		//using HandRSocket to attach weapons
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


void AWizard::Aim(const FInputActionValue& value)	//aim animation doesn't play properly!!!
{
	bool isAimValid = GetController() && value.Get<bool>() && GetPlayerWeapon() && GetWizardAState() != WizardActionState::Attacking  && GetWizardPState() != WizardPlayerState::Jumping;
	if (isAimValid) {
		SetWizardAState(WizardActionState::Aiming);
		RotateCameraToAim(true);
		ZoomCamera(true);		
	}

	else if (!value.Get<bool>() && GetPlayerWeapon()) // may be cancel attack if aiming cancelled? if(notAiming && Attacking) cancel attack
	{
		//set camera to default position here	
		SetWizardAState(WizardActionState::None);
		RotateCameraToAim(false);
		ZoomCamera(false);	
	}
}

void AWizard::Attack(const FInputActionValue& value) //animation gets stuck when player spams attack key
{																																	//currently player can attack only while aiming but it can be both while aiming or not aiming
	bool isAttackValid = GetController() && value.Get<bool>() && GetPlayerWeapon() && GetWizardAState() == WizardActionState::Aiming /* || GetWizardAState() != WizardActionState::Attacking*/ && GetWizardPState() != WizardPlayerState::Jumping;
	if (isAttackValid) {
		SetWizardAState(WizardActionState::Attacking);
		PlayerWeapon->Fire(this, ViewCamera);	//fire weapon
		SetAttackCooldown();
	}
}

void AWizard::SetAttackCooldown()
{
	FTimerHandle AttackCooldown;
	FTimerDelegate AttackCooldownDelegate = FTimerDelegate::CreateUObject(this, &AWizard::SetWizardAState, WizardActionState::None);
	GetWorldTimerManager().SetTimer(AttackCooldown, AttackCooldownDelegate, 1.5f, false);	//set cooldown for next attack
}

void AWizard::RotateCameraToAim(bool rotate) //it rotates capsule component to aiming pos by timer
{
	if (rotate) {
		if(!isCameraRotated && !WizardTimerManager->TimerExists(RotateCameraToAimTimer)) {
			//if camera rotation timer is not set
			FTimerDelegate RotateCameraToAimDelegate = FTimerDelegate::CreateUObject(this, &AWizard::PerformCameraRotationToAim);
			WizardTimerManager->SetTimer(RotateCameraToAimTimer, RotateCameraToAimDelegate, deltaTime, true, -1.0f);
			//UE_LOG(LogTemp, Warning, TEXT("	WizardTimerManager->SetTimer"));
		}
	}

	else{
		isCameraRotated = false;
		LockCamera(false);
		if (WizardTimerManager->TimerExists(RotateCameraToAimTimer)) {
			WizardTimerManager->ClearTimer(RotateCameraToAimTimer);
		}
	}
	
}

void AWizard::PerformCameraRotationToAim() {
	FRotator CapsuleRotation = GetCapsuleComponent()->GetComponentRotation();		//current rotation
	FRotator ControllerRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);	//target rotation (only use yaw rotation)

	if (!CapsuleRotation.Equals(ControllerRotation, 1)) {
		GetCharacterMovement()->bOrientRotationToMovement = false;
		FRotator NewRotation = FMath::RInterpTo(CapsuleRotation, ControllerRotation, deltaTime, 10.0f);
		GetCapsuleComponent()->SetWorldRotation(NewRotation);	//rotate capsule component to new rotation
		isCameraRotated = false;
	}

	else {
		isCameraRotated = true;
		WizardTimerManager->ClearTimer(RotateCameraToAimTimer);
		LockCamera(true);
		//UE_LOG(LogTemp, Warning, TEXT("	WizardTimerManager->ClearTimer"));
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

void AWizard::ZoomCamera(bool zoomIn)
{
	if (zoomIn && !WizardTimerManager->TimerExists(ZoomInCameraTimer)) {
		if(WizardTimerManager->TimerExists(ZoomOutCameraTimer))
			WizardTimerManager->ClearTimer(ZoomOutCameraTimer);
		FTimerDelegate ZoomCameraTimerDelegate = FTimerDelegate::CreateUObject(this, &AWizard::PerformZoomCamera, zoomIn);
		WizardTimerManager->SetTimer(ZoomInCameraTimer, ZoomCameraTimerDelegate, deltaTime, true, -1.0f);

	}

	else if (!zoomIn && !WizardTimerManager->TimerExists(ZoomOutCameraTimer)) {
		if (WizardTimerManager->TimerExists(ZoomInCameraTimer))
			WizardTimerManager->ClearTimer(ZoomInCameraTimer);
		FTimerDelegate ZoomCameraTimerDelegate = FTimerDelegate::CreateUObject(this, &AWizard::PerformZoomCamera, zoomIn);
		WizardTimerManager->SetTimer(ZoomOutCameraTimer, ZoomCameraTimerDelegate, deltaTime, true, -1.0f);
	}

}

void AWizard::PerformZoomCamera(bool zoomIn) {
	//1- change spring arm's location up to character right shoulder
	//2- then zoom in to specified pos
	FVector SpringArmCurrentPos = SpringArm->GetRelativeLocation();
	float springArmCurrentLength = SpringArm->TargetArmLength;

	FVector SpringArmTargetPos;
	float springArmTargetLength;

	if (zoomIn) {
		SpringArmTargetPos = CameraZoomInPos->GetRelativeLocation();
		springArmTargetLength = 0.0f;		//spring arm zoom in length
	}

	else {
		SpringArmTargetPos = CameraZoomOutPos->GetRelativeLocation();
		springArmTargetLength = 300.0f;		//spring arm zoom out length
	}

	if (!SpringArmCurrentPos.Equals(SpringArmTargetPos, 1) || (springArmCurrentLength != springArmTargetLength)) {
		FVector NewSpringArmPos = FMath::VInterpTo(SpringArmCurrentPos, SpringArmTargetPos, deltaTime, 10.0f);
		SpringArm->SetRelativeLocation(NewSpringArmPos);

		float newSpringArmLength = FMath::FInterpTo(springArmCurrentLength, springArmTargetLength, deltaTime, 10.0f);
		SpringArm->TargetArmLength = newSpringArmLength;
	}

	else if(zoomIn) {
		WizardTimerManager->ClearTimer(ZoomInCameraTimer);
	}

	else if (!zoomIn) {
		WizardTimerManager->ClearTimer(ZoomOutCameraTimer);
	}
}
