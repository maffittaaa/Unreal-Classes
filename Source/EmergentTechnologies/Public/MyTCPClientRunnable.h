// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "Templates/SharedPointer.h"

class UMatchmakingSubsystem;
// Minimal TCP client runnable. Uses UE sockets. Keeps owner as a weak pointer and
// always marshals messages back to game thread using AsyncTask.
class EMERGENTTECHNOLOGIES_API MyTCPClientRunnable : public FRunnable {
	public:
		MyTCPClientRunnable(UMatchmakingSubsystem* InOwner);
		virtual ~MyTCPClientRunnable();
	
		// FRunnable interface
		virtual bool Init() override;
		virtual uint32 Run() override;
		virtual void Stop() override;
	
		// Control API
		void HostNewGame(const FString& Name, int32 Port);
		void RequestSessionList();
		bool IsConnected() const { return bConnected; }
	
	private:
		FRunnableThread* Thread;
		FSocket* Socket;
		FThreadSafeBool bRun;
		FThreadSafeBool bConnected;
		TWeakObjectPtr<UMatchmakingSubsystem> OwnerSubsystem;
		FCriticalSection SendMutex;
	
		void SendRawString(const FString& Message);
};
