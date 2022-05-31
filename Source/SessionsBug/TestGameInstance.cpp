#include "TestGameInstance.h"

#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

const FString UTestGameInstance::options = "?listen?bIsLanMatch=1";
const FName UTestGameInstance::serverNameKey = "ServerName";

void UTestGameInstance::onCreateSession(FName sessionName, bool wasSuccessful)
{
	if (wasSuccessful)
	{
		session->StartSession(sessionName);
	}
}

void UTestGameInstance::onStartSession(FName sessionName, bool wasSuccessful)
{
	if (wasSuccessful)
	{
		APlayerController* controller = GetFirstLocalPlayerController();
		FString levelName;

		controller->SetInputMode(FInputModeGameOnly());

		controller->SetShowMouseCursor(false);

		sessionSettings->Get(SETTING_MAPNAME, levelName);

		GetWorld()->ServerTravel(levelName + options, true);
	}
}

void UTestGameInstance::onFindSessions(bool wasSuccessful, TArray<FBlueprintSessionResult>* sessionsData)
{
	session->OnFindSessionsCompleteDelegates.Clear();

	if (wasSuccessful)
	{
		if (searchSession.IsValid())
		{
			sessionsData->Empty();

			for (const auto& i : searchSession->SearchResults)
			{
				sessionsData->Add({ i });
			}
		}
	}
}

void UTestGameInstance::Init()
{
	subsystem = IOnlineSubsystem::Get();

	if (subsystem)
	{
		session = subsystem->GetSessionInterface();

		if (session.IsValid())
		{
			session->OnCreateSessionCompleteDelegates.AddUObject(this, &UTestGameInstance::onCreateSession);

			session->OnStartSessionCompleteDelegates.AddUObject(this, &UTestGameInstance::onStartSession);

			sessionSettings = MakeShareable(new FOnlineSessionSettings());

			sessionSettings->bIsLANMatch = true;
			sessionSettings->bUsesPresence = true;
			sessionSettings->bShouldAdvertise = true;
			sessionSettings->NumPublicConnections = 4;
			sessionSettings->NumPrivateConnections = 0;
			sessionSettings->bAllowJoinInProgress = true;
			sessionSettings->bAllowJoinViaPresence = true;
		}
	}
}

void UTestGameInstance::initSearchSession()
{
	searchSession = MakeShareable(new FOnlineSessionSearch());

	searchSession->bIsLanQuery = true;
	searchSession->MaxSearchResults = 5;
	searchSession->PingBucketSize = 100;

	searchSession->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Type::Equals);
}

void UTestGameInstance::hostSession(TSharedPtr<const FUniqueNetId> userId, FName sessionName, const TSoftObjectPtr<UWorld>& level)
{
	sessionSettings->Set(UTestGameInstance::serverNameKey, sessionName.ToString(), EOnlineDataAdvertisementType::Type::ViaOnlineService);

	sessionSettings->Set(SETTING_MAPNAME, level.GetAssetName(), EOnlineDataAdvertisementType::Type::ViaOnlineService);

	session->CreateSession(*userId, sessionName, *sessionSettings);
}

void UTestGameInstance::findLocalSessions(TSharedPtr<const FUniqueNetId> userId, TArray<FBlueprintSessionResult>& sessionsData)
{
	session->OnFindSessionsCompleteDelegates.AddUObject(this, &UTestGameInstance::onFindSessions, &sessionsData);

	this->initSearchSession();

	session->FindSessions(*userId, searchSession.ToSharedRef());
}

void UTestGameInstance::createSession(FName sessionName, TSoftObjectPtr<UWorld> level)
{
	this->hostSession(GetFirstGamePlayer()->GetPreferredUniqueNetId().GetUniqueNetId(), sessionName, level);
}

void UTestGameInstance::findSessions(TArray<FBlueprintSessionResult>& sessionsData)
{
	this->findLocalSessions(GetFirstGamePlayer()->GetPreferredUniqueNetId().GetUniqueNetId(), sessionsData);
}

void UTestGameInstance::loadNextLevel(TSoftObjectPtr<UWorld> nextLevel)
{
	GetWorld()->ServerTravel(nextLevel.GetAssetName() + options);
}
