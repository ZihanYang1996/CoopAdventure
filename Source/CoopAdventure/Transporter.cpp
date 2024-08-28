// Fill out your copyright notice in the Description page of Project Settings.


#include "Transporter.h"

// Sets default values for this component's properties
UTransporter::UTransporter() :
	StartPoint(FVector::Zero()),
	EndPoint(FVector::Zero()),
	ActivatedTriggerCount(0),
	bAreAllTriggerActorsActivated(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

	// ...
}


// Called when the game starts
void UTransporter::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UTransporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UTransporter::SetPoints(const FVector& Start, const FVector& End)
{
	if (Start.Equals(End))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Start and End points are the same!");
		return;
	}
	
	StartPoint = Start;
	EndPoint = End;
	bArePointsSet = true;
}