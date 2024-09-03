// Fill out your copyright notice in the Description page of Project Settings.


#include "Transporter.h"

#include "CollectableKey.h"
#include "PressurePlate.h"

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

	if (bIsOwnerTriggerActor)
	{
		TriggerActors.Add(GetOwner());
	}
	for (AActor* TriggerActor : TriggerActors)
	{
		if (APressurePlate* PressurePlate = Cast<APressurePlate>(TriggerActor))
		{
			PressurePlate->OnActivated.AddDynamic(this, &UTransporter::OnTriggerActivated);
			PressurePlate->OnDeactivated.AddDynamic(this, &UTransporter::OnTriggerDeactivated);
			continue;
		}

		if (ACollectableKey* CollectableKey = Cast<ACollectableKey>(TriggerActor))
		{
			CollectableKey->OnCollected.AddDynamic(this, &UTransporter::OnTriggerActivated);
		}
	}
}


// Called every frame
void UTransporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	bAreAllTriggerActorsActivated = ActivatedTriggerCount == TriggerActors.Num();

	AActor* MyOwner = GetOwner();
	if (MyOwner->HasAuthority() && bArePointsSet)
	{
		FVector CurrentLocation = MyOwner->GetActorLocation();
		float speed = FVector::Distance(StartPoint, EndPoint) / MoveTime;
		FVector TargetLocation = bAreAllTriggerActorsActivated ? EndPoint : StartPoint;
		if (!CurrentLocation.Equals(TargetLocation))
		{
			FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, speed);
			MyOwner->SetActorLocation(NewLocation);
		}
	}
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

void UTransporter::OnTriggerActivated()
{
	ActivatedTriggerCount++;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
	                                 FString::Printf(TEXT("ActivatedTriggerCount: %d"), ActivatedTriggerCount));
}

void UTransporter::OnTriggerDeactivated()
{
	ActivatedTriggerCount--;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
	                                 FString::Printf(TEXT("ActivatedTriggerCount: %d"), ActivatedTriggerCount));
}
