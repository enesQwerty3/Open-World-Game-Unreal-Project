// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Wizard.h"
#include "Items/Item.h"

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

AWizard::AWizard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	WizardPState = WizardPlayerState::Created;

	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;		//change character rotation to direction of acceleration
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);	//rotarion rate

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));	//assign SpringArm object
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bUsePawnControlRotation = true;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));		//assign Camera object
	ViewCamera->SetupAttachment(SpringArm);
				
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
	SetRunningTime(runningTime + DeltaTime);
	FString Message = FString::Printf(TEXT("RunningTime: %f - DeltaTime: %f - Sin(100): %f"), this->runningTime, DeltaTime, FMath::Sin(100.0f));
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Green, Message);  //on screen message
	}

}

void AWizard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWizard::Move);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWizard::Look);
		
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AWizard::JumpCustom); //Character jump function
		//EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &AWizard::JumpCustom);
		
		EnhancedInput->BindAction(EquipItemAction, ETriggerEvent::Triggered, this, &AWizard::EquipItem);
		//EnhancedInput->BindAction(EquipItemAction, ETriggerEvent::Completed, this, &AWizard::EquipItem);	//may be completed action function different function
		
		EnhancedInput->BindAction(RunAction, ETriggerEvent::Triggered, this, &AWizard::Run);
		//EnhancedInput->BindAction(RunAction, ETriggerEvent::Completed, this, &AWizard::Run);
	}
}

void AWizard::Move(const FInputActionValue& value) {
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

void AWizard::Look(const FInputActionValue& value) {
	bool isLookValid = GetController() ? true : false;

	if (isLookValid) {
		const FVector2D MouseMovementVector = value.Get<FInputActionValue::Axis2D>();
		AddControllerPitchInput(MouseMovementVector.Y);
		AddControllerYawInput(MouseMovementVector.X);
	}
}

void AWizard::JumpCustom(const FInputActionValue& value) {
	bool isJumpValid = GetController() ? true : false;

	if (isJumpValid) {
		if (value.Get<bool>()){
			ACharacter::Jump();
			WizardPState = WizardPlayerState::Jumping;
		}
	}
}

void AWizard::EquipItem(const FInputActionValue& value){
	bool isEquipItemValid = GetController() ? true : false;

	if (isEquipItemValid) {
		if (CollidingItem != nullptr && value.Get<bool>()) {
			CollidingItem->Equip(this, FName("WeaponSocket"));
			SetWizardIState(WizardItemState::EquippedWeapon);
		}

	}
}

void AWizard::Run(const FInputActionValue& value){
	bool isRunValid = GetController() && GetWizardPState() != WizardPlayerState::Jumping && GetWizardPState() == WizardPlayerState::Moving;

	if (isRunValid) {
		if (value.Get<bool>()) {
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			SetWizardPState(WizardPlayerState::Running);
		}
	
		else {
			GetCharacterMovement()->MaxWalkSpeed = 300.0f;
		}

	}
}
