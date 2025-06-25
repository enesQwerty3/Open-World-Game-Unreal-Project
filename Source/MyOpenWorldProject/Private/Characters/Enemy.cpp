#include "Characters\Enemy.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"

AEnemy::AEnemy()
{
 	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Component"));
	PawnSensingComponent->SightRadius = SightRadius;
	PawnSensingComponent->HearingThreshold = 600.f;
	PawnSensingComponent->SetPeripheralVisionAngle(30.f);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	EnemyController = Cast<AAIController>(GetController());
	EnemyState = EEnemyState::EES_Spawned;

}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckIfActorOutOfLOS(ChasingActor);
	CheckIfActorCatched();
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("Pawn Seen!"));
	SetEnemyState(EEnemyState::EES_Alert);
	MoveToTarget(SeenPawn);
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController && (ChasingActor == nullptr || ChasingActor != Target) && !GetActorLocation().Equals(Target->GetActorLocation(), 20.f)) {
		EnemyController->MoveToActor(Target, 20.f);
		ChasingActor = Target;
	}
}

void AEnemy::CheckIfActorCatched()
{
	if ((EnemyState == EEnemyState::EES_Chase || EnemyState == EEnemyState::EES_Alert) && ChasingActor && GetActorLocation().Equals(ChasingActor->GetActorLocation(), 100.f)) {
		UE_LOG(LogTemp, Warning, TEXT("CheckIfActorCatched called StopChasing."));
		StopMovingToTarget();
	}
}

void AEnemy::StopMovingToTarget()
{
	UE_LOG(LogTemp, Warning, TEXT("StopChasing called."));
	ChasingActor = nullptr;
	SetEnemyState(EEnemyState::EES_Patrol);

}

void AEnemy::CheckIfActorOutOfLOS(AActor* Actor)
{
	if (EnemyState == EEnemyState::EES_Alert && Actor) {
		FVector Distance = FVector(GetActorLocation() - Actor->GetActorLocation());
		float DistanceBetween = Distance.Length();
		UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), DistanceBetween)
		if (DistanceBetween >= SightRadius) {
			StopMovingToTarget();
			MoveToTarget(PatrolTarget);
		}
	}
}

void AEnemy::GetHitActor(FVector ImpactPoint, AActor* Actor)
{
	SetEnemyState(EEnemyState::EES_Chase);
	MoveToTarget(Actor);
}