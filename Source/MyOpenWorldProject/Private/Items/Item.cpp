// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Item.h"
#include "Components/SphereComponent.h"
#include "Characters/Wizard.h"

// Sets default values
AItem::AItem()
{
	MyItemState = ItemState::NotEquipped;	//default item state for not yet interacted items
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));  //unreal factory method
	SetRootComponent(ItemMesh);
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ShpereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	
}
	 
// Called when the game starts or when spawned
void AItem::BeginPlay(){
	Super::BeginPlay();
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AItem::SphereOnComponentBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AItem::SphereOnComponentEndOverlap);
}


// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetRunningTime(this->runningTime + DeltaTime);
	FString Message = FString::Printf(TEXT("RunningTime: %f - DeltaTime: %f - Sin(100): %f"), this->runningTime, DeltaTime, FMath::Sin(100.0f));
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Green, Message);  //print running time as on screen message
	}
}


void AItem::SphereOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FString OtherActorName(OtherActor->GetName());
	FString ThisActorName(this->GetName());
	UE_LOG(LogTemp, Warning, TEXT("%s begin Overlap with: %s"), *ThisActorName, *OtherActorName);
}

void AItem::SphereOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	FString OtherActorName(OtherActor->GetName());
	FString ThisActorName(this->GetName());
	UE_LOG(LogTemp, Warning, TEXT("%s end overlap with: %s"), *ThisActorName, *OtherActorName);
}


void AItem::Equip(AWizard* Player, FName SocketName, AActor* equippedActor, APawn* equippedInstigator)
{
	UE_LOG(LogTemp, Display, TEXT("Equipped Item"));
}

void AItem::Unequip(AWizard* Player, FName SocketName)
{
	UE_LOG(LogTemp, Display, TEXT("Unequipped Item"));
}

void AItem::Drop(AWizard* Player, FName SocketName)
{
	UE_LOG(LogTemp, Display, TEXT("Dropped Item"));
}

