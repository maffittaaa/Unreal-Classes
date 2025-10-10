#include "MyLavaFloor.h"

#include "EmergentTechnologiesCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

AMyLavaFloor::AMyLavaFloor() {
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bAlwaysRelevant = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	// collisionBox = CreateDefaultSubobject<UBoxComponent>("Box");
	// collisionBox->SetupAttachment(RootComponent);
	// FVector NewExtent(100.0f, 200.0f, 1.0f);
	// collisionBox->SetBoxExtent(NewExtent, true);
	// collisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// collisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	// collisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	//
	// collisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMyLavaFloor::OnOverlapBegin);
	// collisionBox->OnComponentEndOverlap.AddDynamic(this, &AMyLavaFloor::OnOverlapEnd);
	
	lavaFloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TouchMesh"));
	lavaFloorMesh->SetupAttachment(RootComponent);
	lavaFloorMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	lavaFloorMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	lavaFloorMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	lavaFloorMesh->OnComponentBeginOverlap.AddDynamic(this, &AMyLavaFloor::OnOverlapBegin);
	lavaFloorMesh->OnComponentEndOverlap.AddDynamic(this, &AMyLavaFloor::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AMyLavaFloor::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AMyLavaFloor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AMyLavaFloor::BurnInLava(AEmergentTechnologiesCharacter* projectCharacter) {
	projectCharacter->TakeDamageFromObject_Implementation(2.0f, this);
}


void AMyLavaFloor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	int burnDelay = 1.0f;
	if (AEmergentTechnologiesCharacter* projectCharacter = Cast<AEmergentTechnologiesCharacter>(OtherActor)) {
		if (HasAuthority()) {
			
			BurnInLava(projectCharacter);

			burnDelegate.BindUFunction(this, FName("BurnInLava"), projectCharacter);
			
			GetWorldTimerManager().SetTimer(
				burnTimer,
				burnDelegate,
				burnDelay,
				true
			);
		}
	}
}

void AMyLavaFloor::OnOverlapEnd(UPrimitiveComponent* overlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (!HasAuthority())
		return;

	if (AEmergentTechnologiesCharacter* projectCharacter = Cast<AEmergentTechnologiesCharacter>(OtherActor)) {
		GetWorldTimerManager().ClearTimer(burnTimer);
		UE_LOG(LogTemp, Warning, TEXT("No more damage taken!"));
	}
}

// void AMyLavaFloor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
// {
// 	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
// 	// Register bIsActivated for replication with RepNotify
// 	DOREPLIFETIME(AMyLavaFloor);
// }

