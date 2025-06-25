#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/EnemyHitInterface.h"
#include "Enemy.generated.h"

class AAIController;
class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EEnemyState : uint8 {
	EES_Spawned,
	EES_Patrol,
	EES_Alert,	//if enemy sees a player
	EES_Chase,	//if enemy gets hit by player
	EES_Attack
};

UCLASS()
class MYOPENWORLDPROJECT_API AEnemy : public ACharacter, public IEnemyHitInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	AActor* PatrolTarget = nullptr;		//will be satisfied by AIManager
	
	UPROPERTY(EditAnywhere)				// "" "" ""	""	""
	AActor* ChaseTarget = nullptr;	
	
	UPROPERTY(EditAnywhere)
	AActor* ChasingActor = nullptr;		//store which actor is being chased by enemy

	UPROPERTY(EditAnywhere)
	EEnemyState EnemyState;

	AEnemy();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void SetEnemyState(EEnemyState state) { EnemyState = state; }

	FORCEINLINE EEnemyState GetEnemyState() { return EnemyState; }

	UFUNCTION()
	virtual void GetHitActor(FVector ImpactPoint, AActor* Actor);


private:
	UPROPERTY()
	float SightRadius = 1000.f;
	
	//Components
	UPROPERTY(EditAnywhere)
	UPawnSensingComponent* PawnSensingComponent = nullptr;

	AAIController* EnemyController = nullptr;

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void MoveToTarget(AActor* Target);

	UFUNCTION()
	void CheckIfActorCatched();

	UFUNCTION()
	void CheckIfActorOutOfLOS(AActor* Actor);

	UFUNCTION()
	void StopMovingToTarget();


protected:
	virtual void BeginPlay() override;


};
