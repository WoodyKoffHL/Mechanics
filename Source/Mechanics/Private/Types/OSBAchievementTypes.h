// Copyright June Rhodes. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/OnlineReplStructs.h"
#include "Interfaces/OnlineAchievementsInterface.h"
#include "OSBSessionTypes.h"
#include "Online/CoreOnline.h"

#include "OSBAchievementTypes.generated.h"

USTRUCT(BlueprintType)
struct MECHANICS_API FOnlineAchievementBP
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem|Achievements")
    FString Id;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem|Achievements")
    float Progress;

    static FOnlineAchievementBP FromNative(FOnlineAchievement Achievement);
};

USTRUCT(BlueprintType)
struct MECHANICS_API FOnlineAchievementDescBP
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem|Achievements")
    FText Title;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem|Achievements")
    FText LockedDesc;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem|Achievements")
    FText UnlockedDesc;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem|Achievements")
    bool bIsHidden;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem|Achievements")
    FDateTime UnlockTime;

    static FOnlineAchievementDescBP FromNative(FOnlineAchievementDesc AchievementDescription);
};