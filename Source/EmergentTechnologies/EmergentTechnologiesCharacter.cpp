// Copyright Epic Games, Inc. All Rights Reserved.

#include "EmergentTechnologiesCharacter.h"

#include <filesystem>
#include <iostream>

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "EmergentTechnologies.h"
#include "EmergentTechnologiesGameMode.h"
#include "MyPlayerState.h"
#include "MyHealthComponent.h"
#include "Blueprint/UserWidget.h"
#include "EmergentTechnologies/Public/UShooterComponent.h"

using namespace std;

AEmergentTechnologiesCharacter::AEmergentTechnologiesCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 400.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	healthComponent = CreateDefaultSubobject<UMyHealthComponent>(TEXT("HealthComponent"));

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//Create shooter component
	ShooterComponent = CreateDefaultSubobject<AUShooterComponent>(TEXT("Shooter Component"));

	
	

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AEmergentTechnologiesCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEmergentTechnologiesCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AEmergentTechnologiesCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEmergentTechnologiesCharacter::Look);
		
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AEmergentTechnologiesCharacter::Duck);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AEmergentTechnologiesCharacter::StopDuck);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AEmergentTechnologiesCharacter::Interact);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AEmergentTechnologiesCharacter::StopInteract);
	}
	else
		UE_LOG(LogEmergentTechnologies, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
}

void AEmergentTechnologiesCharacter::BeginPlay() {
	Super::BeginPlay();

	healthComponent->OnDeath.AddDynamic(this, &AEmergentTechnologiesCharacter::RespawnPlayer);

	UE_LOG(LogTemp, Warning, TEXT("WidgetClass: %p"), *widgetClass);
	if (widgetClass)
		HUDWidget = CreateWidget<UUserWidget>(GetWorld(), widgetClass, FName("HUD"));
}

void AEmergentTechnologiesCharacter::AddAndRemoveWidget() {
	float distance = 150.0f;
	FVector startTrace = GetActorLocation();
	FVector endTrace = startTrace + (GetActorForwardVector() * distance);

	ECollisionChannel traceChannel = ECC_Visibility;
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), false, this);
	RV_TraceParams.bTraceComplex = false;
	RV_TraceParams.bReturnPhysicalMaterial = false;
	RV_TraceParams.AddIgnoredActor(this);

	// DrawDebugLine(GetWorld(), startTrace, endTrace, FColor::Red, false, -1.0f, 0, 1.0f);
	
	FHitResult RV_Hit;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		RV_Hit,
		startTrace,
		endTrace,
		traceChannel,
		RV_TraceParams
	);

	UPrimitiveComponent* hitComponent = RV_Hit.GetComponent();
	if (bHit && hitComponent->ComponentHasTag(FName("CanInteract")) && !HUDWidget->IsInViewport())
		HUDWidget->AddToViewport();
	else if (!bHit || !hitComponent->ComponentHasTag(FName("CanInteract")))
		HUDWidget->RemoveFromParent();
}


void AEmergentTechnologiesCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AEmergentTechnologiesCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AEmergentTechnologiesCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AEmergentTechnologiesCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AEmergentTechnologiesCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AEmergentTechnologiesCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AEmergentTechnologiesCharacter::Duck() {
	// signal the character to crouch
	ACharacter::Crouch(false);	
}

void AEmergentTechnologiesCharacter::StopDuck() {
	// signal the character to stop crouching
	ACharacter::UnCrouch(false);
}

void AEmergentTechnologiesCharacter::Interact() {
	UE_LOG(LogTemp, Display, TEXT("Interacting"));
}


void AEmergentTechnologiesCharacter::StopInteract() {
	UE_LOG(LogTemp, Display, TEXT("Stopped Interacting"));
}

void AEmergentTechnologiesCharacter::CollectCoin() {
	if (AMyPlayerState* myPlayerState = Cast<AMyPlayerState>(GetPlayerState()))
		myPlayerState->AddCoin();
}

void AEmergentTechnologiesCharacter:: TakeDamageFromObject_Implementation(float damage, AActor* damageCauser) {
	UE_LOG(LogTemp, Warning, TEXT("Current health: %f"), healthComponent->GetCurrentHealth());
	if (healthComponent->GetCurrentHealth() >= 2.0f)
		healthComponent->TakeDamageFromObject(damage);
}

void AEmergentTechnologiesCharacter::RespawnPlayer() {
	if (AEmergentTechnologiesGameMode* myGameModeBase = Cast<AEmergentTechnologiesGameMode>(GetWorld()->GetAuthGameMode())) {
		if (AController* playerController = this->GetController()) {
			myGameModeBase->RespawnPlayer(playerController);
			UE_LOG(LogTemp, Warning, TEXT("Triggered respawn for player %s"), *this->GetName());
		}
	}	
}

void AEmergentTechnologiesCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (widgetClass)
		AddAndRemoveWidget();
}
