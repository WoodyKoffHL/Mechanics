// Copyright June Rhodes. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/OnlineReplStructs.h"
#include "Online/CoreOnline.h"
#if defined(UE_4_25_OR_LATER)
#include "VoiceChat.h"
#include "VoiceChatResult.h"
#endif
#include "Interfaces/OnlineVoiceAdminInterface.h"

#include "OSBVoiceChatTypes.generated.h"

UENUM(BlueprintType, Meta = (ScriptName = "EVoiceChatChannelType"))
enum class EVoiceChatChannelType_ : uint8
{
    NonPositional,
    Positional,
    Echo
};

UENUM(BlueprintType, Meta = (ScriptName = "EVoiceChatTransmitMode"))
enum class EVoiceChatTransmitMode_ : uint8
{
    None,
    All,
    Channel
};

UENUM(BlueprintType, Meta = (ScriptName = "EVoiceChatAttenuationModel"))
enum class EVoiceChatAttenuationModel_ : uint8
{
    None,
    InverseByDistance,
    LinearByDistance,
    ExponentialByDistance
};

UENUM(BlueprintType, Meta = (ScriptName = "EVoiceChatResult"))
enum class EVoiceChatResult_ : uint8
{
    Success = 0,

    InvalidState,
    NotInitialized,
    NotConnected,
    NotLoggedIn,
    NotPermitted,
    Throttled,

    InvalidArgument,
    CredentialsInvalid,
    CredentialsExpired,

    ClientTimeout,
    ServerTimeout,
    DnsFailure,
    ConnectionFailure,

    ImplementationError
};

USTRUCT(BlueprintType)
struct MECHANICS_API FVoiceChatResultBP
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|VoiceChat")
    bool Successful;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|VoiceChat")
    EVoiceChatResult_ ResultCode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|VoiceChat")
    FString ErrorCode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|VoiceChat")
    int ErrorNum;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|VoiceChat")
    FString ErrorDesc;

#if defined(UE_4_25_OR_LATER)
    static FVoiceChatResultBP FromNative(const FVoiceChatResult &InObj);
    FVoiceChatResult ToNative() const;
#endif
};

USTRUCT(BlueprintType)
struct MECHANICS_API FVoiceChatDeviceInfoBP
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|VoiceChat")
    FString DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|VoiceChat")
    FString Id;

#if defined(UE_4_26_OR_LATER)
    static FVoiceChatDeviceInfoBP FromNative(const FVoiceChatDeviceInfo &InObj);
    FVoiceChatDeviceInfo ToNative() const;
#endif
};

USTRUCT(BlueprintType)
struct MECHANICS_API FVoiceChatChannel3dPropertiesBP
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|VoiceChat")
    bool bSet;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|VoiceChat")
    EVoiceChatAttenuationModel_ AttenuationModel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|VoiceChat")
    float MinDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|VoiceChat")
    float MaxDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|VoiceChat")
    float Rolloff;

#if defined(UE_4_25_OR_LATER)
    static FVoiceChatChannel3dPropertiesBP FromNative(const TOptional<FVoiceChatChannel3dProperties> &InObj);
    TOptional<FVoiceChatChannel3dProperties> ToNative() const;
#endif
};

USTRUCT(BlueprintType)
struct MECHANICS_API FVoiceAdminChannelCredentialsBP
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|VoiceAdmin")
    FUniqueNetIdRepl TargetUserId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|VoiceAdmin")
    FString PlayerName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|VoiceAdmin")
    FString ChannelCredentials;

    static FVoiceAdminChannelCredentialsBP FromNative(const FVoiceAdminChannelCredentials &InObj);
    FVoiceAdminChannelCredentials ToNative() const;
};
