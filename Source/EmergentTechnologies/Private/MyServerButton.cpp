#include "MyServerButton.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

UMyServerButton::UMyServerButton() {
	OnClicked.AddDynamic(this, &UMyServerButton::OnClick);
}

void UMyServerButton::SetSessionInfo(const FMatchSessionInfo& InInfo) {
	SessionInfo = InInfo;
}

void UMyServerButton::OnClick() {
	if (GetOuter() && GetOuter()->GetWorld()) {
		if (APlayerController* playerController = GetOuter()->GetWorld()->GetFirstPlayerController()) {
			UE_LOG(LogTemp, Warning, TEXT("Connecting to %s:%d"), *SessionInfo.ServerIp, SessionInfo.ServerPort);
			FString cmd = FString::Printf(TEXT("open %s:%d"), *SessionInfo.ServerIp, SessionInfo.ServerPort);
			playerController->ConsoleCommand(*cmd);
		}
	}
}



