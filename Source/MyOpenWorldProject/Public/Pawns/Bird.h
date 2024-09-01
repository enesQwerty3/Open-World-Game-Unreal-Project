// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"	//FInputActionValue struct header
#include "Bird.generated.h"  // this header file has to be last header file in order to reflection system work properly!!!

class UCapsuleComponent;
class USkeletalMeshComponent;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;


UCLASS()
class MYOPENWORLDPROJECT_API ABird : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABird();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);

private:
	UPROPERTY(VisibleAnywhere,Category = "Bird Capsule")
	UCapsuleComponent* Capsule;		//class keyword means this is a forward decleration which is incomplete type
	
	UPROPERTY(VisibleAnywhere, Category = "Bird Capsule")
	USkeletalMeshComponent* BirdMesh;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* BirdMappingContext;	//assigned in blueprint

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MoveAction;					//assigned in blueprint

	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& value);

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;					//assigned in blueprint

	UFUNCTION(BlueprintCallable)
	void Look(const FInputActionValue& value);

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* FlyAction;

	UFUNCTION(BlueprintCallable)
	void Fly(const FInputActionValue& value);

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* ViewCamera;
};
