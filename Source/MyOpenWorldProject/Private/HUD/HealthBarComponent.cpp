// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"
#include "HUD/HealthBarWidget.h"
#include "Components/ProgressBar.h"

UHealthBarComponent::UHealthBarComponent()
{

}

void UHealthBarComponent::UpdateHealthBarPercent(float _value)
{
	if (!HealthBarWidget){
		HealthBarWidget = Cast<UHealthBarWidget>(GetUserWidgetObject());
		UE_LOG(LogTemp, Warning, TEXT("HealthBarWidget created!"));
	}

	if (_value >= 0 && HealthBarWidget) {
		HealthBarWidget->HealthBar->SetPercent(_value);
		float percent = HealthBarWidget->HealthBar->GetPercent();
		UE_LOG(LogTemp, Warning, TEXT("Progressbar percent: %f"), percent);
	}

}

