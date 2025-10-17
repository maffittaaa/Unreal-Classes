#include "DoorsMinigame.h"

#include "EmergentTechnologiesCharacter.h"
#include "MyGameStateBase.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"

ADoorsMinigame::ADoorsMinigame() {
	PrimaryActorTick.bCanEverTick = true;

	boxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	boxComponent->SetBoxExtent(FVector(200.0f, 200.0f, 200.0f));
	boxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	boxComponent->SetCollisionResponseToAllChannels(ECR_Ignore); //
	boxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	boxComponent->SetupAttachment(GetRootComponent());
	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADoorsMinigame::OnOverlapBegin);
}

void ADoorsMinigame::BeginPlay() {
	Super::BeginPlay();
}

void ADoorsMinigame::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) {
		AEmergentTechnologiesCharacter* projectCharacter = Cast<AEmergentTechnologiesCharacter>(OtherActor);
		if (projectCharacter != nullptr) {
			AMyGameStateBase *myGameState = Cast<AMyGameStateBase>(GetWorld()->GetGameState());
			if (myGameState != nullptr) {
				myGameState->AddDoorsActivatedWidget();
			}
		}
	}
	
}


void ADoorsMinigame::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

