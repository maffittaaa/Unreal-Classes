// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMatchmakingLevelScript.h"
#include "Blueprint/UserWidget.h"
#include "MyMatchmakingSubsystem.h"
#include "MyServerButton.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

void AMyMatchmakingLevelScript::BeginPlay() {
	Super::BeginPlay();
	
	// Enable tick for refresh timer
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	if (UGameInstance* GI = GetGameInstance()) {
		MatchSubsystem = GI->GetSubsystem<UMatchmakingSubsystem>();
		if (MatchSubsystem) {
			MatchSubsystem->OnSessionsUpdated.AddDynamic(this, &AMyMatchmakingLevelScript::OnSessionsUpdated);
			MatchSubsystem->OnHostRequested.AddDynamic(this, &AMyMatchmakingLevelScript::OnHostRequested);
			MatchSubsystem->OnConnectionStatusChanged.AddDynamic(this, &AMyMatchmakingLevelScript::OnConnectionStatusChanged);
		}
	}
	// Create widget and connect buttons like before, but button callbacks call into subsystem
	if (MatchmakingWidgetClass) {
		MatchmakingWidget = CreateWidget<UUserWidget>(GetWorld(), MatchmakingWidgetClass);
		MatchmakingWidget->AddToViewport();
		if (UButton* ConnectButton = Cast<UButton>(MatchmakingWidget->GetWidgetFromName(TEXT("ConnectButton")))) {
			ConnectButton->OnClicked.AddDynamic(this, &AMyMatchmakingLevelScript::OnConnectClicked);
		}
		if (UButton* HostButton = Cast<UButton>(MatchmakingWidget->GetWidgetFromName(TEXT("HostButton")))) {
			HostButton->OnClicked.AddDynamic(this, &AMyMatchmakingLevelScript::OnHostClicked);
			HostButton->SetIsEnabled(false);
		}
		ServerListScrollBoxWidget = Cast<UScrollBox>(MatchmakingWidget->GetWidgetFromName(TEXT("MyScrollBox")));
	}
}

void AMyMatchmakingLevelScript::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	// Clear the timer
	if (RefreshTimerHandle.IsValid()) {
		GetWorldTimerManager().ClearTimer(RefreshTimerHandle);
	}

	if (MatchSubsystem) {
		MatchSubsystem->OnSessionsUpdated.RemoveDynamic(this, &AMyMatchmakingLevelScript::OnSessionsUpdated);
		MatchSubsystem->OnHostRequested.RemoveDynamic(this, &AMyMatchmakingLevelScript::OnHostRequested);
		MatchSubsystem->OnConnectionStatusChanged.RemoveDynamic(this, &AMyMatchmakingLevelScript::OnConnectionStatusChanged);
	}
	Super::EndPlay(EndPlayReason);
}

void AMyMatchmakingLevelScript::RefreshSessionList() {
	if (MatchSubsystem) {
		UE_LOG(LogTemp, Log, TEXT("AMatchmakingLevelScript::RefreshSessionList - Timer triggered"));
		MatchSubsystem->RefreshSessionList();
	}
}

void AMyMatchmakingLevelScript::OnConnectClicked() {
	if (MatchSubsystem) {
		MatchSubsystem->ConnectToMatchmakingServer();
		if (UButton* HostButton = Cast<UButton>(MatchmakingWidget->GetWidgetFromName(TEXT("HostButton")))) {
			HostButton->SetIsEnabled(true);
		}
	}
}

void AMyMatchmakingLevelScript::OnHostClicked() {
	if (MatchSubsystem) {
		// Example: instruct subsystem to host on port 7777 with name
		MatchSubsystem->HostNewGame(TEXT("My test server"), 7777);
	}
}

void AMyMatchmakingLevelScript::OnHostRequested(int32 Port) {
	// Called when server accepted our host request
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController()) {
		FString Cmd = FString::Printf(TEXT("open Lvl_ThirdPerson?listen"));
		PC->ConsoleCommand(*Cmd);
	}
}
void AMyMatchmakingLevelScript::OnConnectionStatusChanged(bool bIsConnected) {
	UE_LOG(LogTemp, Warning, TEXT("AMatchmakingLevelScript::OnConnectionStatusChanged - Connected: %s"),
	bIsConnected ? TEXT("YES") : TEXT("NO"));

	if (!bIsConnected) {
		// Start the refresh timer now that we're connected
		if (!RefreshTimerHandle.IsValid()) {
			UE_LOG(LogTemp, Log, TEXT("Starting session refresh timer with interval: %.2f seconds"), RefreshInterval);
			GetWorldTimerManager().SetTimer(
			RefreshTimerHandle,
			this,
			&AMyMatchmakingLevelScript::RefreshSessionList,
			RefreshInterval,
			true // Loop
			);
		}
	}
	else {
		// Stop the refresh timer if we're disconnected
		if (RefreshTimerHandle.IsValid()) {
			UE_LOG(LogTemp, Warning, TEXT("Stopping session refresh timer - disconnected"));
			GetWorldTimerManager().ClearTimer(RefreshTimerHandle);
			RefreshTimerHandle.Invalidate();
		}
	}
}

void AMyMatchmakingLevelScript::OnSessionsUpdated(const TArray<FMatchSessionInfo>& Sessions) {
	// Rebuild UI using Sessions array (safely on game thread)
	RebuildServerListUI();
}

void AMyMatchmakingLevelScript::RebuildServerListUI() {
	if (!ServerListScrollBoxWidget || !MatchSubsystem) return;
	// Clear existing
	TArray<UWidget*> children = ServerListScrollBoxWidget->GetAllChildren();
	for (UWidget* W : children) {
		W->RemoveFromParent();
	}
	// Fill with current sessions
	const TArray<FMatchSessionInfo>& Sessions = MatchSubsystem->GetSessions();
	for (const FMatchSessionInfo& SI : Sessions) {
		UVerticalBox* Box = NewObject<UVerticalBox>(this);
		ServerListScrollBoxWidget->AddChild(Box);
		// Create a simple button widget for each entry (you probably have a UMyButton class)
		UMyServerButton* ItemBtn = NewObject<UMyServerButton>(this);
		ItemBtn->SetSessionInfo(SI);
		UTextBlock* Txt = NewObject<UTextBlock>(this);
		Txt->SetText(FText::FromString(SI.Name));
		ItemBtn->AddChild(Txt);
		Box->AddChildToVerticalBox(ItemBtn);
	}
}