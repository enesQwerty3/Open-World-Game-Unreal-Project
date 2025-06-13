// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/BreakableActorHitInterface.h"
#include "BreakableActor.generated.h"


class UGeometryCollectionComponent;
class ATreasure;
class UWidgetComponent;

UCLASS()
class MYOPENWORLDPROJECT_API ABreakableActor : public AActor, public IBreakableActorHitInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	/*UPROPERTY(EditAnywhere)
	UClass* Treasure = nullptr;
	*/
	UPROPERTY(EditAnywhere)
	TArray< TSubclassOf<ATreasure>> TreasureClasess;
	
	UPROPERTY(EditAnywhere)
	USceneComponent* TreasureSpawnPoint = nullptr;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* HealthBarWidget = nullptr;

	ABreakableActor();

	virtual void Tick(float DeltaTime) override;

	void GetHit_Implementation(const FVector& HitImpactPoint);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	bool TreasureSpawned = false;
	
	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollectionComponent;
};
