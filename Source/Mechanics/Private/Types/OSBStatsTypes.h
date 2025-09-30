// Copyright June Rhodes. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/OnlineReplStructs.h"
#include "Interfaces/OnlineStatsInterface.h"
#include "OSBSessionTypes.h"
#include "Online/CoreOnline.h"

#include "OSBStatsTypes.generated.h"

UENUM(BlueprintType, Meta = (ScriptName = "EOnlineStatModificationType"))
enum class EOnlineStatModificationType_ : uint8
{
    Unknown,
    Sum,
    Set,
    Largest,
    Smallest
};

USTRUCT(BlueprintType)
struct MECHANICS_API FOnlineStatsUserStatsBP
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|Stats")
    FUniqueNetIdRepl PlayerId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|Stats")
    TMap<FString, FVariantDataBP> Stats;

    static FOnlineStatsUserStatsBP FromNative(const FOnlineStatsUserStats &InObj);
    static FOnlineStatsUserStatsBP FromNative(const TSharedRef<const FOnlineStatsUserStats> InObj);
    static FOnlineStatsUserStatsBP FromNative(const TSharedPtr<const FOnlineStatsUserStats> InObj);
    FOnlineStatsUserStats ToNative() const;
};

USTRUCT(BlueprintType)
struct MECHANICS_API FOnlineStatUpdateBP
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|Stats")
    EOnlineStatModificationType_ Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|Stats")
    FVariantDataBP Value;
};

USTRUCT(BlueprintType)
struct MECHANICS_API FOnlineStatsUserUpdatedStatsBP
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|Stats")
    FUniqueNetIdRepl PlayerId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|Stats")
    TMap<FString, FOnlineStatUpdateBP> Stats;

    static FOnlineStatsUserUpdatedStatsBP FromNative(const FOnlineStatsUserUpdatedStats &InObj);
    FOnlineStatsUserUpdatedStats ToNative() const;
};