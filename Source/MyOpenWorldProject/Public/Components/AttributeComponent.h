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

	UFUNCTION(BlueprintCallable)
	void RecieveDamage(float Damage);

protected:
	virtual void BeginPlay() override;

private:	
	UPROPERTY(VisibleAnywhere)
	float health = 100.f;
	
	UPROPERTY(VisibleAnywhere)
	float maxHealth = 100.f;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetHealth(float value) {health = value;}
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHealth() {return health;}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetMaxHealth(float value) {maxHealth = value;}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxHealth() {return maxHealth;}
};
