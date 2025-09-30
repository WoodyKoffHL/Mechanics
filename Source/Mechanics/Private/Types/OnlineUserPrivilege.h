// Copyright June Rhodes. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "OnlineUserPrivilege.generated.h"

UENUM(BlueprintType, Meta = (ScriptName = "EUserPrivileges"))
enum class EOnlineUserPrivilege : uint8
{
    CanPlay,
    CanPlayOnline,
    CanCommunicateOnline,
    CanUseUserGeneratedContent,
    CanUserCrossPlay
};