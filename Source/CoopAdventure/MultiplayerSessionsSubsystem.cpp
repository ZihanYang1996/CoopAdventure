// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Online/OnlineSessionNames.h"

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
	bCreateServerAfterDestroy = false;

	ServerNameToFInd = "";
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
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(
				this, &UMultiplayerSessionsSubsystem::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(
				this, &UMultiplayerSessionsSubsystem::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(
				this, &UMultiplayerSessionsSubsystem::OnFindSessionsComplete);
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

	// Check if the session with the same name already exists
	FNamedOnlineSession* ExisitingSession = SessionInterface->GetNamedSession(FName(*ServerName));

	if (ExisitingSession)
	{
		printString("Session with the same name already exists! Destroying it...");
		bCreateServerAfterDestroy = true;
		SessionInterface->DestroySession(FName(*ServerName));
		return;
	}

	if (ServerName.IsEmpty())
	{
		printString("Server name is empty!");
		return;
	}

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true; // Allow players to join a game in progress
	SessionSettings.bIsDedicated = false; // It's listen server (not dedicated)
	SessionSettings.bShouldAdvertise = true;
	// Advertise this session, meaning it will be shown in the public online services
	SessionSettings.NumPublicConnections = 2; // It's a 2 player game
	SessionSettings.bUseLobbiesIfAvailable = true; // Use lobbies if available
	SessionSettings.bUsesPresence = true; // Use the online presence features
	SessionSettings.bAllowJoinViaPresence = true; // Allow players to join via presence
	SessionSettings.bIsLANMatch = OnlineSubsystem->GetSubsystemName() == "NULL";

	// Save the ServerName in the settings
	SessionSettings.Set(FName("SEVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// If we are using the NULL subsystem, it's a LAN match

	SessionInterface->CreateSession(0, FName(*ServerName), SessionSettings);
}

void UMultiplayerSessionsSubsystem::FindServer(FString ServerName)
{
	printString("Finding Server: " + ServerName);

	if (ServerName.IsEmpty())
	{
		printString("Server name is empty!");
		return;
	}

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = OnlineSubsystem->GetSubsystemName() == "NULL";
	// If we are using the NULL subsystem, it's a LAN match
	SessionSearch->MaxSearchResults = 90; // Maximum number of search results
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	// Search for sessions with presence because we set bUsesPresence to true in CreateSession

	ServerNameToFInd = ServerName;
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef()); // Convert TSharedPtr to TSharedRef
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

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		printString(
			FString::Printf(TEXT("Exising session (%s) destroyed! Creating a new one..."), *SessionName.ToString()));
	}
	else
	{
		printString("Session destruction failed!");
	}

	if (bCreateServerAfterDestroy)
	{
		CreateServer(SessionName.ToString());
		bCreateServerAfterDestroy = false;
	}
}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TArray<FOnlineSessionSearchResult> Results = SessionSearch->SearchResults;
		if (Results.Num() > 0)
		{
			printString(FString::Printf(TEXT("%d sessions found!"), Results.Num()));
			for (const FOnlineSessionSearchResult& Result : Results)
			{
				if (Result.IsValid())
				{
					FString ServerName = "Null";
					Result.Session.SessionSettings.Get(FName("SEVER_NAME"), ServerName);

					printString(FString::Printf(TEXT("Server name is: %s"), *ServerName));
				}
			}
			return;
		}
	}

	printString("Find sessions failed!");
}
