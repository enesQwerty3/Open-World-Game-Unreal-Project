// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"	//FInputActionValue struct header
#include "Wizard.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class AItem;

UENUM(BlueprintType)
enum class WizardPlayerState : uint8 { None, Created, Idling, Moving, Running, Jumping, Attacking, Defending, Destroyed };

UENUM(BlueprintType)
enum class WizardItemState : uint8 { None, EquippedItem, EquippedWeapon };


UCLASS()
class MYOPENWORLDPROJECT_API AWizard : public ACharacter
{
	GENERATED_BODY()


public:
	AWizard();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetRunningTime(float _runnigTime){ runningTime = _runnigTime; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetRunningTime() { return runningTime; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCollidingItem(AItem* Item) { CollidingItem = Item; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE AItem* GetCollidingItem() { return CollidingItem; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetWizardPState(WizardPlayerState _WizardPState){ WizardPState = _WizardPState; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE WizardPlayerState GetWizardPState() { return WizardPState; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetWizardIState(WizardItemState _WizardIState) { WizardIState = _WizardIState; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE WizardItemState GetWizardIState() { return WizardIState; }

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(EditAnywhere, Category = "WIZARD PLAYER STATE")
	WizardPlayerState WizardPState = WizardPlayerState::None;		//player state

	UPROPERTY(EditAnywhere, Category = "WIZARD PLAYER STATE")
	WizardItemState WizardIState = WizardItemState::None;			//player item state

	UPROPERTY(EditAnywhere, Category = "TIME")
	float runningTime = 0;						//actor running time

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* ViewCamera;	

	UPROPERTY(EditAnyWhere, Category = "INPUT")
	UInputMappingContext* InputMappingContext;	//assigned in BP_Wizard

	UPROPERTY(EditAnywhere, Category = "INPUT")
	UInputAction* MoveAction;	//assigned in BP_Wizard	we don't instantiate input action objects

	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& value);

	UPROPERTY(EditAnywhere, Category = "INPUT")
	UInputAction* LookAction;	//assigned in BP_Wizard

	UFUNCTION(BlueprintCallable)
	void Look(const FInputActionValue& value);

	UPROPERTY(EditAnywhere, Category = "INPUT")
	UInputAction* JumpAction;	//assigned in BP_Wizard

	UFUNCTION(BlueprintCallable)
	void JumpCustom(const FInputActionValue& value);	//for now I will use ACharacter::Jump function to perform jump action!!!

	UPROPERTY(EditAnywhere, Category = "INPUT")
	UInputAction* EquipItemAction;	//assigned in BP_Wizard

	UFUNCTION(BlueprintCallable)
	void EquipItem(const FInputActionValue& value);

	UPROPERTY(EditAnywhere, Category = "ITEM")
	AItem* CollidingItem = nullptr;

	UPROPERTY(EditAnywhere, Category = "INPUT")
	UInputAction* RunAction;

	UFUNCTION(BlueprintCallable)
	void Run(const FInputActionValue& value);
};


