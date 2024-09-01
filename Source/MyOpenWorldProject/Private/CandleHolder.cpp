// Fill out your copyright notice in the Description page of Project Settings.


#include "CandleHolder.h"

ACandleHolder::ACandleHolder()
{
	//Super::SetActorTickEnabled(false); //how to disable parent tick function?
	PrimaryActorTick.bCanEverTick = true;
	CandleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CandleMesh"));
	CandleMesh->SetupAttachment(GetRootComponent());
}

void ACandleHolder::BeginPlay()
{
	AActor::BeginPlay();	//call actor clss BeginPlay function
	
}

void ACandleHolder::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);	//call actor class Tick function

	this->SetRunningTime(GetRunningTime() + DeltaTime);

	if (GEngine)
	{
		FString Message = FString::Printf(TEXT("CandleHolder running time: %f - delta time: %f"), GetRunningTime(), DeltaTime);
		GEngine->AddOnScreenDebugMessage(3, 1.0f, FColor::Red, Message);  //on screen message
	}

}

