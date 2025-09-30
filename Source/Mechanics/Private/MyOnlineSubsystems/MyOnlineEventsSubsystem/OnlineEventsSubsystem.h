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
#include "Interfaces/OnlineEventsInterface.h"

#include "OnlineEventsSubsystem.generated.h"


UCLASS(BlueprintType)
class MECHANICS_API UOnlineEventsSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

    friend class UOnlineHelpers;

private:
    struct FWorldContext* WorldContext;
    TWeakPtr<class IOnlineEvents, ESPMode::ThreadSafe> HandleWk;
    TSharedPtr<class IOnlineEvents, ESPMode::ThreadSafe> GetHandle();
    bool IsHandleValid(const TSharedPtr<class IOnlineEvents, ESPMode::ThreadSafe>& InHandle);

public:
    TSet<FString> __InFlightRequests;

    UOnlineEventsSubsystem();
    virtual void BeginDestroy() override;

    UFUNCTION(BlueprintPure, Category = "MyOnlineSubsystem|Events")
    bool IsSubsystemAvailable();
   
};
