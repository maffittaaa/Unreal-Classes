// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "MyLavaFloor.generated.h"

UCLASS()
class EMERGENTTECHNOLOGIES_API AMyLavaFloor : public AActor
{
	GENERATED_BODY()
	
	public:	
		// Sets default values for this actor's properties
		UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

		UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* overlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

		AMyLavaFloor();

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* lavaFloorMesh;

	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
		

	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
		UBoxComponent* collisionBox;
	
	public:	
		// Called every frame
		virtual void Tick(float DeltaTime) override;

};
