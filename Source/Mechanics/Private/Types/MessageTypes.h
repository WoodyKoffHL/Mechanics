// Copyright June Rhodes. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineMessageInterface.h"
#include "Online/CoreOnline.h"

#include "MessageTypes.generated.h"

USTRUCT(BlueprintType)
struct MECHANICS_API FOnlineMessagePayloadData
{
    GENERATED_BODY()

public:
    static FOnlineMessagePayloadData FromNative(const FOnlineMessagePayload &InObj);
    FOnlineMessagePayload ToNative() const;
};