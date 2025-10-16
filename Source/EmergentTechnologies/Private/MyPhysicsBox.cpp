// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPhysicsBox.h"

// Sets default values
AMyPhysicsBox::AMyPhysicsBox()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	SetReplicateMovement(true);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshComponent->SetupAttachment(RootComponent);

	// Configure mesh component
	meshComponent->SetIsReplicated(true);
	
	// Enable collision
	meshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	meshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	
	// Physics will be enabled in BeginPlay only on server
	meshComponent->SetSimulatePhysics(false);
	
	meshComponent->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AMyPhysicsBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPhysicsBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

