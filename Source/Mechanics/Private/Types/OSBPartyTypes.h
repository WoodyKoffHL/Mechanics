// Copyright June Rhodes. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/OnlineReplStructs.h"
#include "Interfaces/OnlinePartyInterface.h"
#include "OSBSessionTypes.h"
#include "Online/CoreOnline.h"

#include "OSBPartyTypes.generated.h"

UCLASS(BlueprintType, Transient, Meta = (DontUseGenericSpawnObject, IsBlueprintBase = "false"))
class MECHANICS_API UParty : public UObject
{
    GENERATED_BODY()

private:
    TWeakPtr<const FOnlineParty> Party;

public:
    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem")
    UPartyId *GetPartyId();

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem")
    int64 GetPartyTypeId();

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem")
    FUniqueNetIdRepl GetLeaderId();

    static UParty *FromNative(FOnlinePartyConstPtr InObj);
    FOnlinePartyConstPtr ToNative() const
    {
        if (this->Party.IsValid())
        {
            return this->Party.Pin();
        }

        return nullptr;
    }
    bool IsValid() const
    {
        return this->Party.IsValid();
    }
};

UENUM(BlueprintType, Meta = (ScriptName = "EMemberConnectionStatus"))
enum class EMemberConnectionStatus_ : uint8
{
    Uninitialized,
    Disconnected,
    Initializing,
    Connected
};

UENUM(BlueprintType, Meta = (ScriptName = "EMemberExitedReason"))
enum class EMemberExitedReason_ : uint8
{
    Unknown,
    Left,
    Removed,
    Kicked
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
    FPartyMemberAttributeChanged,
    FUniqueNetIdRepl,
    ChangedUserId,
    FString,
    Attribute,
    FString,
    NewValue,
    FString,
    OldValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
    FPartyMemberConnectionStatusChanged,
    FUniqueNetIdRepl,
    ChangedUserId,
    EMemberConnectionStatus_,
    NewStatus,
    EMemberConnectionStatus_,
    PreviousStatus);

UCLASS(BlueprintType, Transient, Meta = (DontUseGenericSpawnObject, IsBlueprintBase = "false"))
class MECHANICS_API UBlueprintPartyMember : public UObject
{
    GENERATED_BODY()

private:
    FOnlinePartyMemberConstPtr PartyMember;
    FDelegateHandle AttributeHandle;
    FDelegateHandle ConnectionStatusHandle;

    void OnMemberAttributeChanged(
        const FUniqueNetId &ChangedUserId,
        const FString &Attribute,
        const FString &NewValue,
        const FString &OldValue);
    void OnMemberConnectionStatusChanged(
        const FUniqueNetId &ChangedUserId,
        const EMemberConnectionStatus NewMemberConnectionStatus,
        const EMemberConnectionStatus PreviousMemberConnectionStatus);

public:
    virtual void BeginDestroy() override;

    UPROPERTY(BlueprintAssignable, Category = "MyOnlineSubsystem")
    FPartyMemberAttributeChanged OnAttributeChanged;

    UPROPERTY(BlueprintAssignable, Category = "MyOnlineSubsystem")
    FPartyMemberConnectionStatusChanged OnConnectionStatusChanged;

    UFUNCTION(BlueprintPure, Category = "MyOnlineSubsystem")
    FUniqueNetIdRepl GetUserId();

    UFUNCTION(BlueprintPure, Category = "MyOnlineSubsystem")
    FString GetRealName();

    UFUNCTION(BlueprintPure, Category = "MyOnlineSubsystem")
    FString GetDisplayName(const FString &Platform);

    UFUNCTION(BlueprintPure, Category = "MyOnlineSubsystem")
    bool GetUserAttribute(const FString &AttrName, FString &OutAttrValue);

    static UBlueprintPartyMember *FromNative(FOnlinePartyMemberConstPtr InObj);
    FOnlinePartyMemberConstPtr ToNative() const
    {
        return this->PartyMember;
    }
    bool IsValid() const
    {
        return this->PartyMember.IsValid();
    }
};

UCLASS(BlueprintType, Transient, Meta = (DontUseGenericSpawnObject, IsBlueprintBase = "false"))
class MECHANICS_API UReadablePartyData : public UObject
{
    GENERATED_BODY()

private:
    TSharedPtr<const FOnlinePartyData> ConstPartyData;

protected:
    void AssignFromConstPartyData(const FOnlinePartyData &InObj);

public:
    static UReadablePartyData *FromNative(const FOnlinePartyData &InObj);
    static UReadablePartyData *FromNative(TSharedPtr<const FOnlinePartyData> InObj);
    TSharedRef<const FOnlinePartyData> ToNativeConst() const
    {
        return this->ConstPartyData.ToSharedRef();
    }
    bool IsValid() const
    {
        return this->ConstPartyData.IsValid();
    }

    UFUNCTION(BlueprintPure, Category = "MyOnlineSubsystem")
    virtual void GetAttribute(const FString &AttrName, bool &OutFound, FVariantDataBP &OutAttrValue) const;
};

UCLASS(BlueprintType, Transient, Meta = (DontUseGenericSpawnObject, IsBlueprintBase = "false"))
class MECHANICS_API UMutablePartyData : public UReadablePartyData
{
    GENERATED_BODY()

private:
    TSharedPtr<FOnlinePartyData> MutablePartyData;

public:
    static UMutablePartyData *FromNative(FOnlinePartyData &InObj);
    static UMutablePartyData *FromNative(TSharedRef<FOnlinePartyData> InObj);
    static UMutablePartyData *FromNative(TSharedPtr<FOnlinePartyData> InObj);
    TSharedRef<FOnlinePartyData> ToNativeMutable() const
    {
        return this->MutablePartyData.ToSharedRef();
    }

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem")
    void SetAttribute(const FString &AttrName, FVariantDataBP AttrValue);

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem")
    void RemoveAttribute(const FString &AttrName);
};

UCLASS(BlueprintType, Transient, Meta = (DontUseGenericSpawnObject, IsBlueprintBase = "false"))
class MECHANICS_API UPartyId : public UObject
{
    GENERATED_BODY()

private:
    TSharedPtr<const FOnlinePartyId> PartyId;

public:
    static UPartyId *FromNative(const FOnlinePartyId &InObj);
    static UPartyId *FromNative(const TSharedPtr<const FOnlinePartyId> &InObj);
    const FOnlinePartyId &ToNative() const
    {
        return this->PartyId.ToSharedRef().Get();
    }
    bool IsValid() const
    {
        return this->PartyId.IsValid();
    }

    UFUNCTION(BlueprintPure, Category = "MyOnlineSubsystem")
    FString ToDebugString()
    {
        if (!this->IsValid())
        {
            return TEXT("");
        }

        return this->PartyId->ToDebugString();
    }
};

UCLASS(BlueprintType, Transient, Meta = (DontUseGenericSpawnObject, IsBlueprintBase = "false"))
class MECHANICS_API UOnlinePartyJoinInfo : public UObject
{
    GENERATED_BODY()

private:
    TSharedPtr<const IOnlinePartyJoinInfo> OnlinePartyJoinInfo;

public:
    static UOnlinePartyJoinInfo *FromNative(const IOnlinePartyJoinInfo &InObj);
    const IOnlinePartyJoinInfo &ToNative() const
    {
        return this->OnlinePartyJoinInfo.ToSharedRef().Get();
    }
    bool IsValid() const
    {
        return this->OnlinePartyJoinInfo.IsValid();
    }

    UFUNCTION(BlueprintPure, Category = "MyOnlineSubsystem")
    UPartyId *GetPartyId();

    UFUNCTION(BlueprintPure, Category = "MyOnlineSubsystem")
    FString GetSourceDisplayName();

    UFUNCTION(BlueprintPure, Category = "MyOnlineSubsystem")
    FUniqueNetIdRepl GetSourceUserId();

    UFUNCTION(BlueprintPure, Category = "MyOnlineSubsystem")
    FString ToDebugString();
};

UENUM(BlueprintType, Meta = (ScriptName = "EPartyState"))
enum class EPartyState_ : uint8
{
    None,
    CreatePending,
    JoinPending,
    RejoinPending,
    LeavePending,
    Active,
    Disconnected,
    CleanUp
};

UENUM(BlueprintType, Meta = (ScriptName = "EPartySystemPermissions"))
enum class EPartySystemPermissions : uint8
{
    Noone,
    Leader,
    Friends,
    Anyone
};

UENUM(BlueprintType, Meta = (ScriptName = "EJoinRequestAction"))
enum class EJoinRequestAction_ : uint8
{
    Manual,
    AutoApprove,
    AutoReject
};

USTRUCT(BlueprintType)
struct FOnlinePartyConfiguration
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    EJoinRequestAction_ JoinRequestAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    EPartySystemPermissions PresencePermissions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    EPartySystemPermissions InvitePermissions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool ChatEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool ShouldRemoveOnDisconnection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool IsAcceptingMembers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    int32 NotAcceptingMembersReason;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    int32 MaxMembers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FString Nickname;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FString Password;

    static FOnlinePartyConfiguration FromNative(const FPartyConfiguration &Config);
    TSharedPtr<FPartyConfiguration> ToNative();
};

UENUM(BlueprintType, Meta = (ScriptName = "ECreatePartyCompletionResult"))
enum class ECreatePartyCompletionResult_ : uint8
{
    UnknownClientFailure = (uint8)-100,
    AlreadyInPartyOfSpecifiedType,
    AlreadyCreatingParty,
    AlreadyInParty,
    FailedToCreateMucRoom,
    NoResponse,
    LoggedOut,
    NotPrimaryUser,
    UnknownInternalFailure = 0,
    Succeeded = 1
};

UENUM(BlueprintType, Meta = (ScriptName = "EJoinPartyCompletionResult"))
enum class EJoinPartyCompletionResult_ : uint8
{
    UnknownClientFailure = (uint8)-100,
    BadBuild,
    InvalidAccessKey,
    AlreadyInLeadersJoiningList,
    AlreadyInLeadersPartyRoster,
    NoSpace,
    NotApproved,
    RequesteeNotMember,
    RequesteeNotLeader,
    NoResponse,
    LoggedOut,
    UnableToRejoin,
    IncompatiblePlatform,
    AlreadyJoiningParty,
    AlreadyInParty,
    JoinInfoInvalid,
    AlreadyInPartyOfSpecifiedType,
    MessagingFailure,
    GameSpecificReason,

    UnknownInternalFailure = 0,

    Succeeded = 1
};

UENUM(BlueprintType, Meta = (ScriptName = "ELeavePartyCompletionResult"))
enum class ELeavePartyCompletionResult_ : uint8
{
    UnknownClientFailure = (uint8)-100,
    NoResponse,
    LoggedOut,

    UnknownParty,
    LeavePending,

    UnknownLocalUser,
    NotMember,
    MessagingFailure,
    UnknownTransportFailure,
    UnknownInternalFailure = 0,

    Succeeded = 1
};

UENUM(BlueprintType, Meta = (ScriptName = "EUpdateConfigCompletionResult"))
enum class EUpdateConfigCompletionResult_ : uint8
{
    UnknownClientFailure = (uint8)-100,
    UnknownParty,
    LocalMemberNotMember,
    LocalMemberNotLeader,
    RemoteMemberNotMember,
    MessagingFailure,
    NoResponse,
    UnknownInternalFailure = 0,
    Succeeded = 1
};

UENUM(BlueprintType, Meta = (ScriptName = "ERequestPartyInvitationCompletionResult"))
enum class ERequestPartyInvitationCompletionResult_ : uint8
{
    NotLoggedIn = (uint8)-100,
    InvitePending,
    AlreadyInParty,
    PartyFull,
    NoPermission,
    UnknownInternalFailure = 0,
    Succeeded = 1
};

UENUM(BlueprintType, Meta = (ScriptName = "ESendPartyInvitationCompletionResult"))
enum class ESendPartyInvitationCompletionResult_ : uint8
{
    NotLoggedIn = (uint8)-100,
    InvitePending,
    AlreadyInParty,
    PartyFull,
    NoPermission,
    RateLimited,
    UnknownInternalFailure = 0,
    Succeeded = 1
};

UENUM(BlueprintType, Meta = (ScriptName = "EAcceptPartyInvitationCompletionResult"))
enum class EAcceptPartyInvitationCompletionResult_ : uint8
{
    NotLoggedIn = (uint8)-100,
    InvitePending,
    AlreadyInParty,
    PartyFull,
    NoPermission,
    UnknownInternalFailure = 0,
    Succeeded = 1
};

UENUM(BlueprintType, Meta = (ScriptName = "ERejectPartyInvitationCompletionResult"))
enum class ERejectPartyInvitationCompletionResult_ : uint8
{
    NotLoggedIn = (uint8)-100,
    InvitePending,
    AlreadyInParty,
    PartyFull,
    NoPermission,
    UnknownInternalFailure = 0,
    Succeeded = 1
};

UENUM(BlueprintType, Meta = (ScriptName = "EKickMemberCompletionResult"))
enum class EKickMemberCompletionResult_ : uint8
{
    UnknownClientFailure = (uint8)-100,
    UnknownParty,
    LocalMemberNotMember,
    LocalMemberNotLeader,
    RemoteMemberNotMember,
    MessagingFailure,
    NoResponse,
    LoggedOut,
    UnknownInternalFailure = 0,
    Succeeded = 1
};

UENUM(BlueprintType, Meta = (ScriptName = "EPromoteMemberCompletionResult"))
enum class EPromoteMemberCompletionResult_ : uint8
{
    UnknownClientFailure = (uint8)-100,
    UnknownServiceFailure,
    UnknownParty,
    LocalMemberNotMember,
    LocalMemberNotLeader,
    PromotionAlreadyPending,
    TargetIsSelf,
    TargetNotMember,
    MessagingFailure,
    NoResponse,
    LoggedOut,
    UnknownInternalFailure = 0,
    Succeeded = 1
};

UENUM(BlueprintType, Meta = (ScriptName = "EInvitationResponse"))
enum class EInvitationResponse_ : uint8
{
    UnknownFailure,
    BadBuild,
    Rejected,
    Accepted
};