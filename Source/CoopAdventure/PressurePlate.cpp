// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlate.h"

// Sets default values
APressurePlate::APressurePlate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Enable replication for this actor
	SetReplicates(true);
	SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));
	TriggerMesh->SetupAttachment(RootComponent);
	TriggerMesh->SetIsReplicated(true);

	auto TriggerMeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(
		TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder"));
	if (TriggerMeshAsset.Succeeded())
	{
		TriggerMesh->SetStaticMesh(TriggerMeshAsset.Object);
		TriggerMesh->SetRelativeScale3D(FVector(3.3f, 3.3f, 0.2f));
		TriggerMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
	}

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetIsReplicated(true);

	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(
		TEXT("/Game/Stylized_Egypt/Meshes/building/SM_building_part_08.SM_building_part_08"));
	if (MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
		Mesh->SetRelativeScale3D(FVector(4.0f, 4.0f, 0.5f));
		Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 7.2f));
	}
}

// Called when the game starts or when spawned
void APressurePlate::BeginPlay()
{
	Super::BeginPlay();

	// Make the trigger mesh invisible when the game starts
	TriggerMesh->SetVisibility(false);
	// Make the trigger mesh overlap with other actors
	TriggerMesh->SetCollisionProfileName(FName("OverlapAll"));
}

// Called every frame
void APressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
