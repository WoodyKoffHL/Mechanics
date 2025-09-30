// Copyright June Rhodes. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/OnlineReplStructs.h"
#include "OnlineSubsystemTypes.h"
#include "Online/CoreOnline.h"

#include "OSBFileTypes.generated.h"

UCLASS(BlueprintType, Transient, Meta = (DontUseGenericSpawnObject, IsBlueprintBase = "false"))
class MECHANICS_API UFileData : public UObject
{
    GENERATED_BODY()

private:
    TArray<uint8> Data;

public:
    static UFileData *FromNative(const TArray<uint8> &InData);
    TArray<uint8> ToNative() const
    {
        return this->Data;
    };
    const TArray<uint8> &GetData() const
    {
        return this->Data;
    };
};

USTRUCT(BlueprintType)
struct MECHANICS_API FCloudFileHeaderBP
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem|Files")
    FString Hash;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem|Files")
    FName HashType;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem|Files")
    FString DLName;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem|Files")
    FString FileName;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem|Files")
    int32 FileSize;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem|Files")
    FString URL;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyOnlineSubsystem|Files")
    int64 ChunkID;

    static FCloudFileHeaderBP FromNative(const FCloudFileHeader &FileHeader);
};

USTRUCT(BlueprintType)
struct MECHANICS_API FPagedQueryBP
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|Files")
    int32 Start;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem|Files")
    int32 Count;

    FPagedQuery ToNative() const;
};