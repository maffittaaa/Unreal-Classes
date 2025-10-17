// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyGameStateBase.h"
#include "MyDoor.generated.h"

UCLASS()
class EMERGENTTECHNOLOGIES_API AMyDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyDoor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* doorMesh;

	UPROPERTY(ReplicatedUsing=OnRep_IsActivated, VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle")
	bool bIsActivated;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	int id;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameState")
	AMyGameStateBase* myGameState;

	UFUNCTION(BlueprintPure, Category = "Puzzle")
	bool IsActivated() const {return bIsActivated;}

	void ActivateDoor();
	void DeactivateDoor();

	void UpdateInitialDoorVisuals();
	void UpdateFinalDoorVisuals();

	TArray<AActor*> doors;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_IsActivated();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
