// Copyright June Rhodes. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IMessageSanitizerInterface.h"
#include "Online/CoreOnline.h"

#include "BlockedQueryResultInfo.generated.h"

USTRUCT(BlueprintType)
struct MECHANICS_API FBlockedQueryResultInfo
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool Blocked;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool BlockedNonFriends;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FString UserId;

    static FBlockedQueryResultInfo FromNative(const FBlockedQueryResult &InObj);
    FBlockedQueryResult ToNative();
};