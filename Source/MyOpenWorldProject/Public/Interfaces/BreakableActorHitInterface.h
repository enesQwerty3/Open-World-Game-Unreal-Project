// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BreakableActorHitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBreakableActorHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYOPENWORLDPROJECT_API IBreakableActorHitInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)	//blueprint overrideble function GetHit_Implementation can be used in c++ not GetHit directly!
	void GetHit(const FVector& ImpactPoint, float damage);
};
