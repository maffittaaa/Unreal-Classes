// Fill out your copyright notice in the Description page of Project Settings.


#include "MyLaser.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "EmergentTechnologiesCharacter.h"
#include "Components/SphereComponent.h"

AMyLaser::AMyLaser() {
	PrimaryActorTick.bCanEverTick = true;

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
	// niagaraLaser->SetColorParameter(FName("Colour"), FLinearColor(0.703f, 0.245f, 0.0f, 1.0f));

	niagaraLaserImpact = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NSLaserImpact"));
	niagaraLaserImpact->SetupAttachment(RootComponent);
	
	// collisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AMyLaser::OnOverlapBegin);
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

// void AMyLaser::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
// 	if (AEmergentTechnologiesCharacter* projectCharacter = Cast<AEmergentTechnologiesCharacter>(OtherActor)) {
// 		if (HasAuthority()) {
// 			projectCharacter->CollectCoin();
// 		}
// 	}
// }

void AMyLaser::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
	UE_LOG(LogTemp, Warning, TEXT("AMyLaser::Tick - Executing laser logic"));
	
	float distance = 1100.0f;
	FVector startTrace = GetActorLocation();
	FVector endTrace = startTrace + (GetActorForwardVector() * distance);
	ECollisionChannel traceChannel = ECC_Visibility;
	
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), false, this);
	RV_TraceParams.bTraceComplex = false;
	RV_TraceParams.bReturnPhysicalMaterial = false;
	RV_TraceParams.AddIgnoredActor(this);
	
	
	FHitResult RV_Hit(ForceInit);
	
	GetWorld()->LineTraceSingleByChannel(
		RV_Hit,
		startTrace,
		endTrace,
		traceChannel,
		RV_TraceParams
	);

	FVector param = RV_Hit.bBlockingHit ? RV_Hit.Location : endTrace; //select in blueprint
	niagaraLaser->SetVariableVec3(FName("User.BeamEnd"), param);
	
	if (RV_Hit.bBlockingHit) {
		niagaraLaserImpact->SetWorldLocation(RV_Hit.Location);
		niagaraLaserImpact->SetActive(true);
	} else
		niagaraLaserImpact->SetActive(false);
}

