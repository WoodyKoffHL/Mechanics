// Copyright June Rhodes. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineError.h"
#include "Online/CoreOnline.h"

#include "OnlineErrorInfo.generated.h"

USTRUCT(BlueprintType)
struct MECHANICS_API FOnlineErrorInfo
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool Successful;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FString ErrorRaw;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FString ErrorCode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FText ErrorMessage;

    static FOnlineErrorInfo FromNative(const FOnlineError &Obj);
    FOnlineError ToNative();
};

USTRUCT(BlueprintType, DisplayName = "OnlineActivityTasksToReset")
struct FOnlineActivityTasksToResetBP
{
    GENERATED_USTRUCT_BODY()

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Online")
        TArray<FString> InProgressTasks;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Online")
        TArray<FString> CompletedTasks;

    FOnlineActivityTasksToResetBP()
    {
        InProgressTasks.Add(TEXT(""));
        CompletedTasks.Add(TEXT(""));
    }
};

UENUM(BlueprintType, DisplayName = "OnlineActivityOutcome")
enum class EOnlineActivityOutcomeBP : uint8
{
    Completed,
    Failed,
    Cancelled
};