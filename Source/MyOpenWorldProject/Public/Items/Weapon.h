// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"

#include "Weapon.generated.h"

class UCameraComponent;
class AWizard;
/**
 * 
 */
UCLASS()
class MYOPENWORLDPROJECT_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();
	
	virtual void Tick(float DeltaTime) override;

	//we can't have UFUNCTION() macro on override functions
	void Equip(AWizard* Player, FName SocketName) override;

	void Unequip(AWizard* Player, FName SocketName) override;

	void Drop(AWizard* Player, FName SocketName) override;
	
	UFUNCTION(BlueprintCallable)
	void Fire(AWizard* Player, UCameraComponent* Camera);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetWeaponRange() { return weaponRange;}

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& location);

protected:

	virtual void BeginPlay() override;
	

private:
	//we can't have UFUNCTION() macro on override functions
	void SphereOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void SphereOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	FORCEINLINE void SetWeaponRange(float _weaponRange) {weaponRange = _weaponRange; }
	
	UPROPERTY(EditAnywhere, Category = "WEAPON SPECS")
	float weaponRange = 10000.0f;
};
