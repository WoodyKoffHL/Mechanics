// Copyright June Rhodes. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Online/CoreOnline.h"

#include "OnlineAccountCredential.generated.h"

USTRUCT(BlueprintType)
struct MECHANICS_API FOnlineAccountCredential
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FString Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FString Id;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FString Token;

    static FOnlineAccountCredential FromNative(const FOnlineAccountCredentials &InCred);
    FOnlineAccountCredentials ToNative();
};