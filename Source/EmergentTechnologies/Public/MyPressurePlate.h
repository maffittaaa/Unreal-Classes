// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPressurePlate.generated.h"

UCLASS()
class EMERGENTTECHNOLOGIES_API AMyPressurePlate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyPressurePlate();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* plateMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* triggerBox;

	UPROPERTY(ReplicatedUsing=OnRep_IsActivated, VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle")
	bool bIsActivated;

	UFUNCTION(BlueprintPure, Category = "Puzzle")
	bool IsActivated() const {return bIsActivated;}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_IsActivated();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* overlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* overlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void UpdatePlateVisuals();

	UPROPERTY(EditAnywhere, Category = "Visual")
	FLinearColor inactiveColor;

	UPROPERTY(EditAnywhere, Category = "Visual")
	FLinearColor activeColor;

	UPROPERTY()
	class UMaterialInstanceDynamic* dynamicMaterialInstance;

	int32 playersOnPlate;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
