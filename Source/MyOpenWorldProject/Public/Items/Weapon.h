// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class AWizard;
/**
 * 
 */
UCLASS()
class MYOPENWORLDPROJECT_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	void Equip(AWizard* Player, FName SocketName) override;

protected:

private:
	//we can't have UFUNCTION() macro on override functions
	void SphereOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void SphereOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
