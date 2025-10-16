// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPhysicsBox.generated.h"

UCLASS()
class EMERGENTTECHNOLOGIES_API AMyPhysicsBox : public AActor
{
	GENERATED_BODY()

	public:	
		AMyPhysicsBox();
		
		// UFUNCTION(BlueprintCallable, Category = "Physics Actor")
		// void SetMesh(UStaticMesh* newMesh);
		//
		// /** Apply an impulse to the mesh (only works on server) */
		// UFUNCTION(BlueprintCallable, Category = "Physics Actor")
		// void ApplyImpulse(FVector impulse);
		//
		// /** Apply force to the mesh (only works on server) */
		// UFUNCTION(BlueprintCallable, Category = "Physics Actor")
		// void ApplyForce(FVector force);

	protected:
		virtual void BeginPlay() override;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* meshComponent;

	public:	
		virtual void Tick(float DeltaTime) override;
};
