// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

class UProgressBar;

UCLASS()
class MYOPENWORLDPROJECT_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	float progressBarPercent = 0;

	UPROPERTY(meta = (BindWidget))		//this meta specifier automatically attach this variable to the same named progressbar in blueprint
		UProgressBar* HealthBar = nullptr;

private:
	

protected:

};
