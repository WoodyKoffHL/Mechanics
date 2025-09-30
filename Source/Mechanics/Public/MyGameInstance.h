// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineGameActivityInterface.h"
#include "Types/OnlineErrorInfo.h"
#include "Types/OSBSessionTypes.h"
#include "OnlineHelpers.h"
#include "Helpers/UniqueNetIdConversion.h"
#include "MyGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGameActivityActivationRequested_Delegate, FUniqueNetIdRepl, LocalUserId, FString, ActivityId, FOnlineSessionSearchResultBP, SessionInfo);

UCLASS()
class MECHANICS_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void FinishDestroy() override;

	UPROPERTY(BlueprintAssignable, Category = "Online|GameActivity")
		FOnGameActivityActivationRequested_Delegate OnGameActivityActivationRequested;

	void OnGameActivityHandle(const FUniqueNetId& LocalUserId, const FString& ActivityId, const FOnlineSessionSearchResult* SessionInfo);

private:
	FDelegateHandle OnGameActivityActivationRequested_DelegateHandle;

	TWeakPtr<class IOnlineGameActivity, ESPMode::ThreadSafe> HandleWk;
	TSharedPtr<class IOnlineGameActivity, ESPMode::ThreadSafe> GetHandle();

};
