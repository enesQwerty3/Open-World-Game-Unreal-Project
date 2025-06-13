// Fill out your copyright notice in the Description page of Project Settings.

#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

//EnhancedInput
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
//EnhancedInput

//camera and spring arm
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
//camera and spring arm


// Sets default values
ABird::ABird()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Bird Capsule"));
	Capsule->SetCapsuleHalfHeight(20.0f);
	Capsule->SetCapsuleRadius(15.0f);

	//set capsule collision
	Capsule->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	Capsule->SetGenerateOverlapEvents(true);
	
	SetRootComponent(Capsule);	//set capsule root component
	
	//set mesh collision
	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bird Skeletal Mesh"));
	BirdMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BirdMesh->SetGenerateOverlapEvents(false);
	BirdMesh->SetupAttachment(GetRootComponent());	//instead of RootComponent argument, Capsule or GetRootComponent can be used
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Capsule);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	ViewCamera->SetupAttachment(SpringArm);
	//AutoPossessPlayer = EAutoReceiveInput::Player0;				//gets possesion of a player
}											//Player0 identifies us

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();
						//add mapping context to BP_Bird
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {	//cast AController to APlayerController
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(BirdMappingContext, 0);  //BirdMappingContext has to be asssigned in BP_Bird!!!
		}
	}
}

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent); //calling parent function
	//PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABird::MoveForward);
	
	//binding input actions to mapping context
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);	//Cast old input system to new enhanced input system
	if (EnhancedInputComponent) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);
										//MoveAction has to be assigned in BP_Bird
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::Look);
		EnhancedInputComponent->BindAction(FlyAction, ETriggerEvent::Triggered, this, &ABird::Fly);
	}										
}

void ABird::MoveForward(float value) {		//doesn't work
	if (GetController() && value != 0.0f) {
		FVector ForwardVector = GetActorForwardVector();
		AddMovementInput(ForwardVector, value);
	}
}

void ABird::Move(const FInputActionValue& value) {
	if (GetController()) {
		const FVector2D movementVector = value.Get<FInputActionValue::Axis2D>();
		
		//AddMovementInput(GetActorForwardVector(),movementVector.X);	//X direction movement depends on actors forward vector
 		//AddMovementInput(GetActorRightVector(), movementVector.Y);	//Y	   ""       ""       ""    ""   ""   right vector
		
		//rotate pawn to controller direction
		const FRotator ControllerRotation = Controller->GetControlRotation();
		const FRotator ControllerYawRotation(0.0f, ControllerRotation.Yaw, 0.0f);	

		FVector ForwardDirection = FRotationMatrix(ControllerYawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, movementVector.X);
		
		FVector RightDirection = FRotationMatrix(ControllerYawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, movementVector.Y);
		
		/*
		const FRotator PitchRotation(ControllerRotation.Pitch, 0.0f, 0.0f);
		FVector UpwardDirection = FRotationMatrix(PitchRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(UpwardDirection, 1.0f);
		*/

		UE_LOG(LogTemp, Warning, TEXT("YawRotation.X: %f - YawRotation.Y: %f - movementVector.X: %f - movementVector.Y: %f"), ForwardDirection.X, ForwardDirection.Y, movementVector.X, movementVector.Y);
		
	}
}

void ABird::Look(const FInputActionValue& value) {
	if (GetController()) {
		const FVector2D lookVector = value.Get<FInputActionValue::Axis2D>();
		AddControllerPitchInput(lookVector.Y);
		AddControllerYawInput(lookVector.X);
	}
}

void ABird::Fly(const FInputActionValue& value) {//can't fly properly Controller Pitch.Y is always 1.0f why?
	if (GetController() && value.Get<bool>()) {
		
		const FRotator ControllerRotation  = Controller->GetControlRotation();
		const FRotator ControllerUpwardRotation(ControllerRotation.Pitch, 0.0f, 0.0f);

		const FVector UpwardDirection = FRotationMatrix(ControllerUpwardRotation).GetUnitAxis(EAxis::Y);
		const FVector NotUpwardDirection = FRotationMatrix(ControllerUpwardRotation).GetUnitAxis(EAxis::X);
		
		UE_LOG(LogTemp, Warning, TEXT("cPitch: %f - cPitch.X: %f - c.Pitch.Y: %f"), ControllerUpwardRotation.Pitch, NotUpwardDirection.X, UpwardDirection.Y);
		
		//AddMovementInput(UpwardDirection, 1.0f);
		//UE_LOG(LogTemp, Warning, TEXT("UpwardDirection.X: %f - UpwardDirection.Y: %f"), UpwardDirection.X, UpwardDirection.Y);
	}
}

void ABird::GetHit(FVector ImpactPoint)
{
	UE_LOG(LogTemp, Warning, TEXT("Bird get hit!"));
	
	float dotProduct = FVector::DotProduct(GetActorForwardVector(), (ImpactPoint - GetActorLocation()).GetSafeNormal());	//
	float degreeBetweenHitAndForward = FMath::RadiansToDegrees(FMath::Acos(dotProduct));
	FVector CrossProduct = FVector::CrossProduct(GetActorForwardVector(), (ImpactPoint - GetActorLocation()).GetSafeNormal());
	if(CrossProduct.Z < 0)
		degreeBetweenHitAndForward*=-1;
	DrawDebugDirectionalArrow(GetWorld(), ImpactPoint, ImpactPoint + GetActorForwardVector() * 100.f, 100.f, FColor::Red, false, 5.f);
	DrawDebugDirectionalArrow(GetWorld(), ImpactPoint, ImpactPoint + CrossProduct.GetSafeNormal() * 100.f, 100.f, FColor::Blue, false, 5.f);
	DrawDebugDirectionalArrow(GetWorld(), ImpactPoint, ImpactPoint + (ImpactPoint - GetActorLocation()).GetSafeNormal() * 100.f, 100.f, FColor::Magenta, false, 5.f);
	UE_LOG(LogTemp, Display, TEXT("Degree between: %lf"), degreeBetweenHitAndForward);
}

