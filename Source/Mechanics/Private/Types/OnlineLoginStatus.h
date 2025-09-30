// Copyright June Rhodes. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "OnlineLoginStatus.generated.h"

UENUM(BlueprintType, Meta = (ScriptName = "ELoginStatus"))
enum class EOnlineLoginStatus : uint8
{
    NotLoggedIn,
    UsingLocalProfile,
    LoggedIn
};