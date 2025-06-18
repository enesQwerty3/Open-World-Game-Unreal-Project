#include "Actors/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Actors/Treasure.h"
#include "HUD/HealthBarComponent.h"
#include "HUD/HealthBarWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/AttributeComponent.h"

// Sets default values
ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollectionComponent"));
	GeometryCollectionComponent->SetGenerateOverlapEvents(true);
	SetRootComponent(GeometryCollectionComponent);

	TreasureSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Treasure Spawn Point"));
	TreasureSpawnPoint->SetupAttachment(RootComponent);
	
	HealthBarComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar Component"));
	HealthBarComponent->SetupAttachment(RootComponent);

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attribute Component"));
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
}

void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActor::GetHit_Implementation(const FVector& HitImpactPoint, float damage) //implementation may be called from blueprint Get Hit event.
{	//only call GetHit_Implementation to spawn treasure!
	UE_LOG(LogTemp, Warning, TEXT("GetHit_Implementation called!"));
	SpawnTreasure();
}

void ABreakableActor::SpawnTreasure()
{
	if (GetWorld() && !TreasureClasess.IsEmpty() && !TreasureSpawned) {
		FVector Location = TreasureSpawnPoint->GetComponentLocation();
		int TreasureIndex = FMath::RandRange(0, TreasureClasess.Num() - 1);
		GetWorld()->SpawnActor<ATreasure>(TreasureClasess[TreasureIndex], Location, GetActorRotation());
		TreasureSpawned = true;		//need to check if teasure spawned because breake event fires this function more than once.
	}
}

void ABreakableActor::GetDamage(float damage)
{
	//if (Health > 0) {
	//	Health = FMath::Clamp(Health-=damage, 0.f, 100.f); //reduce health
	//	if (HealthBarComponent) {
	//		HealthBarComponent->UpdateHealthBarPercent(Health/MaxHealth);
	//		//actor damaged
	//		UE_LOG(LogTemp, Warning, TEXT("Health bar component not null!"));
	//	}

	//	//HealthBarComponent isn't null	
	//}

	//else {
	//	//actor dead
	//}

	//UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);
}

float ABreakableActor::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	AActor* DamageCauserOwner = DamageCauser->GetOwner() ? DamageCauser->GetOwner() : DamageCauser;

	if (AttributeComponent) {
		AttributeComponent->RecieveDamage(DamageAmount);
		
		if (HealthBarComponent) {
			if (AttributeComponent->GetHealth() > 0) { //reduce health
				HealthBarComponent->UpdateHealthBarPercent(AttributeComponent->GetHealth() / AttributeComponent->GetMaxHealth());
				//actor damaged
				//UE_LOG(LogTemp, Warning, TEXT("Health bar component not null!"));
			}

			else {
				//actor dead
			}

			UE_LOG(LogTemp, Warning, TEXT("Health: %f"), AttributeComponent->GetHealth());
		}

		UE_LOG(LogTemp, Warning, TEXT("Damage caused by %s"), *DamageCauserOwner->GetClass()->GetName());
	}
	
	return 0.0f;
}

