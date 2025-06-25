// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Weapon.h"
#include "Components/SphereComponent.h"
#include "Characters/Wizard.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "Interfaces/HitInterface.h"
#include "Interfaces/BreakableActorHitInterface.h"
#include "Interfaces/EnemyHitInterface.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{


}


void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (GetMyItemState() == ItemState::NotEquipped) {	//because some weapons can be spawned equipped check when spawned
		GetStaticMesh()->SetSimulatePhysics(true);	//simulate physics, doesn't work inside constructor function
		GetStaticMesh()->SetMassOverrideInKg(FName("NAME_None"), 100.0f, true);
	}
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AWeapon::SphereOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::SphereOnComponentBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex ,bFromSweep, SweepResult); //prints other colliding actor's name

	AWizard* Player = Cast<AWizard>(OtherActor);
	bool canAttachToPlayer = Player && Player->GetCollidingItem() == nullptr && GetMyItemState() == ItemState::NotEquipped;
	if (canAttachToPlayer) {
		Player->SetCollidingItem(this);
	}
}


void AWeapon::SphereOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::SphereOnComponentEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);	//prints other no more colliding actor's name
	
	AWizard* Player = Cast<AWizard>(OtherActor);
	bool cannotAttachToPlayer = Player && GetMyItemState() == ItemState::NotEquipped;
	if (cannotAttachToPlayer) {
		Player->SetCollidingItem(nullptr);
	}
}


void AWeapon::Equip(AWizard* Player, FName SocketName, AActor* equippedActor, APawn* equippedInstigator)
{
	ItemMesh->SetSimulatePhysics(false);		//stop simulating physics
	SphereCollision->SetGenerateOverlapEvents(false);	//stop sphere collision events
	const FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(Player->GetMesh(), TransformRules, SocketName);	//attach item to player
	MyItemState = ItemState::Equipped;		//change this item's state
	Player->SetCollidingItem(nullptr);		//clear player colliding item property	
	Player->SetPlayerWeapon(this);			//set player weapon	property
	SetOwner(equippedActor);
	SetInstigator(equippedInstigator);
}

void AWeapon::Unequip(AWizard* Player, FName SocketName)
{
	/*
	***
	**
	* 
	*/
	SetMyItemState(ItemState::Holstered);
}

void AWeapon::Drop(AWizard* Player, FName SocketName)
{
	const FDetachmentTransformRules TransformRules(EDetachmentRule::KeepWorld, true);
	ItemMesh->DetachFromComponent(TransformRules);
	Player->SetPlayerWeapon(nullptr);
	ItemMesh->SetSimulatePhysics(true);
	SphereCollision->SetGenerateOverlapEvents(true); //start again sphere collision events
	MyItemState = ItemState::NotEquipped;

}

void AWeapon::Fire(AWizard* Player, UCameraComponent* Camera)
{
	/*
		create new projectile class
		does weapon class have to has a projectile instance or array of projectiles? May be projectiles destroyed in their classes?
		projectile gets fired here this means projectile actor will be spawned in weapon's fire method
		InitialLifeSpan = 3.0f;
	*/

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(Player);

	FHitResult OutHit = FHitResult();	//hit result

	FVector CameraPos = Camera->GetComponentLocation();
	FVector CameraForwardVector = Camera->GetForwardVector();
	
	//draw a LineTrace if projectile hits something, start point is where player fired the weapon and end point is where projectile hit something
	UKismetSystemLibrary::LineTraceSingle(
										this,
										CameraPos,
										CameraPos + CameraForwardVector * weaponRange,
										ETraceTypeQuery::TraceTypeQuery1,
										false,
										ActorsToIgnore,
										EDrawDebugTrace::ForDuration,
										OutHit,
										true,
										FLinearColor::Red,
										FLinearColor::Green,
										5.f);
	
	if (OutHit.GetActor()) {
		AActor* OtherActor = OutHit.GetActor();
		FString OtherActorName = OtherActor->GetName();
		
		UE_LOG(LogTemp, Warning, TEXT("OtherActorName: %s"), *OtherActorName);
		
		//if actor has a IHitInterface
		if (IHitInterface* OtherActorHitInterface = Cast<IHitInterface>(OtherActor)) {//changed cast to cast checked
			OtherActorHitInterface->GetHit(OutHit.ImpactPoint);
		}

		//if actor has a IBreakableActorHitInterface
		else if(IBreakableActorHitInterface* OtherBreakableActorHitInterface = Cast<IBreakableActorHitInterface>(OtherActor)){
			OtherBreakableActorHitInterface->Execute_GetHit(OutHit.GetActor(), OutHit.ImpactPoint, weaponDamage); //executes Get Hit bp native event
			UGameplayStatics::ApplyDamage(OtherActor, weaponDamage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
			CreateFields(OutHit.Location); 
		}

		else if (IEnemyHitInterface* OtherEnemyActorHitInterface = Cast<IEnemyHitInterface>(OtherActor)) {
			OtherEnemyActorHitInterface->GetHitActor(OutHit.ImpactPoint, GetOwner());
		}
	}
}