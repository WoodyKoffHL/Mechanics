// Copyright June Rhodes. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineUserPresenceData.h"
#include "OnlineUserRef.h"
#include "Online/CoreOnline.h"

#include "FriendTypes.generated.h"

UENUM(BlueprintType, Meta = (ScriptName = "EInviteStatus"))
enum class EInviteStatus_ : uint8
{
    Unknown,
    Accepted,
    PendingInbound,
    PendingOutbound,
    Blocked,
    Suggested
};

UCLASS(BlueprintType, Blueprintable, Transient, Meta = (DontUseGenericSpawnObject))
class MECHANICS_API UOnlineFriendRef : public UOnlineUserRef
{
    GENERATED_BODY()

private:
    TSharedPtr<FOnlineFriend> Friend;

public:
    static UOnlineFriendRef *FromFriend(const TSharedPtr<FOnlineFriend> &Friend);
    TSharedPtr<FOnlineFriend> GetFriend() const
    {
        return this->Friend;
    }

    UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "MyOnlineSubsystem")
    EInviteStatus_ GetInviteStatus();

    UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "MyOnlineSubsystem")
    FOnlineUserPresenceData GetPresence();

protected:
    virtual EInviteStatus_ GetInviteStatus_Native();
    virtual FOnlineUserPresenceData GetPresence_Native();
};

USTRUCT(BlueprintType)
struct MECHANICS_API FReportPlayedWithUserInfo
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FUniqueNetIdRepl UserId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FString PresenceStr;

    static FReportPlayedWithUserInfo FromNative(const FReportPlayedWithUser &InObj);
    FReportPlayedWithUser ToNative() const;
};

USTRUCT(BlueprintType)
struct MECHANICS_API FFriendSettingsData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    TMap<FString, FString> Data;

    static FFriendSettingsData FromNative(const FFriendSettings &InObj);
    FFriendSettings ToNative() const;
};

USTRUCT(BlueprintType)
struct MECHANICS_API FOnlineFriendSettingsSourceDataConfig
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool NeverShowAgain;

    static FOnlineFriendSettingsSourceDataConfig FromNative(const TSharedRef<FOnlineFriendSettingsSourceData> &InObj);
    FOnlineFriendSettingsSourceData ToNative() const;
};