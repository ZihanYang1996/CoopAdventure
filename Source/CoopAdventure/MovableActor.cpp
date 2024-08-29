// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableActor.h"

#include "Transporter.h"
#include "Components/ArrowComponent.h"

// Sets default values
AMovableActor::AMovableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);
	SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetIsReplicated(true);  // It seems okay even without this line

	Point1 = CreateDefaultSubobject<UArrowComponent>(TEXT("StartLocation"));
	Point1->SetupAttachment(RootComponent);
	Point1->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	Point2 = CreateDefaultSubobject<UArrowComponent>(TEXT("EndLocation"));
	Point2->SetupAttachment(RootComponent);
	Point2->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

	Transporter = CreateDefaultSubobject<UTransporter>("TransporterComponent");

}

// Called when the game starts or when spawned
void AMovableActor::BeginPlay()
{
	Super::BeginPlay();

	Transporter->SetPoints(Point1->GetComponentLocation(), Point2->GetComponentLocation());
}

// Called every frame
void AMovableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

