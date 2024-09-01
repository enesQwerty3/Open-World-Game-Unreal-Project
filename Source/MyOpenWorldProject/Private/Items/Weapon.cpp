// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Weapon.h"
#include "Characters/Wizard.h"


void AWeapon::SphereOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::SphereOnComponentBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex ,bFromSweep, SweepResult);

	AWizard* Player = Cast<AWizard>(OtherActor);
	bool canAttachToPlayer = Player && Player->GetCollidingItem() == nullptr && GetMyItemState() != ItemState::Equipped;
	if (canAttachToPlayer) {
		Player->SetCollidingItem(this);
	}
}

void AWeapon::SphereOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::SphereOnComponentEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	
	AWizard* Player = Cast<AWizard>(OtherActor);
	bool canDetachFromPlayer = Player && GetMyItemState() == ItemState::NotEqquipped;
	if (canDetachFromPlayer) {
		Player->SetCollidingItem(nullptr);
	}
}

void AWeapon::Equip(AWizard* Player, FName SocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(Player->GetMesh(), TransformRules, SocketName);
	SetMyItemState(ItemState::Equipped);
	Player->SetCollidingItem(nullptr);			//item equipped but collision between item and player countiues
}
