// Fill out your copyright notice in the Description page of Project Settings.


#include "Treasure.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Characters/Wizard.h"

// Sets default values
ATreasure::ATreasure()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TreasureCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Treasure Capsule"));
	TreasureCapsule->SetSimulatePhysics(true);
	TreasureCapsule->SetEnableGravity(true);
	TreasureCapsule->SetGenerateOverlapEvents(true);
	TreasureCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TreasureCapsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	SetRootComponent(TreasureCapsule);
	
	TreasureMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Treasure Mesh"));
	TreasureMesh->SetSimulatePhysics(false);
	TreasureMesh->SetEnableGravity(false);
	TreasureMesh->SetGenerateOverlapEvents(true);
	TreasureMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TreasureMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	TreasureMesh->SetupAttachment(RootComponent);
	
	TreasureEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Treasure Effect"));
	TreasureEffect->SetupAttachment(RootComponent);	
	
}

// Called when the game starts or when spawned
void ATreasure::BeginPlay()
{
	Super::BeginPlay();
	TreasureCapsule->OnComponentBeginOverlap.AddDynamic(this, &ATreasure::OnCapsuleComponentBeginOverlap);	//always put this things to the beginplay not constructure!
}

void ATreasure::OnCapsuleComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (OtherActor) {
		if (AWizard* Player = Cast<AWizard>(OtherActor)) {
			Destroy();
			UE_LOG(LogTemp, Warning, TEXT("Overlap with Wizard."));
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Treasure's Capsule Overlaped With Something."));
}

// Called every frame
void ATreasure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

