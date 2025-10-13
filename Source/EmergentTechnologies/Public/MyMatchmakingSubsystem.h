#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MyMatchSessionInfo.h"
#include "MyTCPClientRunnable.h"
#include "MyMatchmakingSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionsUpdated, const TArray<FMatchSessionInfo>&, Sessions);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHostRequested, int32, Port);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnectionStatusChanged, bool, bIsConnected);

class FTCPClientRunnable;

UCLASS()
class EMERGENTTECHNOLOGIES_API UMatchmakingSubsystem : public UGameInstanceSubsystem {
	GENERATED_BODY()
	
	public:
		// Subsystem lifecycle
		virtual void Initialize(FSubsystemCollectionBase& collection) override; 
		virtual void Deinitialize() override; //to close the socket connection
		
		// Public API
		UFUNCTION(BlueprintCallable)
		void ConnectToMatchmakingServer();
		
		UFUNCTION(BlueprintCallable)
		void HostNewGame(const FString& name, int32 port);
		
		UFUNCTION(BlueprintCallable)
		void RefreshSessionList();
		
		UFUNCTION(BlueprintCallable)
		bool IsConnected() const;
		
		const TArray<FMatchSessionInfo>& GetSessions() const { return sessions; }
		
		UPROPERTY(BlueprintAssignable)
		FOnSessionsUpdated OnSessionsUpdated;
		
		UPROPERTY(BlueprintAssignable)
		FOnHostRequested OnHostRequested;
		
		UPROPERTY(BlueprintAssignable)
		FOnConnectionStatusChanged OnConnectionStatusChanged;
		
		// Called by the runnable on the game thread
		void HandleServerMessage(const FString& serverMessage);

		// Called by the runnable when connection status changes
		void HandleConnectionStatusChanged(bool bIsConnected);
		
	private:
		// Owned runnable that talks to the matchmaking server
		MyTCPClientRunnable* clientRunnable = nullptr;
		
		// Stored sessions (value types)
		TArray<FMatchSessionInfo> sessions;
		
		// Helpers
		void ParseAndSetSessions(const FString& serverMessage);
};
