// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPressurePlate.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "EmergentTechnologiesCharacter.h"

// Sets default values
AMyPressurePlate::AMyPressurePlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bAlwaysRelevant = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	plateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlateMesh"));
	plateMesh->SetupAttachment(RootComponent);
	plateMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	triggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	triggerBox->SetupAttachment(RootComponent);
	triggerBox->SetBoxExtent(FVector(100.0f, 100.0f, 50.0f));
	triggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	triggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	triggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	triggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMyPressurePlate::OnOverlapBegin);
	triggerBox->OnComponentEndOverlap.AddDynamic(this, &AMyPressurePlate::OnOverlapEnd);
	
	bIsActivated = false;
	playersOnPlate = 0;
	dynamicMaterialInstance = nullptr;

	inactiveColor = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
	activeColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);
}

// Called when the game starts or when spawned
void AMyPressurePlate::BeginPlay()
{
	Super::BeginPlay();

	if (plateMesh && plateMesh->GetNumMaterials() > 0)
		dynamicMaterialInstance = plateMesh->CreateDynamicMaterialInstance(0);
	else
		UE_LOG(LogTemp, Warning, TEXT("PressurePlate: No mesh or materials found!"));

	UpdatePlateVisuals();
}

// Called every frame
void AMyPressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyPressurePlate::OnRep_IsActivated() {
	UpdatePlateVisuals();

	//All visual feedback should go here so it's synchronized across all clients
}

void AMyPressurePlate::OnOverlapBegin(UPrimitiveComponent* overlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (!HasAuthority())
		return;

	if (AEmergentTechnologiesCharacter* projectCharacter = Cast<AEmergentTechnologiesCharacter>(OtherActor)) {
		playersOnPlate++;
		UE_LOG(LogTemp, Warning, TEXT("Player stepped on a pressure plate.Players on plate: %d"), playersOnPlate);
	}

	if (playersOnPlate > 0 && !bIsActivated) {
		bIsActivated = true;
		UE_LOG(LogTemp, Warning, TEXT("Pressure plate ACTIVATED!"));
		UpdatePlateVisuals();
	}
}

void AMyPressurePlate::OnOverlapEnd(UPrimitiveComponent* overlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (!HasAuthority())
		return;

	if (AEmergentTechnologiesCharacter* projectCharacter = Cast<AEmergentTechnologiesCharacter>(OtherActor)) {
		playersOnPlate--;
		if (playersOnPlate < 0)
			playersOnPlate = 0;
		UE_LOG(LogTemp, Warning, TEXT("Player left the pressure plate.Players on plate: %d"), playersOnPlate);
	}

	if (playersOnPlate == 0 && bIsActivated) {
		bIsActivated = false;
		UE_LOG(LogTemp, Warning, TEXT("Pressure plate DEACTIVATED!"));
		UpdatePlateVisuals();
	}
}

void AMyPressurePlate::UpdatePlateVisuals()
{

	if (dynamicMaterialInstance){
		FLinearColor CurrentColor = bIsActivated ? activeColor : inactiveColor;
		dynamicMaterialInstance->SetVectorParameterValue(FName("Color"), CurrentColor);
	}
	
	if (plateMesh) {
		FVector CurrentLocation = plateMesh->GetRelativeLocation();
		float TargetZ = bIsActivated ? -100.0f : 0.0f; // Press down when activated
		plateMesh->SetRelativeLocation(FVector(CurrentLocation.X, CurrentLocation.Y, TargetZ));
	}
}

void AMyPressurePlate::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Register bIsActivated for replication with RepNotify
	DOREPLIFETIME(AMyPressurePlate, bIsActivated);
}




