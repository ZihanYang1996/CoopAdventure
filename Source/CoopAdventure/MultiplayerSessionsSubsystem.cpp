// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

void printString(const FString& str)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, str);
	}
}

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
{
	// printString("MSS Constructor");
}

void UMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	// printString("MSS Initialize");
	OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		FString SubsystemName = OnlineSubsystem->GetSubsystemName().ToString();
		printString("OnlineSubsystem found: " + SubsystemName);

		// Get session interface
		SessionInterface = OnlineSubsystem->GetSessionInterface();

		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnCreateSessionComplete);
		}
		else
		{
			printString("Session interface is NOT valid!");
		}
	}
	else
	{
		printString("OnlineSubsystem not found");
	}
}

void UMultiplayerSessionsSubsystem::Deinitialize()
{
	UE_LOG(LogTemp, Warning, TEXT("MSS Deinitialize"));
}

void UMultiplayerSessionsSubsystem::CreateServer(FString ServerName)
{
	printString("Create Server: " + ServerName);

	if (ServerName.IsEmpty())
	{
		printString("Server name is empty!");
		return;
	}

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;  // Allow players to join a game in progress
	SessionSettings.bIsDedicated = false;  // It's listen server (not dedicated)
	SessionSettings.bShouldAdvertise = true;  // Advertise this session, meaning it will be shown in the public online services
	SessionSettings.NumPublicConnections = 2; // It's a 2 player game
	SessionSettings.bUseLobbiesIfAvailable = true;  // Use lobbies if available
	SessionSettings.bUsesPresence = true;  // Use the online presence features
	SessionSettings.bAllowJoinViaPresence = true;  // Allow players to join via presence
	SessionSettings.bIsLANMatch = OnlineSubsystem->GetSubsystemName() == "NULL";  // If we are using the NULL subsystem, it's a LAN match
	
	SessionInterface->CreateSession(0, FName(*ServerName), SessionSettings);
}

void UMultiplayerSessionsSubsystem::FindServer(FString ServerName)
{
	printString("Find Server: " + ServerName);
}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
	}
	else
	{
		printString("Session creation failed!");
	}
}