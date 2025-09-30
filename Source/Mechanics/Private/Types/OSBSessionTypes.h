// Copyright June Rhodes. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/OnlineReplStructs.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Online/CoreOnline.h"

#include "OSBSessionTypes.generated.h"

UENUM(BlueprintType, Meta = (ScriptName = "EOnJoinSessionCompleteResult"))
enum class EOnJoinSessionCompleteResult_ : uint8
{
    Success,
    SessionIsFull,
    SessionDoesNotExist,
    CouldNotRetrieveAddress,
    AlreadyInSession,
    UnknownError
};

UENUM(BlueprintType, Meta = (ScriptName = "ESessionFailure"))
enum class ESessionFailure_ : uint8
{
    ServiceConnectionLost
};

UENUM(BlueprintType, Meta = (ScriptName = "EOnlineSessionState"))
enum class EOnlineSessionState_ : uint8
{
    NoSession,
    Creating,
    Pending,
    Starting,
    InProgress,
    Ending,
    Ended,
    Destroying
};

UCLASS(BlueprintType, Transient, Meta = (DontUseGenericSpawnObject, IsBlueprintBase = "false"))
class MECHANICS_API UOnlineSessionInfo : public UObject
{
    GENERATED_BODY()

private:
    TSharedPtr<class FOnlineSessionInfo> SessionInfo;

public:
    static UOnlineSessionInfo *FromNative(TSharedPtr<class FOnlineSessionInfo> InSessionInfo);
    TSharedPtr<class FOnlineSessionInfo> ToNative() const
    {
        return this->SessionInfo;
    }
    bool IsValid() const
    {
        return this->SessionInfo.IsValid();
    }
};

UENUM(BlueprintType, Meta = (ScriptName = "EOnlineDataAdvertisementType"))
enum class EOnlineDataAdvertisementType_ : uint8
{
    DontAdvertise,
    ViaPingOnly,
    ViaOnlineService,
    ViaOnlineServiceAndPing
};

UENUM(BlueprintType, Meta = (ScriptName = "EOnlineKeyValuePairDataType"))
enum class EOnlineKeyValuePairDataType_ : uint8
{
    Empty,
    Int32,
    UInt32_NotSupported,
    Int64,
    UInt64_NotSupported,
    Double_NotSupported,
    String,
    Float,
    Blob_NotSupported,
    Bool,
    Json_NotSupported
};

USTRUCT(BlueprintType) struct FVariantDataBP
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    EOnlineKeyValuePairDataType_ Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool AsBool;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    int32 AsInt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    float AsFloat;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    int64 AsInt64;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FString AsString;

    static FVariantDataBP FromNative(const FVariantData &InObj);
    FVariantData ToNative() const;
};

USTRUCT(BlueprintType) struct FOnlineSessionSettingBP
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FVariantDataBP Data;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    EOnlineDataAdvertisementType_ AdvertisementType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    int32 ID;

    static FOnlineSessionSettingBP FromNative(const FOnlineSessionSetting &InObj);
    FOnlineSessionSetting ToNative();
};

USTRUCT(BlueprintType) struct FOnlineSessionSettingsBP
{
    GENERATED_BODY()

private:
    FOnlineSessionSettings RefTemp;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    int32 NumPublicConnections;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    int32 NumPrivateConnections;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool bShouldAdvertise;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool bAllowJoinInProgress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool bIsLANMatch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool bIsDedicated;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool bUsesStats;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool bAllowInvites;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool bUsesPresence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool bAllowJoinViaPresence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool bAllowJoinViaPresenceFriendsOnly;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool bAntiCheatProtected;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    int32 BuildUniqueId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    TMap<FName, FOnlineSessionSettingBP> Settings;

    static FOnlineSessionSettingsBP FromNative(const FOnlineSessionSettings &InObj);
    FOnlineSessionSettings ToNative();
    FOnlineSessionSettings &ToNativeRef();
};

UCLASS(BlueprintType, Transient, Meta = (DontUseGenericSpawnObject, IsBlueprintBase = "false"))
class MECHANICS_API UOnlineSessionSettings : public UObject
{
    GENERATED_BODY()

private:
    FOnlineSessionSettingsBP Value;

public:
    static UOnlineSessionSettings *FromNative(class FOnlineSessionSettings *InSession);

    UFUNCTION(BlueprintPure, Category = "MyOnlineSubsystem")
    FOnlineSessionSettingsBP GetValue()
    {
        return this->Value;
    }
};

USTRUCT(BlueprintType) struct FOnlineSessionBP
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FUniqueNetIdRepl OwningUserId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FString OwningUserName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FOnlineSessionSettingsBP SessionSettings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    UOnlineSessionInfo *SessionInfo;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem")
    int32 NumOpenPrivateConnections;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem")
    int32 NumOpenPublicConnections;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem")
    FString SessionId;

    static FOnlineSessionBP FromNative(const FOnlineSession &InObj);
    FOnlineSession ToNative();
};

USTRUCT(BlueprintType) struct FNamedOnlineSessionBP : public FOnlineSessionBP
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem")
    FName SessionName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem")
    bool bHosting;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem")
    FUniqueNetIdRepl LocalOwnerId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem")
    TArray<FUniqueNetIdRepl> RegisteredPlayers;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem")
    EOnlineSessionState_ SessionState;

    static FNamedOnlineSessionBP FromNative(const FNamedOnlineSession &InObj);
};

UCLASS(BlueprintType, Transient, Meta = (DontUseGenericSpawnObject, IsBlueprintBase = "false"))
class MECHANICS_API UNamedOnlineSession : public UObject
{
    GENERATED_BODY()

private:
    FNamedOnlineSessionBP Value;

public:
    static UNamedOnlineSession *FromNative(class FNamedOnlineSession *InSession);

    UFUNCTION(BlueprintPure, Category = "MyOnlineSubsystem")
    FNamedOnlineSessionBP GetValue()
    {
        return this->Value;
    }
};

USTRUCT(BlueprintType)
struct FOnlineSessionSearchResultBP
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FOnlineSessionBP Session;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    int PingInMs;

    static FOnlineSessionSearchResultBP FromNative(const FOnlineSessionSearchResult &InObj);
    FOnlineSessionSearchResult ToNative();
};

UENUM(BlueprintType, Meta = (ScriptName = "EOnlineAsyncTaskState"))
enum class EOnlineAsyncTaskState_ : uint8
{
    NotStarted,
    InProgress,
    Done,
    Failed
};

UENUM(BlueprintType, Meta = (ScriptName = "EOnlineComparisonOp"))
enum class EOnlineComparisonOp_ : uint8
{
    Equals,
    NotEquals,
    GreaterThan,
    GreaterThanEquals,
    LessThan,
    LessThanEquals,
    Near,
    In,
    NotIn
};

USTRUCT(BlueprintType)
struct FSessionSearchParamBP
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FVariantDataBP Data;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    EOnlineComparisonOp_ Op;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    int32 ID;
};

UCLASS(BlueprintType, Meta = (IsBlueprintBase = "false"))
class MECHANICS_API UOnlineSessionSearch : public UObject
{
    GENERATED_BODY()

private:
    TSharedPtr<class FOnlineSessionSearch> Search;
    TSharedPtr<TSharedRef<class FOnlineSessionSearch>> SearchTemp;
    void SyncPropertiesFromNative();
    void SyncPropertiesToNative();

public:
    virtual void PostInitProperties() override;

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem")
    TArray<FOnlineSessionSearchResultBP> GetSearchResults() const;

    UFUNCTION(BlueprintPure, Category = "MyOnlineSubsystem")
    EOnlineAsyncTaskState_ GetSearchState() const;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    int32 MaxSearchResults;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool bIsLanQuery;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    int32 PingBucketSize;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    int32 PlatformHash;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    float TimeoutInSeconds;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    TMap<FName, FSessionSearchParamBP> SearchParams;

    TSharedRef<class FOnlineSessionSearch> ToNative();
    TSharedRef<class FOnlineSessionSearch> &ToNativeRef();
    bool IsValid() const;
};