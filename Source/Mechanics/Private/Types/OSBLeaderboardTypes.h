// Copyright June Rhodes. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/OnlineReplStructs.h"
#include "Interfaces/OnlineLeaderboardInterface.h"
#include "OSBSessionTypes.h"
#include "Online/CoreOnline.h"

#include "OSBLeaderboardTypes.generated.h"

USTRUCT(BlueprintType)
struct MECHANICS_API FOnlineStatsRowBP
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem|Leaderboard")
    FString PlayerNickname;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem|Leaderboard")
    FUniqueNetIdRepl PlayerId;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem|Leaderboard")
    int32 Rank;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem|Leaderboard")
    TMap<FName, FVariantDataBP> Columns;

    static FOnlineStatsRowBP FromNative(FOnlineStatsRow &Row);
};

USTRUCT(BlueprintType)
struct MECHANICS_API FColumnMetaDataBP
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|Leaderboard")
    FName ColumnName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|Leaderboard")
    EOnlineKeyValuePairDataType_ DataType;

    static FColumnMetaDataBP FromNative(FColumnMetaData &Row);
    FColumnMetaData ToNative() const;
};

UCLASS(BlueprintType, Meta = (IsBlueprintBase = "false"))
class MECHANICS_API UOnlineLeaderboardRead : public UObject
{
    GENERATED_BODY()

private:
    UOnlineLeaderboardRead() : UObject(), Leaderboard(MakeShared<FOnlineLeaderboardRead, ESPMode::ThreadSafe>())
    {
    }

    UOnlineLeaderboardRead(const FObjectInitializer &ObjectInitializer)
        : UObject(ObjectInitializer), Leaderboard(MakeShared<FOnlineLeaderboardRead, ESPMode::ThreadSafe>())
    {
    }

    FOnlineLeaderboardReadRef Leaderboard;

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|Leaderboard")
    void SetLeaderboardName(FName LeaderboardName);

    UFUNCTION(BlueprintPure, Category = "MyOnlineSubsystem|Leaderboard")
    FName GetLeaderboardName() const;

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|Leaderboard")
    void SetSortedColumn(FName SortedColumn);

    UFUNCTION(BlueprintPure, Category = "MyOnlineSubsystem|Leaderboard")
    FName GetSortedColumn() const;

    UFUNCTION(BlueprintPure, Category = "MyOnlineSubsystem|Leaderboard")
    EOnlineAsyncTaskState_ GetReadState() const;

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|Leaderboard")
    void SetColumns(TArray<FColumnMetaDataBP> InColumns);

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|Leaderboard")
    TArray<FColumnMetaDataBP> GetColumns() const;

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|Leaderboard")
    TArray<FOnlineStatsRowBP> GetRows() const;

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|Leaderboard")
    FOnlineStatsRowBP FindPlayerRecord(FUniqueNetIdRepl UserId, bool &OutFound) const;

public:
    FOnlineLeaderboardReadRef &GetLeaderboard();
};

UCLASS(BlueprintType, Meta = (IsBlueprintBase = "false"))
class MECHANICS_API UOnlineLeaderboardWrite : public UObject
{
    GENERATED_BODY()

public:
    TSharedRef<FOnlineLeaderboardWrite> ToNative();
};