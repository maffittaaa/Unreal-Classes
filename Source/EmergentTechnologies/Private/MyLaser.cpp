#include "MyLaser.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "EmergentTechnologiesCharacter.h"
#include "Components/SphereComponent.h"

AMyLaser::AMyLaser() {
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bAlwaysRelevant = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	collisionSphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	collisionSphere->SetupAttachment(RootComponent);
	collisionSphere->SetSphereRadius(100.0f);
	collisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	laserMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickUpMesh"));
	laserMesh->SetupAttachment(RootComponent);
	laserMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	niagaraLaser = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NSLaser"));
	niagaraLaser->SetupAttachment(RootComponent);

	niagaraLaserImpact = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NSLaserImpact"));
	niagaraLaserImpact->SetupAttachment(RootComponent);
	
}

void AMyLaser::BeginPlay() {
	Super::BeginPlay();
	SetLaserColors();
}

void AMyLaser::SetLaserColors() {
	if (niagaraLaser)
		niagaraLaser->SetVariableLinearColor(FName("User.BeamColor"), FLinearColor(0.703f, 0.245f, 0.0f, 1.0f));

	if (niagaraLaserImpact)
		niagaraLaserImpact->SetVariableLinearColor(FName("User.Colour"), FLinearColor(0.703f, 0.245f, 0.0f, 1.0f));
}

void AMyLaser::TouchingLaser(AEmergentTechnologiesCharacter* projectCharacter) {
	projectCharacter->TakeDamageFromObject_Implementation(2.0f, this);
}

void AMyLaser::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
	float distance = 2100.0f;
	FVector startTrace = GetActorLocation();
	FVector endTrace = startTrace + (GetActorForwardVector() * distance);
	ECollisionChannel traceChannel = ECC_WorldStatic;

	// DrawDebugLine(GetWorld(), startTrace, endTrace, FColor::Red, false, -1.0f, 0, 1.0f);
	
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), false, this);
	RV_TraceParams.bTraceComplex = false;
	RV_TraceParams.bReturnPhysicalMaterial = false;
	RV_TraceParams.AddIgnoredActor(this);
	
	FHitResult RV_Hit;
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		RV_Hit,
		startTrace,
		endTrace,
		traceChannel,
		RV_TraceParams
	);

	FVector beamEnd = bHit ? RV_Hit.Location : endTrace; //select in blueprint
	niagaraLaser->SetVariableVec3(FName("User.BeamEnd"), beamEnd);
	
	if (bHit) {

		if (niagaraLaserImpact)
		{
			niagaraLaserImpact->SetWorldLocation(RV_Hit.Location);
			niagaraLaserImpact->SetActive(true);
		}
		
		if (AEmergentTechnologiesCharacter* projectCharacter = Cast<AEmergentTechnologiesCharacter>(RV_Hit.GetActor())) {
			if (HasAuthority())
				TouchingLaser(projectCharacter);
		}
	} else
	{
		if (niagaraLaserImpact)
			niagaraLaserImpact->SetActive(false);
	}
}

