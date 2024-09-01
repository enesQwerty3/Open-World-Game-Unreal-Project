// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "CandleHolder.generated.h"

/**
 * 
 */
UCLASS()
class MYOPENWORLDPROJECT_API ACandleHolder : public AItem
{
	GENERATED_BODY()
	
public:
	ACandleHolder();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CandleMesh;
	
};
