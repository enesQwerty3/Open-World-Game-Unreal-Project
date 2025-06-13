// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "Treasure.generated.h"

class UCapsuleComponent;
class UPrimitiveComponent;

UCLASS()
class ATreasure : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TreasureMesh = nullptr;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* TreasureCapsule = nullptr;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* TreasureEffect = nullptr;

	UPROPERTY(EditAnywhere)
	int8 Gold = 0; 

	// Sets default values for this actor's properties
	ATreasure();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void OnCapsuleComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, 
												int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

private:	

};
