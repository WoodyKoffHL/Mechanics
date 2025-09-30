// Copyright June Rhodes. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/OnlineReplStructs.h"
#include "Interfaces/OnlinePresenceInterface.h"

#include "OnlineUserPresenceData.generated.h"

UENUM(BlueprintType, Meta = (ScriptName = "EOnlinePresenceState"))
enum class FOnlineUserPresenceStatusState : uint8
{
    Online,
    Offline,
    Away,
    ExtendedAway,
    DoNotDisturb,
    Chat,
};

UENUM(BlueprintType, Meta = (ScriptName = "EOnlineCachedResult"))
enum class EOnlineCachedResult_ : uint8
{
    Success,
    NotFound
};

USTRUCT(BlueprintType, Meta = (ScriptName = "FOnlineUserPresenceStatus"))
struct MECHANICS_API FOnlineUserPresenceStatusData
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MyOnlineSubsystem")
    FString Status;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MyOnlineSubsystem")
    FOnlineUserPresenceStatusState State;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MyOnlineSubsystem")
    TMap<FString, FString> Properties;

    static FOnlineUserPresenceStatusData FromNative(const FOnlineUserPresenceStatus &InObj);
    FOnlineUserPresenceStatus ToNative() const;
};

USTRUCT(BlueprintType, Meta = (ScriptName = "FOnlineUserPresence"))
struct MECHANICS_API FOnlineUserPresenceData
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MyOnlineSubsystem")
    FUniqueNetIdRepl SessionId;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MyOnlineSubsystem")
    bool IsOnline;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MyOnlineSubsystem")
    bool IsPlaying;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MyOnlineSubsystem")
    bool IsPlayingThisGame;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MyOnlineSubsystem")
    bool IsJoinable;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MyOnlineSubsystem")
    bool HasVoiceSupport;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MyOnlineSubsystem")
    FDateTime LastOnline;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MyOnlineSubsystem")
    FOnlineUserPresenceStatusData Status;

    static FOnlineUserPresenceData FromNative(TSharedPtr<FOnlineUserPresence> InObj);
    static FOnlineUserPresenceData FromNative(const FOnlineUserPresence &InObj);
    FOnlineUserPresence ToNative() const;
};