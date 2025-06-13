// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:	
	UPROPERTY(VisibleAnywhere)
	float Healt = 100.f;
	
	UPROPERTY(VisibleAnywhere)
	float MaxHealt = 100.f;
	
	UPROPERTY(VisibleAnywhere)
	int32 Exp = 0;
	
	UPROPERTY(VisibleAnywhere)
	int32 Gold = 0;
};
