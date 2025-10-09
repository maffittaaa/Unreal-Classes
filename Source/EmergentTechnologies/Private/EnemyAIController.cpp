#include "EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "EnemyCharacter.h"

AEnemyAIController::AEnemyAIController()
{
	aiPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	sightConfig->SightRadius = 1500.0f;
	sightConfig->LoseSightRadius = 1800.0f;
	sightConfig->PeripheralVisionAngleDegrees = 90.0f;
	sightConfig->SetMaxAge(5.0f);
	sightConfig->AutoSuccessRangeFromLastSeenLocation = 520.0f;
	
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	
	aiPerceptionComponent->ConfigureSense(*sightConfig);
	aiPerceptionComponent->SetDominantSense(sightConfig->GetSenseImplementation());

	aiPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdated);
	SetPerceptionComponent(*aiPerceptionComponent);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (aiPerceptionComponent)
	{
		aiPerceptionComponent->Activate();
		UE_LOG(LogTemp, Warning, TEXT("AIController %s: AI Perception activated for pawn %s"),
		*GetName(), *InPawn->GetName());
	}
}
void AEnemyAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// Forward the perception event to the enemy character
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(GetPawn());
	if (Enemy)
		Enemy->OnTargetPerceptionUpdated(Actor, Stimulus);
}
