// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPhysicsBox.h"

#include "MyPressurePlate.h"

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

	// Only simulate physics on the server (authority)
	if (HasAuthority()) {
		meshComponent->SetSimulatePhysics(true);
		UE_LOG(LogTemp, Log, TEXT("PhysicsReplicatedActor %s: Physics enabled on SERVER"), *GetName());
	}
	else {
		// On clients, disable physics simulation - just receive replicated transforms
		meshComponent->SetSimulatePhysics(false);
		UE_LOG(LogTemp, Log, TEXT("PhysicsReplicatedActor %s: Physics DISABLED on CLIENT (receiving replication)"), *GetName());
	}
}

// Called every frame
void AMyPhysicsBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

