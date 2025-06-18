// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

class UHealthBarWidget;

UCLASS()
class UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UHealthBarComponent();

	UFUNCTION()
	void UpdateHealthBarPercent(float _value);

private:
	UPROPERTY()
	UHealthBarWidget* HealthBarWidget = nullptr;



protected: 


};
