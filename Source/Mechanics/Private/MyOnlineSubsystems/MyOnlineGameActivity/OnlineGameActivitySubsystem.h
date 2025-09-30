// Copyright June Rhodes. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Delegates/Delegate.h"
#include "Delegates/DelegateCombinations.h"
#include "GameFramework/OnlineReplStructs.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Types/OnlineUserRef.h"
#include "Types/FriendTypes.h"
#include "Types/OnlineAccountCredential.h"
#include "Types/OnlineErrorInfo.h"
#include "Types/OnlineRecentPlayerRef.h"
#include "Types/OnlineLoginStatus.h"
#include "Types/BlockedQueryResultInfo.h"
#include "Types/OnlineUserPrivilege.h"
#include "Types/MessageTypes.h"
#include "Types/ExternalUITypes.h"
#include "Types/OSBFileTypes.h"
#include "Types/OSBPartyTypes.h"
#include "Types/OSBLobbyTypes.h"
#include "Types/OSBSessionTypes.h"
#include "Types/OSBLeaderboardTypes.h"
#include "Types/OSBStatsTypes.h"
#include "Types/OSBAchievementTypes.h"
#include "Helpers/ArrayConversion.h"
#include "Helpers/UniqueNetIdConversion.h"
#include "Interfaces/OnlineGameActivityInterface.h"
#include "MyOnlineSubsystems/MyOnlineEventsSubsystem/OnlineEventsSubsystem.h"

#include "OnlineGameActivitySubsystem.generated.h"


// @parameters-not-compatible
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGameActivity_GameActivityActivationRequested_BP, const FUniqueNetId&, LocalUserId, const FString&, ActivityId, const FOnlineSessionSearchResult*, SessionInfo);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGameActivity_OnGameActivityActivationRequested, FUniqueNetIdRepl, LocalUserId, FString, ActivityId, FOnlineSessionSearchResultBP, SessionInfo);

UCLASS(BlueprintType)
class MECHANICS_API UOnlineGameActivitySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

		friend class UOnlineHelpers;

private:
	struct FWorldContext* WorldContext;
	TWeakPtr<class IOnlineGameActivity, ESPMode::ThreadSafe> HandleWk;
	TSharedPtr<class IOnlineGameActivity, ESPMode::ThreadSafe> GetHandle();
	bool IsHandleValid(const TSharedPtr<class IOnlineGameActivity, ESPMode::ThreadSafe>& InHandle);
	//FDelegateHandle DelegateHandle_OnGameActivityActivationRequested;

public:
	TSet<FString> __InFlightRequests;

	UOnlineGameActivitySubsystem();
	virtual void BeginDestroy() override;

	UFUNCTION(BlueprintPure, Category = "Online|GameActivity")
		bool IsSubsystemAvailable();

	//UPROPERTY(BlueprintAssignable, Category = "Online|GameActivity")
	//	FOnGameActivity_OnGameActivityActivationRequested OnGameActivityActivationRequested;

	//void OnGameActivityDelegate(const FUniqueNetId& LocalUserId, const FString& ActivityId, const FOnlineSessionSearchResult* SessionInfo);


	friend class UOnlineGameActivitySubsystemResetAllActiveActivities;
	friend class UOnlineGameActivitySubsystemSetActivityAvailability;
	friend class UOnlineGameActivitySubsystemSetActivityPriority;
	friend class UOnlineGameActivitySubsystemStartActivity;
	friend class UOnlineGameActivitySubsystemEndActivity;
	friend class UOnlineGameActivitySubsystemResumeActivity;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnlineGameActivitySubsystemResetAllActiveActivitiesCallbackPin, FUniqueNetIdRepl, LocalUserId, FOnlineErrorInfo, Status);

UCLASS()
class MECHANICS_API UOnlineGameActivitySubsystemResetAllActiveActivities : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FOnlineGameActivitySubsystemResetAllActiveActivitiesCallbackPin OnCallFailed;
	UPROPERTY(BlueprintAssignable)
		FOnlineGameActivitySubsystemResetAllActiveActivitiesCallbackPin OnResetAllActiveActivitiesComplete;

	UFUNCTION(BlueprintCallable, Category = "Online|GameActivity", meta = (BlueprintInternalUseOnly = "true"))
		static UOnlineGameActivitySubsystemResetAllActiveActivities* ResetAllActiveActivities(
			UOnlineGameActivitySubsystem* Subsystem
			, FUniqueNetIdRepl LocalUserId
		);

	virtual void Activate() override;

private:
	UPROPERTY()
		UOnlineGameActivitySubsystem* __Store__Subsystem;
	UPROPERTY()
		FString __Store__CallUniquenessId;
	void HandleCallback_OnResetAllActiveActivitiesComplete(const FUniqueNetId& LocalUserId, const FOnlineError& Status);

	UPROPERTY()
		FUniqueNetIdRepl __Store__LocalUserId;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnlineGameActivitySubsystemSetActivityAvailabilityCallbackPin, FUniqueNetIdRepl, LocalUserId, FOnlineErrorInfo, Status);

UCLASS()
class MECHANICS_API UOnlineGameActivitySubsystemSetActivityAvailability : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FOnlineGameActivitySubsystemSetActivityAvailabilityCallbackPin OnCallFailed;
	UPROPERTY(BlueprintAssignable)
		FOnlineGameActivitySubsystemSetActivityAvailabilityCallbackPin OnSetActivityAvailabilityComplete;

	UFUNCTION(BlueprintCallable, Category = "Online|GameActivity", meta = (BlueprintInternalUseOnly = "true"))
		static UOnlineGameActivitySubsystemSetActivityAvailability* SetActivityAvailability(
			UOnlineGameActivitySubsystem* Subsystem
			, FUniqueNetIdRepl LocalUserId, FString ActivityId, bool bEnabled
		);

	virtual void Activate() override;

private:
	UPROPERTY()
		UOnlineGameActivitySubsystem* __Store__Subsystem;
	UPROPERTY()
		FString __Store__CallUniquenessId;
	void HandleCallback_OnSetActivityAvailabilityComplete(const FUniqueNetId& LocalUserId, const FOnlineError& Status);

	UPROPERTY()
		FUniqueNetIdRepl __Store__LocalUserId;

	UPROPERTY()
		FString __Store__ActivityId;

	UPROPERTY()
		bool __Store__bEnabled;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnlineGameActivitySubsystemSetActivityPriorityCallbackPin, FUniqueNetIdRepl, LocalUserId, FOnlineErrorInfo, Status);

UCLASS()
class MECHANICS_API UOnlineGameActivitySubsystemSetActivityPriority : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FOnlineGameActivitySubsystemSetActivityPriorityCallbackPin OnCallFailed;
	UPROPERTY(BlueprintAssignable)
		FOnlineGameActivitySubsystemSetActivityPriorityCallbackPin OnSetActivityPriorityComplete;

	UFUNCTION(BlueprintCallable, Category = "Online|GameActivity", meta = (BlueprintInternalUseOnly = "true"))
		static UOnlineGameActivitySubsystemSetActivityPriority* SetActivityPriority(
			UOnlineGameActivitySubsystem* Subsystem
			, FUniqueNetIdRepl LocalUserId, TMap<FString, int32> PrioritizedActivities
		);

	virtual void Activate() override;

private:
	UPROPERTY()
		UOnlineGameActivitySubsystem* __Store__Subsystem;
	UPROPERTY()
		FString __Store__CallUniquenessId;
	void HandleCallback_OnSetActivityPriorityComplete(const FUniqueNetId& LocalUserId, const FOnlineError& Status);

	UPROPERTY()
		FUniqueNetIdRepl __Store__LocalUserId;

	UPROPERTY()
		TMap<FString, int32> __Store__PrioritizedActivities;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnlineGameActivitySubsystemStartActivityCallbackPin, FUniqueNetIdRepl, LocalUserId, FString, ActivityId, FOnlineErrorInfo, Status);

UCLASS()
class MECHANICS_API UOnlineGameActivitySubsystemStartActivity : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FOnlineGameActivitySubsystemStartActivityCallbackPin Failed;
	UPROPERTY(BlueprintAssignable)
		FOnlineGameActivitySubsystemStartActivityCallbackPin Complete;

	UFUNCTION(BlueprintCallable, Category = "Online|GameActivity", meta = (BlueprintInternalUseOnly = "true"))
		static UOnlineGameActivitySubsystemStartActivity* StartActivity(
			UOnlineGameActivitySubsystem* Subsystem
			, FUniqueNetIdRepl LocalUserId, FString ActivityId/*, FBOnlineEventParms Parms*/
		);

	virtual void Activate() override;

private:
	UPROPERTY()
		UOnlineGameActivitySubsystem* __Store__Subsystem;
	UPROPERTY()
		FString __Store__CallUniquenessId;
	void HandleCallback_OnStartActivityComplete(const FUniqueNetId& LocalUserId, const FString& ActivityId, const FOnlineError& Status);

	UPROPERTY()
		FUniqueNetIdRepl __Store__LocalUserId;

	UPROPERTY()
		FString __Store__ActivityId;

	//UPROPERTY()
	//	FBOnlineEventParms __Store__Parms;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnlineGameActivitySubsystemEndActivityCallbackPin, FUniqueNetIdRepl, LocalUserId, FString, ActivityId, EOnlineActivityOutcomeBP, Outcome, FOnlineErrorInfo, Status);

UCLASS()
class MECHANICS_API UOnlineGameActivitySubsystemEndActivity : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FOnlineGameActivitySubsystemEndActivityCallbackPin Failed;
	UPROPERTY(BlueprintAssignable)
		FOnlineGameActivitySubsystemEndActivityCallbackPin Complete;

	UFUNCTION(BlueprintCallable, Category = "Online|GameActivity", meta = (BlueprintInternalUseOnly = "true"))
		static UOnlineGameActivitySubsystemEndActivity* EndActivity(
			UOnlineGameActivitySubsystem* Subsystem
			, FUniqueNetIdRepl LocalUserId, FString ActivityId, EOnlineActivityOutcomeBP ActivityOutcome/*, FBOnlineEventParms Parms*/
		);

	virtual void Activate() override;

private:
	UPROPERTY()
		UOnlineGameActivitySubsystem* __Store__Subsystem;
	UPROPERTY()
		FString __Store__CallUniquenessId;
	void HandleCallback_OnEndActivityComplete(const FUniqueNetId& LocalUserId, const FString& ActivityId, const EOnlineActivityOutcome& ActivityOutcome, const FOnlineError& Status);

	UPROPERTY()
		FUniqueNetIdRepl __Store__LocalUserId;

	UPROPERTY()
		FString __Store__ActivityId;

	UPROPERTY()
		EOnlineActivityOutcomeBP __Store__Outcome;

	//UPROPERTY()
	//	FBOnlineEventParms __Store__Parms;


};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnlineGameActivitySubsystemResumeActivityCallbackPin, FUniqueNetIdRepl, LocalUserId, FString, ActivityId, FOnlineErrorInfo, Status);

UCLASS()
class MECHANICS_API UOnlineGameActivitySubsystemResumeActivity : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FOnlineGameActivitySubsystemResumeActivityCallbackPin Failed;
	UPROPERTY(BlueprintAssignable)
		FOnlineGameActivitySubsystemResumeActivityCallbackPin Complete;

	UFUNCTION(BlueprintCallable, Category = "Online|GameActivity", meta = (BlueprintInternalUseOnly = "true"))
		static UOnlineGameActivitySubsystemResumeActivity* ResumeActivity(
			UOnlineGameActivitySubsystem* Subsystem
			, FUniqueNetIdRepl LocalUserId, FString ActivityId, FOnlineActivityTasksToResetBP TasksToReset
		);

	virtual void Activate() override;

private:
	UPROPERTY()
		UOnlineGameActivitySubsystem* __Store__Subsystem;
	UPROPERTY()
		FString __Store__CallUniquenessId;
	void HandleCallback_OnResumeActivityComplete(const FUniqueNetId& LocalUserId, const FString& ActivityId, const FOnlineError& Status);

	UPROPERTY()
		FUniqueNetIdRepl __Store__LocalUserId;

	UPROPERTY()
		FString __Store__ActivityId;

	UPROPERTY()
		FOnlineActivityTasksToResetBP __Store__TasksToReset;
};

