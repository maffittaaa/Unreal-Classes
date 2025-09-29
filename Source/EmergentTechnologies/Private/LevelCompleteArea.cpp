#include "LevelCompleteArea.h"
#include "EmergentTechnologiesCharacter.h"
#include "MyGameStateBase.h"
#include "Components/BoxComponent.h"

// Sets default values
ALevelCompleteArea::ALevelCompleteArea() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	boxComponent->SetBoxExtent(FVector(200.0f, 200.0f, 200.0f));
	boxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	boxComponent->SetCollisionResponseToAllChannels(ECR_Ignore); //
	boxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	boxComponent->SetupAttachment(GetRootComponent());
	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALevelCompleteArea::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ALevelCompleteArea::BeginPlay() {
	Super::BeginPlay();
}

void ALevelCompleteArea::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (HasAuthority()) {
		AEmergentTechnologiesCharacter* projectCharacter = Cast<AEmergentTechnologiesCharacter>(OtherActor);
		if (projectCharacter != nullptr) {
			AMyGameStateBase *myGamestate = Cast<AMyGameStateBase>(GetWorld()->GetGameState());
			if (myGamestate != nullptr) {
				if (myGamestate->totalLevelCoins == 0) {
					UE_LOG(LogTemp, Display, TEXT("Level Complete"));
					myGamestate->MulticastOnLevelComplete(projectCharacter, true);
				}
			}
		}
	}
}

// Called every frame
void ALevelCompleteArea::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

