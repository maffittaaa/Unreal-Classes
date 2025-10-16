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

	bIsActivated = false;
}

void AMyDoor::BeginPlay() {
	Super::BeginPlay();
}

void AMyDoor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AMyDoor::OnRep_IsActivated() {
	UpdateDoorVisuals();
}

void AMyDoor::ActivateDoor() {
	bIsActivated = true;
	UE_LOG(LogTemp, Warning, TEXT("Door is activated!!"));
	OnRep_IsActivated();
}

void AMyDoor::DeactivateDoor() {
	bIsActivated = false;
	UE_LOG(LogTemp, Warning, TEXT("Door is activated!!"));
	OnRep_IsActivated();
}

void AMyDoor::UpdateDoorVisuals() {
	if (RootComponent) {
		FVector CurrentLocation = RootComponent->GetComponentLocation();
		float TargetZ = bIsActivated ? 240.0f: 0.0f;
		RootComponent->SetRelativeLocation(FVector(CurrentLocation.X, CurrentLocation.Y, TargetZ));
		UE_LOG(LogTemp, Warning, TEXT("Z:  %f"), TargetZ);
	}
}

void AMyDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(AMyDoor, bIsActivated);
}

