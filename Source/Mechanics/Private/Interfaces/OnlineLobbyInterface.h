// Copyright June Rhodes. MIT Licensed.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlinePartyInterface.h"
#include "OnlineDelegateMacros.h"
#include "OnlineSubsystem.h"

#define ONLINE_LOBBY_INTERFACE_NAME FName(TEXT("Lobby"))

typedef FOnlinePartyId FOnlineLobbyId;


DECLARE_MULTICAST_DELEGATE_TwoParams(
    FOnLobbyUpdate,
    const FUniqueNetId & /* UserId */,
    const FOnlineLobbyId & /* LobbyId */);
typedef FOnLobbyUpdate::FDelegate FOnLobbyUpdateDelegate;

DECLARE_MULTICAST_DELEGATE_TwoParams(
    FOnLobbyDelete,
    const FUniqueNetId & /* UserId */,
    const FOnlineLobbyId & /* LobbyId */);
typedef FOnLobbyDelete::FDelegate FOnLobbyDeleteDelegate;

DECLARE_MULTICAST_DELEGATE_ThreeParams(
    FOnMemberConnect,
    const FUniqueNetId & /* UserId */,
    const FOnlineLobbyId & /* LobbyId */,
    const FUniqueNetId & /* MemberId */);
typedef FOnMemberConnect::FDelegate FOnMemberConnectDelegate;

DECLARE_MULTICAST_DELEGATE_ThreeParams(
    FOnMemberUpdate,
    const FUniqueNetId & /* UserId */,
    const FOnlineLobbyId & /* LobbyId */,
    const FUniqueNetId & /* MemberId */);
typedef FOnMemberUpdate::FDelegate FOnMemberUpdateDelegate;

DECLARE_MULTICAST_DELEGATE_ThreeParams(
    FOnMemberDisconnect,
    const FUniqueNetId & /* UserId */,
    const FOnlineLobbyId & /* LobbyId */,
    const FUniqueNetId & /* MemberId */);
typedef FOnMemberDisconnect::FDelegate FOnMemberDisconnectDelegate;

DECLARE_DELEGATE_TwoParams(
    FOnLobbyOperationComplete,
    const FOnlineError & /* Error */,
    const FUniqueNetId & /* UserId */);
DECLARE_DELEGATE_ThreeParams(
    FOnLobbyCreateOrConnectComplete,
    const FOnlineError & /* Error */,
    const FUniqueNetId & /* UserId */,
    const TSharedPtr<class FOnlineLobby> & /* Lobby */);
DECLARE_DELEGATE_ThreeParams(
    FOnLobbySearchComplete,
    const FOnlineError & /* Error */,
    const FUniqueNetId & /* UserId */,
    const TArray<TSharedRef<const FOnlineLobbyId>> & /* Lobbies */);

class FOnlineLobby : public TSharedFromThis<FOnlineLobby>
{
protected:
    FOnlineLobby(){};

public:
    FOnlineLobby(const FOnlineLobby &) = delete;
    virtual ~FOnlineLobby(){};

    TSharedPtr<const FOnlineLobbyId> Id;
    TSharedPtr<const FUniqueNetId> OwnerId;
};

class FOnlineLobbyTransaction : public TSharedFromThis<FOnlineLobbyTransaction>
{
protected:
    FOnlineLobbyTransaction(){};

public:
    FOnlineLobbyTransaction(const FOnlineLobbyTransaction &) = delete;
    virtual ~FOnlineLobbyTransaction(){};

    TMap<FString, FVariantData> SetMetadata;

    TArray<FString> DeleteMetadata;

    TOptional<bool> Locked;

    TOptional<uint32> Capacity;

    TOptional<bool> Public;
};

class FOnlineLobbyMemberTransaction : public TSharedFromThis<FOnlineLobbyMemberTransaction>
{
protected:
    FOnlineLobbyMemberTransaction(){};

public:
    FOnlineLobbyMemberTransaction(const FOnlineLobbyMemberTransaction &) = delete;
    virtual ~FOnlineLobbyMemberTransaction(){};

    TMap<FString, FVariantData> SetMetadata;

    TArray<FString> DeleteMetadata;
};

enum class EOnlineLobbySearchQueryFilterComparator
{
    Equal = 0,
    NotEqual,
    LessThan,
    LessThanOrEqual,
    GreaterThan,
    GreaterThanOrEqual,
};

class FOnlineLobbySearchQueryFilter
{
public:
    virtual ~FOnlineLobbySearchQueryFilter(){};

    FOnlineLobbySearchQueryFilter(
        const FString &InKey,
        const FVariantData &InValue,
        EOnlineLobbySearchQueryFilterComparator InComparison)
        : Key(InKey), Value(InValue), Comparison(InComparison){};
    FOnlineLobbySearchQueryFilter(const FOnlineLobbySearchQueryFilter &Other)
        : Key(Other.Key), Value(Other.Value), Comparison(Other.Comparison){};

    FString Key;
    FVariantData Value;
    EOnlineLobbySearchQueryFilterComparator Comparison;
};

class FOnlineLobbySearchQuery
{
public:
    FOnlineLobbySearchQuery(){};
    virtual ~FOnlineLobbySearchQuery(){};

    TArray<FOnlineLobbySearchQueryFilter> Filters;

    TOptional<uint32> Limit;
};

class IOnlineLobby
{

public:
    virtual ~IOnlineLobby(){};

    DEFINE_ONLINE_DELEGATE_TWO_PARAM(
        OnLobbyUpdate,
        const FUniqueNetId & /* UserId */,
        const FOnlineLobbyId & /* LobbyId */);
    DEFINE_ONLINE_DELEGATE_TWO_PARAM(
        OnLobbyDelete,
        const FUniqueNetId & /* UserId */,
        const FOnlineLobbyId & /* LobbyId */);
    DEFINE_ONLINE_DELEGATE_THREE_PARAM(
        OnMemberConnect,
        const FUniqueNetId & /* UserId */,
        const FOnlineLobbyId & /* LobbyId */,
        const FUniqueNetId & /* MemberId */);
    DEFINE_ONLINE_DELEGATE_THREE_PARAM(
        OnMemberUpdate,
        const FUniqueNetId & /* UserId */,
        const FOnlineLobbyId & /* LobbyId */,
        const FUniqueNetId & /* MemberId */);
    DEFINE_ONLINE_DELEGATE_THREE_PARAM(
        OnMemberDisconnect,
        const FUniqueNetId & /* UserId */,
        const FOnlineLobbyId & /* LobbyId */,
        const FUniqueNetId & /* MemberId */);

    virtual FDateTime GetUtcNow() = 0;

    virtual TSharedPtr<FOnlineLobbyTransaction> MakeCreateLobbyTransaction(const FUniqueNetId &UserId) = 0;
    virtual TSharedPtr<FOnlineLobbyTransaction> MakeUpdateLobbyTransaction(
        const FUniqueNetId &UserId,
        const FOnlineLobbyId &LobbyId) = 0;
    virtual TSharedPtr<FOnlineLobbyMemberTransaction> MakeUpdateLobbyMemberTransaction(
        const FUniqueNetId &UserId,
        const FOnlineLobbyId &LobbyId,
        const FUniqueNetId &MemberId) = 0;

    virtual bool CreateLobby(
        const FUniqueNetId &UserId,
        const FOnlineLobbyTransaction &Transaction,
        FOnLobbyCreateOrConnectComplete OnComplete = FOnLobbyCreateOrConnectComplete()) = 0;
    virtual bool UpdateLobby(
        const FUniqueNetId &UserId,
        const FOnlineLobbyId &LobbyId,
        const FOnlineLobbyTransaction &Transaction,
        FOnLobbyOperationComplete OnComplete = FOnLobbyOperationComplete()) = 0;
    virtual bool DeleteLobby(
        const FUniqueNetId &UserId,
        const FOnlineLobbyId &LobbyId,
        FOnLobbyOperationComplete OnComplete = FOnLobbyOperationComplete()) = 0;

    virtual bool ConnectLobby(
        const FUniqueNetId &UserId,
        const FOnlineLobbyId &LobbyId,
        FOnLobbyCreateOrConnectComplete OnComplete = FOnLobbyCreateOrConnectComplete()) = 0;
    virtual bool DisconnectLobby(
        const FUniqueNetId &UserId,
        const FOnlineLobbyId &LobbyId,
        FOnLobbyOperationComplete OnComplete = FOnLobbyOperationComplete()) = 0;

    virtual bool UpdateMemberSelf(
        const FUniqueNetId &UserId,
        const FOnlineLobbyId &LobbyId,
        const FOnlineLobbyMemberTransaction &Transaction,
        FOnLobbyOperationComplete OnComplete = FOnLobbyOperationComplete()) = 0;

    virtual bool GetMemberCount(const FUniqueNetId &UserId, const FOnlineLobbyId &LobbyId, int32 &OutMemberCount) = 0;
    virtual bool GetMemberUserId(
        const FUniqueNetId &UserId,
        const FOnlineLobbyId &LobbyId,
        int32 MemberIndex,
        TSharedPtr<const FUniqueNetId> &OutMemberId) = 0;

    virtual bool GetMemberMetadataValue(
        const FUniqueNetId &UserId,
        const FOnlineLobbyId &LobbyId,
        const FUniqueNetId &MemberId,
        const FString &MetadataKey,
        FVariantData &OutMetadataValue) = 0;

    virtual bool Search(
        const FUniqueNetId &UserId,
        const FOnlineLobbySearchQuery &Query,
        FOnLobbySearchComplete OnComplete = FOnLobbySearchComplete()) = 0;

    virtual bool GetLobbyMetadataValue(
        const FUniqueNetId &UserId,
        const FOnlineLobbyId &LobbyId,
        const FString &MetadataKey,
        FVariantData &OutMetadataValue) = 0;

    virtual TSharedPtr<FOnlineLobbyId> ParseSerializedLobbyId(const FString &InLobbyId) = 0;
};

namespace Online
{
FORCEINLINE TSharedPtr<IOnlineLobby, ESPMode::ThreadSafe> GetLobbyInterface(IOnlineSubsystem *Subsystem)
{
    if (Subsystem == nullptr)
    {
        return nullptr;
    }

    void *Ptr = (void *)Subsystem->GetNamedInterface(ONLINE_LOBBY_INTERFACE_NAME);
    if (Ptr == nullptr)
    {
        return nullptr;
    }

    TSharedPtr<IOnlineLobby, ESPMode::ThreadSafe> SharedPtr = *(TSharedPtr<IOnlineLobby, ESPMode::ThreadSafe> *)Ptr;
    return SharedPtr;
}
} // namespace Online

