// Fill out your copyright notice in the Description page of Project Settings.


#include "MyLaser.h"
#include "NiagaraComponent.h"
#include "EmergentTechnologiesCharacter.h"
#include "Components/SphereComponent.h"

AMyLaser::AMyLaser() {
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	collisionSphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	collisionSphere->SetupAttachment(RootComponent);
	collisionSphere->SetSphereRadius(100.0f);
	collisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	collisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AMyLaser::OnOverlapBegin);

	laserMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickUpMesh"));
	laserMesh->SetupAttachment(RootComponent);
	laserMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	niagaraLaser = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraLaser"));
	UNiagaraSystem* niagaraLaserAsset = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/ThirdPerson/Blueprints/Lasers/NSLaser"));
	niagaraLaser->SetAsset(niagaraLaserAsset, true);
	niagaraLaser->SetupAttachment(RootComponent);

	niagaraLaserImpact = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraLaserImpact"));
	UNiagaraSystem* niagaraLaserImpactAsset = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/ThirdPerson/Blueprints/Lasers/NsLaserImpact"));
	niagaraLaserImpact->SetAsset(niagaraLaserImpactAsset, true);
	niagaraLaserImpact->SetupAttachment(RootComponent);
}

void AMyLaser::BeginPlay() {
	Super::BeginPlay();
	SetColorsForNiagaraLaser(niagaraLaser, niagaraLaserImpact, FColor(0.703f, 0.245f, 0.0f, 1.0f));
}

void AMyLaser::SetColorsForNiagaraLaser(UNiagaraComponent* newNiagaraLaser, UNiagaraComponent* newNiagaraLaserImpact, FLinearColor colorToPass) {
	if (newNiagaraLaser)
		newNiagaraLaser->SetColorParameter(FName("Colour"), colorToPass);

	if (newNiagaraLaserImpact)
		newNiagaraLaserImpact->SetColorParameter(FName("Colour"), colorToPass);
}

void AMyLaser::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (AEmergentTechnologiesCharacter* projectCharacter = Cast<AEmergentTechnologiesCharacter>(OtherActor)) {
		if (HasAuthority()) {
			projectCharacter->CollectCoin();
		}
	}
}

void AMyLaser::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

