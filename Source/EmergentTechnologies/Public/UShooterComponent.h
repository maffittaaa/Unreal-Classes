// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UShooterComponent.generated.h"

UCLASS()
class EMERGENTTECHNOLOGIES_API AUShooterComponent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUShooterComponent();
	UPROPERTY(EditAnywhere) TSubclassOf<AActor> ShooterBP;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
