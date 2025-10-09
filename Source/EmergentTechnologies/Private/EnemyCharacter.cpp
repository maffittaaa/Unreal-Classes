#include "EnemyCharacter.h"

#include "EmergentTechnologiesCharacter.h"
#include "EmergentTechnologiesGameMode.h"
#include "EnemyAIController.h"
#include "MyGameStateBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	this->GetMesh()->GlobalAnimRateScale = 2.0f;
	this->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay() {
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), waypoints);
	myAIController = Cast<AAIController>(GetController());
	if (waypoints.Num() > 0 && myAIController) {
		myAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &AEnemyCharacter::AIMoveCompleted);
		if (HasAuthority()) {
			myAIController->MoveToActor(GetRandomWaypoint());
		}
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

ATargetPoint* AEnemyCharacter::GetRandomWaypoint() {
	int index = FMath::RandRange(0, waypoints.Num() - 1);
	return Cast<ATargetPoint>(waypoints[index]);
}

void AEnemyCharacter::AIMoveCompleted(FAIRequestID requestID, const FPathFollowingResult& result)
{
	if (!HasAuthority())
		return;
	
	if (result.IsSuccess()) {
		if (target) {
			AEmergentTechnologiesCharacter* character = Cast<AEmergentTechnologiesCharacter>(target);

			if (character) {
				UE_LOG(LogTemp, Warning, TEXT("Enemy %s caught the player %s!"), *GetName(), *character->GetName());

				if (AEmergentTechnologiesGameMode* myGameModeBase = Cast<AEmergentTechnologiesGameMode>(GetWorld()->GetAuthGameMode())) {
					if (AController* playerController = character->GetController()) {
						myGameModeBase->RespawnPlayer(playerController);
						UE_LOG(LogTemp, Warning, TEXT("Triggered respawn for player %s"), *character->GetName());
					}
				}
			}
			
			target = nullptr;
			SetEnemySpeed(200.0f, 1.0f);
		}
		
		if (waypoints.Num() > 0 && myAIController) {
			myAIController->MoveToActor(GetRandomWaypoint());
		}
	}
}

void AEnemyCharacter::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus stimulus) {
	if (!HasAuthority()) return;

	if (stimulus.WasSuccessfullySensed()) {
		APawn* detectedPawn = Cast<APawn>(Actor);
		if (detectedPawn && myAIController && !target) {
			target = detectedPawn;
			SetEnemySpeed(250.0f, 2.5f);
			myAIController->MoveToActor(detectedPawn);

			UE_LOG(LogTemp, Display, TEXT("Enemy dectected player: %s"), *Actor->GetName());
		}
	}
	else {
		if (Actor == target) {
			SetEnemySpeed(200.0f, 2.5f);
			target = nullptr;
			myAIController->MoveToActor(GetRandomWaypoint());

			UE_LOG(LogTemp, Log, TEXT("Enemy target out of range, returning to patrol"));
		}
	}
}

void AEnemyCharacter::SetEnemySpeed(float newSpeed, float newAnimRate)
{
	if (HasAuthority()) {
		currentSpeed = newSpeed;
		animationRate = newAnimRate;

		//update locally on server
		OnRep_CurrentSpeed();
		OnRep_AnimationRate();
	}
}

void AEnemyCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEnemyCharacter, currentSpeed);
	DOREPLIFETIME(AEnemyCharacter, animationRate);
}

void AEnemyCharacter::OnRep_CurrentSpeed() {
	if (GetCharacterMovement()) {
		GetCharacterMovement()->MaxWalkSpeed = currentSpeed;
		UE_LOG(LogTemp, Display, TEXT("Enemy speed updated to: %f"), currentSpeed);
	}
}

void AEnemyCharacter::OnRep_AnimationRate() {
	if (GetMesh()) {
		GetMesh()->GlobalAnimRateScale = animationRate;
		UE_LOG(LogTemp, Display, TEXT("Enemy animation rate updated to: %f"), animationRate);
	}
}


