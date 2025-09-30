// Copyright June Rhodes. MIT Licensed.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "OnlineDelegateMacros.h"
#include "OnlineSubsystem.h"

#define ONLINE_AVATAR_INTERFACE_NAME FName(TEXT("Avatar"))

DECLARE_DELEGATE_TwoParams(
    FOnGetAvatarComplete,
    bool /* bWasSuccessful */,
    TSoftObjectPtr<UTexture2D> /* ResultTexture */);

class IOnlineAvatar
{

public:
    virtual ~IOnlineAvatar(){};

    virtual bool GetAvatar(
        const FUniqueNetId &LocalUserId,
        const FUniqueNetId &TargetUserId,
        TSoftObjectPtr<UTexture2D> DefaultTexture,
        FOnGetAvatarComplete OnComplete = FOnGetAvatarComplete()) = 0;
};

namespace Online
{
FORCEINLINE TSharedPtr<IOnlineAvatar, ESPMode::ThreadSafe> GetAvatarInterface(IOnlineSubsystem *Subsystem)
{
    if (Subsystem == nullptr)
    {
        return nullptr;
    }

    void *Ptr = (void *)Subsystem->GetNamedInterface(ONLINE_AVATAR_INTERFACE_NAME);
    if (Ptr == nullptr)
    {
        return nullptr;
    }

    TSharedPtr<IOnlineAvatar, ESPMode::ThreadSafe> SharedPtr = *(TSharedPtr<IOnlineAvatar, ESPMode::ThreadSafe> *)Ptr;
    return SharedPtr;
}
} // namespace Online

