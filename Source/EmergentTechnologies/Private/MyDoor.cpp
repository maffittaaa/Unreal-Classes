#include "MyDoor.h"
#include "Net/UnrealNetwork.h"

AMyDoor::AMyDoor() {
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bAlwaysRelevant = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	doorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	doorMesh->SetupAttachment(RootComponent);
	doorMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	bDoorActivated = false;
}

void AMyDoor::BeginPlay() {
	Super::BeginPlay();
	
}

void AMyDoor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AMyDoor::OnRep_IsActivated() {
	
}

void AMyDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyDoor, bDoorActivated);

}

