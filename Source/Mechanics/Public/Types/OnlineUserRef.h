// Copyright June Rhodes. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/OnlineReplStructs.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineUserInterface.h"
#include "Online/CoreOnline.h"

#include "OnlineUserRef.generated.h"

UCLASS(BlueprintType, Blueprintable, Transient, Meta = (DontUseGenericSpawnObject))
class MECHANICS_API UOnlineUserRef : public UObject
{
    GENERATED_BODY()

private:
    TSharedPtr<FOnlineUser> User;
    TSharedPtr<FOnlineBlockedPlayer> BlockedPlayer;

protected:
    void AssignFromUser(const TSharedPtr<FOnlineUser> &User);
    void AssignFromBlockedPlayer(const TSharedPtr<FOnlineBlockedPlayer> &BlockedPlayer);

public:
    static UOnlineUserRef *FromUser(const TSharedPtr<FOnlineUser> &User);
    TSharedPtr<FOnlineUser> GetUser() const
    {
        return this->User;
    }

    static UOnlineUserRef *FromBlockedPlayer(const TSharedPtr<FOnlineBlockedPlayer> &BlockedPlayer);
    TSharedPtr<FOnlineBlockedPlayer> GetBlockedPlayer() const
    {
        return this->BlockedPlayer;
    }

    UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "MyOnlineSubsystem")
    FUniqueNetIdRepl GetUserId();

    UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "MyOnlineSubsystem")
    FString GetRealName();

    UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "MyOnlineSubsystem")
    FString GetDisplayName();

    UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "MyOnlineSubsystem")
    FString GetUserAttribute(const FString &Key, bool &Found);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MyOnlineSubsystem")
    void SetUserLocalAttribute(const FString &Key, const FString &Value, bool &Success);

protected:
    virtual FUniqueNetIdRepl GetUserId_Native();
    virtual FString GetRealName_Native();
    virtual FString GetDisplayName_Native();
    virtual FString GetUserAttribute_Native(const FString &Key, bool &Found);
    virtual void SetUserLocalAttribute_Native(const FString &Key, const FString &Value, bool &Success);
};

UCLASS(BlueprintType, Transient, Meta = (DontUseGenericSpawnObject, IsBlueprintBase = "false"))
class MECHANICS_API UUserOnlineAccountRef : public UOnlineUserRef
{
    GENERATED_BODY()

private:
    TSharedPtr<FUserOnlineAccount> UserOnlineAccount;

protected:
    void AssignFromUserOnlineAccount(const TSharedPtr<FUserOnlineAccount> &User);

public:
    static UUserOnlineAccountRef *FromUserOnlineAccount(const TSharedPtr<FUserOnlineAccount> &BlockedPlayer);
    TSharedPtr<FUserOnlineAccount> GetUserOnlineAccount() const
    {
        return this->UserOnlineAccount;
    }

    UFUNCTION(BlueprintPure, Category = "MyOnlineSubsystem")
    FString GetAccessToken();

    UFUNCTION(BlueprintPure, Category = "MyOnlineSubsystem")
    FString GetAuthAttribute(FString Key, bool &Found);

    UFUNCTION(BlueprintPure, Category = "MyOnlineSubsystem")
    bool SetUserAttribute(FString Key, FString Value);
};

USTRUCT(BlueprintType) struct MECHANICS_API FExternalIdQueryOptionsBP
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    bool bLookupByDisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyOnlineSubsystem")
    FString AuthType;

    static FExternalIdQueryOptionsBP FromNative(const FExternalIdQueryOptions &Obj);
    FExternalIdQueryOptions ToNative();
};