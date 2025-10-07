#include "EnemyCharacter.h"

#include "EmergentTechnologiesCharacter.h"
#include "MyGameStateBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create AI Perception Component
	aiPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	//Create and configure Sight Sense for AI perception
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig")); //create the sight sense config object
	sightConfig->SightRadius = 1500.0f; // Maximum distance at which the AI can see targets
	sightConfig->LoseSightRadius = 1800.0f; // Distance at which the AI loses sight of targets (hysteresis)
	sightConfig->PeripheralVisionAngleDegrees = 90.0f; // Field of view angle (degrees) for peripheral vision
	sightConfig->SetMaxAge(5); // How long a stimulus is valid after being sensed (in seconds)
	sightConfig->AutoSuccessRangeFromLastSeenLocation = 520.0f; // Range within which AI will automatically succeed in snsing if target was recently seen

	//Configure which affiliations the AI can detect
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	//Add sight configuration to perception component
	aiPerceptionComponent->ConfigureSense(*sightConfig);
	aiPerceptionComponent->SetDominantSense(sightConfig->GetSenseImplementation());

	this->GetMesh()->GlobalAnimRateScale = 2.0f;
	this->GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	//Bind perception update event
	aiPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyCharacter::OnTargetPerceptionUpdated);
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
	if (result.IsSuccess() && HasAuthority()) {
		if (target) {
			AEmergentTechnologiesCharacter* character = Cast<AEmergentTechnologiesCharacter>(target);
			AMyGameStateBase* myGameState = Cast<AMyGameStateBase>(GetWorld()->GetGameState());

			if (character && myGameState)
				myGameState->MulticastOnLevelComplete(character, false);
			target = nullptr;
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
			SetEnemySpeed(150.0f, 3.0f);
			myAIController->MoveToActor(detectedPawn);

			UE_LOG(LogTemp, Display, TEXT("Enemy dectected player: %s"), *Actor->GetName());
		}
	}
	else {
		if (Actor == target) {
			SetEnemySpeed(200.0f, 2.0f);
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


