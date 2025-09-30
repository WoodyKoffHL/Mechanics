// Copyright June Rhodes. MIT Licensed.

#pragma once

#include "Containers/UnrealString.h"
#include "CoreMinimal.h"
#include "OnlineDelegateMacros.h"
#include "OnlineError.h"
#include "OnlineSubsystem.h"

#define ONLINE_VOICE_ADMIN_INTERFACE_NAME FName(TEXT("VoiceAdmin"))

struct FVoiceAdminChannelCredentials
{
public:
    TSharedPtr<const FUniqueNetId> TargetUserId;

    FString PlayerName;

    FString ChannelCredentials;
};

DECLARE_DELEGATE_ThreeParams(
    FOnVoiceAdminCreateChannelCredentialsComplete,
    const FOnlineError & /* Result */,
    const FUniqueNetId & /* LocalUserId */,
    const TArray<FVoiceAdminChannelCredentials> & /* Credentials */);
DECLARE_DELEGATE_ThreeParams(
    FOnVoiceAdminKickParticipantComplete,
    const FOnlineError & /* Result */,
    const FUniqueNetId & /* LocalUserId */,
    const FUniqueNetId & /* TargetUserId */);
DECLARE_DELEGATE_FourParams(
    FOnVoiceAdminSetParticipantHardMuteComplete,
    const FOnlineError & /* Result */,
    const FUniqueNetId & /* LocalUserId */,
    const FUniqueNetId & /* TargetUserId */,
    bool /* bIsNowMuted */);

class IOnlineVoiceAdmin
{

public:
    virtual ~IOnlineVoiceAdmin(){};

    virtual bool CreateChannelCredentials(
        const FUniqueNetId &LocalUserId,
        const FString &ChannelName,
        const TArray<TSharedRef<const FUniqueNetId>> &TargetUserIds,
        FOnVoiceAdminCreateChannelCredentialsComplete OnComplete = FOnVoiceAdminCreateChannelCredentialsComplete()) = 0;

    virtual bool KickParticipant(
        const FUniqueNetId &LocalUserId,
        const FString &ChannelName,
        const FUniqueNetId &TargetUserId,
        FOnVoiceAdminKickParticipantComplete OnComplete = FOnVoiceAdminKickParticipantComplete()) = 0;

    virtual bool SetParticipantHardMute(
        const FUniqueNetId &LocalUserId,
        const FString &ChannelName,
        const FUniqueNetId &TargetUserId,
        bool bMuted,
        FOnVoiceAdminSetParticipantHardMuteComplete OnComplete = FOnVoiceAdminSetParticipantHardMuteComplete()) = 0;
};

namespace Online
{
FORCEINLINE TSharedPtr<IOnlineVoiceAdmin, ESPMode::ThreadSafe> GetVoiceAdminInterface(IOnlineSubsystem *Subsystem)
{
    if (Subsystem == nullptr)
    {
        return nullptr;
    }

    void *Ptr = (void *)Subsystem->GetNamedInterface(ONLINE_VOICE_ADMIN_INTERFACE_NAME);
    if (Ptr == nullptr)
    {
        return nullptr;
    }

    TSharedPtr<IOnlineVoiceAdmin, ESPMode::ThreadSafe> SharedPtr =
        *(TSharedPtr<IOnlineVoiceAdmin, ESPMode::ThreadSafe> *)Ptr;
    return SharedPtr;
}
} // namespace Online

