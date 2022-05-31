#pragma once

#include "Core.h"

#include "Engine/GameInstance.h"
#include "Online.h"
#include "Net/UnrealNetwork.h"
#include "FindSessionsCallbackProxy.h"

#include "TestGameInstance.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnDestroySessionCompleteCallback, FName, sessionName, bool, wasSuccessful);

UCLASS()
class SESSIONSBUG_API UTestGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	void onCreateSession(FName sesisonName, bool wasSuccessful);

	void onStartSession(FName sessionName, bool wasSuccessful);

	void onFindSessions(bool wasSuccessful, TArray<FBlueprintSessionResult>* sessionsData);

private:
	static const FString options;

public:
	static const FName serverNameKey;

private:
	IOnlineSubsystem* subsystem;
	IOnlineSessionPtr session;
	TSharedPtr<FOnlineSessionSettings> sessionSettings;
	TSharedPtr<FOnlineSessionSearch> searchSession;

private:
	FOnDestroySessionCompleteDelegate onDestroyDelegate;

private:
	void Init() override;

	void initSearchSession();

public:
	UTestGameInstance() = default;

	void hostSession(TSharedPtr<const FUniqueNetId> userId, FName sessionName, const TSoftObjectPtr<UWorld>& level);

	void findLocalSessions(TSharedPtr<const FUniqueNetId> userId, TArray<FBlueprintSessionResult>& sessionsData);

	UFUNCTION(Category = Sessions, BlueprintCallable)
	void createSession(FName sessionName, TSoftObjectPtr<UWorld> level);

	UFUNCTION(Category = Sessions, BlueprintCallable)
	void findSessions(UPARAM(ref) TArray<FBlueprintSessionResult>& sessionsData);

	UFUNCTION(Category = LevelLoading, BlueprintCallable)
	void loadNextLevel(TSoftObjectPtr<UWorld> nextLevel);
};
