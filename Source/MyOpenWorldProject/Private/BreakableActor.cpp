// Fill out your copyright notice in the Description page of Project Settings.

#include "BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Treasure.h"
#include "HUD/HealthBarComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollectionComponent"));
	SetRootComponent(GeometryCollectionComponent);
	GeometryCollectionComponent->SetGenerateOverlapEvents(true);

	TreasureSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Treasure Spawn Point"));
	TreasureSpawnPoint->SetupAttachment(RootComponent);

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar Widget"));
	HealthBarWidget->SetupAttachment(RootComponent);

}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActor::GetHit_Implementation(const FVector& HitImpactPoint) //implementation may be called from blueprint Get Hit event.
{	//only call GetHit_Implementation to spawn treasure!
	UE_LOG(LogTemp, Warning, TEXT("GetHit_Implementation Called!"));
	if (GetWorld() && !TreasureClasess.IsEmpty() && !TreasureSpawned) {
		FVector Location = TreasureSpawnPoint->GetComponentLocation();
		int TreasureIndex = FMath::RandRange(0, TreasureClasess.Num() - 1);
		GetWorld()->SpawnActor<ATreasure>(TreasureClasess[TreasureIndex], Location, GetActorRotation());
		TreasureSpawned = true;		//need to check if teasure spawned because breake event fires this function more than once.
	}
}

