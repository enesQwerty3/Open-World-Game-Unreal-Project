#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/BreakableActorHitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;
class ATreasure;
class UHealthBarComponent;
class UAttributeComponent;

UCLASS()
class MYOPENWORLDPROJECT_API ABreakableActor : public AActor, public IBreakableActorHitInterface
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ATreasure>> TreasureClasess;
	
	UPROPERTY(EditAnywhere)
	USceneComponent* TreasureSpawnPoint = nullptr;

	UPROPERTY(EditAnywhere)
	UHealthBarComponent* HealthBarComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollectionComponent = nullptr;
	
	UPROPERTY(EditAnywhere)
	UAttributeComponent* AttributeComponent = nullptr;

	ABreakableActor();

	virtual void Tick(float DeltaTime) override;

	void GetHit_Implementation(const FVector& HitImpactPoint, float damage);

	UFUNCTION(BlueprintCallable)
	void SpawnTreasure();


protected:
	virtual void BeginPlay() override;

	//UGameplayStatics:ApplyDamage will call AActor::TakeDamage function
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


private:
	bool TreasureSpawned = false;	

};
