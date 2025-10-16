#include "ACPreassuredDoor.h"

UACPreassuredDoor::UACPreassuredDoor() {
	PrimaryComponentTick.bCanEverTick = true;

	door = CreateDefaultSubobject<AMyDoor>(TEXT("Door"));

	pressurePlate = CreateDefaultSubobject<AMyPressurePlate>(TEXT("PressurePlate"));
}

void UACPreassuredDoor::BeginPlay() {
	Super::BeginPlay();
	OpenDoorWithPressurePlate();
}

void UACPreassuredDoor::OpenDoorWithPressurePlate() {
	// if (pressurePlate->bIsActivated) {
		UE_LOG(LogTemp, Warning, TEXT("Door is activated!!"));
		door->bDoorActivated = true;
	
		FVector CurrentLocation = door->doorMesh->GetRelativeLocation();
		float TargetZ = door->bDoorActivated ? 708.0f : 408.0f;
		door->doorMesh->SetRelativeLocation(FVector(CurrentLocation.X, CurrentLocation.Y, TargetZ));
		UE_LOG(LogTemp, Warning, TEXT("Z:  %f"), TargetZ);
	// }
}

void UACPreassuredDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

