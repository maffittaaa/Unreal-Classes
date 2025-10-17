#include "MyDoor.h"
#include "Net/UnrealNetwork.h"

int32 AMyDoor::numberOfDoorsActivated = 0;

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
	UpdateInitialDoorVisuals();
	UpdateFinalDoorVisuals();
}

void AMyDoor::ActivateDoor() {
	bIsActivated = true;
	numberOfDoorsActivated++;
	UE_LOG(LogTemp, Warning, TEXT("Door is activated!!"));
	OnRep_IsActivated();
}

void AMyDoor::DeactivateDoor() {
	bIsActivated = false;
	numberOfDoorsActivated--;
	UE_LOG(LogTemp, Warning, TEXT("Door is deactivated!!"));
	OnRep_IsActivated();
}

void AMyDoor::UpdateInitialDoorVisuals() {
	if (RootComponent) {
		FVector CurrentLocation = RootComponent->GetComponentLocation();
		float TargetZ = bIsActivated ? 120.0f: 0.0f;
		RootComponent->SetRelativeLocation(FVector(CurrentLocation.X, CurrentLocation.Y, TargetZ));
		UE_LOG(LogTemp, Warning, TEXT("Z:  %f"), TargetZ);
	}
}

void AMyDoor::UpdateFinalDoorVisuals() {
	FVector CurrentLocation = RootComponent->GetComponentLocation();
	float TargetZ;
	if (numberOfDoorsActivated >= 4)
		TargetZ = 240.0f;
	else if (bIsActivated)
		TargetZ = 120.0f;
	else
		TargetZ = 0.0f;
	RootComponent->SetRelativeLocation(FVector(CurrentLocation.X, CurrentLocation.Y, TargetZ));
	UE_LOG(LogTemp, Warning, TEXT("Z: %f, Total Active: %d"), TargetZ, numberOfDoorsActivated);
}

void AMyDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyDoor, bIsActivated);
}

