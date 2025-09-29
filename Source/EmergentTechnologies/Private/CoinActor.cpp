#include "CoinActor.h"
#include "EmergentTechnologiesCharacter.h"
#include "MyGameStateBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ACoinActor::ACoinActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Script/Engine.StaticMesh'/Game/LevelPrototyping/Interactable/JumpPad/Assets/Meshes/SM_CircularBand.SM_CircularBand'"));
	
	collisionSphere = CreateDefaultSubobject<USphereComponent>("Sphere"); //all available in the blueprint
	collisionSphere->SetupAttachment(RootComponent); //always accessible in the actor class
	collisionSphere->SetSphereRadius(100.0f); //default value
	collisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision); //type of collision - queryOnly - only run this collision when you run the events
	collisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACoinActor::OnOverlapBegin);

	coinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickUpMesh"));
	coinMesh->SetupAttachment(RootComponent);
	coinMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	coinMesh->SetStaticMesh(MeshObj.Object);
}

// Called when the game starts or when spawned
void ACoinActor::BeginPlay()
{
	Super::BeginPlay();
}

void ACoinActor::PlayEffects_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, collectEffects, GetActorLocation());
}

// Called every frame
void ACoinActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACoinActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACharacter* character = Cast<ACharacter>(OtherActor))
	{
		//UE_LOG(LogTemp, Warning, TEXT("ACoinActor::OnOverlapBegin"));
		if (AEmergentTechnologiesCharacter* projectCharacter = Cast<AEmergentTechnologiesCharacter>(character))
		{
			if (HasAuthority())
			{
				projectCharacter->CollectCoin();
				PlayEffects();
				Destroy();
				AMyGameStateBase* myGameStateBase = Cast<AMyGameStateBase>(GetWorld()->GetGameState());
				if (myGameStateBase != nullptr)
					myGameStateBase->UpdateTotalCoinsInLevel();
			}
		}
	}
}

