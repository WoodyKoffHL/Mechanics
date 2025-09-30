// Copyright June Rhodes. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Delegates/Delegate.h"
#include "Delegates/DelegateCombinations.h"
#include "GameFramework/OnlineReplStructs.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Types/OnlineUserRef.h"
#include "Types/FriendTypes.h"
#include "Types/OnlineAccountCredential.h"
#include "Types/OnlineErrorInfo.h"
#include "Types/OnlineRecentPlayerRef.h"
#include "Types/OnlineLoginStatus.h"
#include "Types/BlockedQueryResultInfo.h"
#include "Types/OnlineUserPrivilege.h"
#include "Types/MessageTypes.h"
#include "Types/ExternalUITypes.h"
#include "Types/OSBFileTypes.h"
#include "Types/OSBPartyTypes.h"
#include "Types/OSBLobbyTypes.h"
#include "Types/OSBSessionTypes.h"
#include "Types/OSBLeaderboardTypes.h"
#include "Types/OSBStatsTypes.h"
#include "Types/OSBAchievementTypes.h"
#include "Helpers/ArrayConversion.h"
#include "Helpers/UniqueNetIdConversion.h"
#include "Interfaces/OnlineExternalUIInterface.h"

#include "OnlineExternalUISubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FExternalUI_ExternalUIChange_BP, bool, Param1);

UCLASS(BlueprintType)
class MECHANICS_API UOnlineExternalUISubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

    friend class UOnlineHelpers;

private:
    struct FWorldContext* WorldContext;
    TWeakPtr<class IOnlineExternalUI, ESPMode::ThreadSafe> HandleWk;
    TSharedPtr<class IOnlineExternalUI, ESPMode::ThreadSafe> GetHandle();
    bool IsHandleValid(const TSharedPtr<class IOnlineExternalUI, ESPMode::ThreadSafe>& InHandle);
    FDelegateHandle DelegateHandle_OnExternalUIChange;

public:
    TSet<FString> __InFlightRequests;

    UOnlineExternalUISubsystem();
    virtual void BeginDestroy() override;

    UFUNCTION(BlueprintPure, Category = "MyOnlineSubsystem|ExternalUI")
    bool IsSubsystemAvailable();
private:
    UPROPERTY()
    TArray<FExternalUIFlowHandlerRegistration> FlowHandlers;
    int NextFlowHandlersRequestID;
    TMap<int, FOnLoginFlowComplete> LoginFlowCompleteDelegates;
    TMap<int, FOnLoginRedirectURL> LoginRedirectURLDelegates;

public:
    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|ExternalUI")
    void AddLoginFlowHandler(const TScriptInterface<IExternalUIFlowHandler>& Handler);

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|ExternalUI")
    void RemoveLoginFlowHandler(const TScriptInterface<IExternalUIFlowHandler>& Handler);

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|ExternalUI")
    FLoginFlowResultBP NotifyLoginRedirectURL(int RequestID, const FString &URL);

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|ExternalUI")
    void LoginFlowComplete(int RequestID, FLoginFlowResultBP Result);

    UPROPERTY(BlueprintAssignable, Category = "MyOnlineSubsystem|ExternalUI")
    FExternalUI_ExternalUIChange_BP OnExternalUIChange;

    friend class UOnlineExternalUISubsystemShowLoginUI;
    
    friend class UOnlineExternalUISubsystemShowAccountCreationUI;
    
    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|ExternalUI")
    bool ShowFriendsUI(int32 LocalUserNum);

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|ExternalUI")
    bool ShowInviteUI(int32 LocalUserNum, FName SessionName);

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|ExternalUI")
    bool ShowAchievementsUI(int32 LocalUserNum);

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|ExternalUI")
    bool ShowLeaderboardUI(FString LeaderboardName);

    friend class UOnlineExternalUISubsystemShowWebURL;
    
    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|ExternalUI")
    bool CloseWebURL();

    friend class UOnlineExternalUISubsystemShowProfileUI;
    
    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|ExternalUI")
    bool ShowAccountUpgradeUI(FUniqueNetIdRepl UniqueId);

    friend class UOnlineExternalUISubsystemShowStoreUI;
    
    friend class UOnlineExternalUISubsystemShowSendMessageUI;
    
    friend class UOnlineExternalUISubsystemShowSendMessageToUserUI;
    
    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|ExternalUI")
    bool ShowPlatformMessageBox(FUniqueNetIdRepl UserId, EPlatformMessageType_ MessageType);

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|ExternalUI")
    void ReportEnterInGameStoreUI();

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|ExternalUI")
    void ReportExitInGameStoreUI();
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnlineExternalUISubsystemShowLoginUICallbackPin, FUniqueNetIdRepl, UniqueId, int, ControllerIndex, FOnlineErrorInfo, Error);

UCLASS()
class MECHANICS_API UOnlineExternalUISubsystemShowLoginUI : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnlineExternalUISubsystemShowLoginUICallbackPin OnCallFailed;
    UPROPERTY(BlueprintAssignable)
    FOnlineExternalUISubsystemShowLoginUICallbackPin OnLoginUIClosed;

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|ExternalUI", meta = (BlueprintInternalUseOnly = "true"))
    static UOnlineExternalUISubsystemShowLoginUI* ShowLoginUI(
        UOnlineExternalUISubsystem* Subsystem
        ,int ControllerIndex, bool bShowOnlineOnly, bool bShowSkipButton
    );

    virtual void Activate() override;

private:
    UPROPERTY()
    UOnlineExternalUISubsystem* __Store__Subsystem;
    UPROPERTY()
    FString __Store__CallUniquenessId;
    void HandleCallback_OnLoginUIClosed(FUniqueNetIdPtr UniqueId, const int ControllerIndex, const FOnlineError& Error);

    UPROPERTY()
    int __Store__ControllerIndex;

UPROPERTY()
    bool __Store__bShowOnlineOnly;

UPROPERTY()
    bool __Store__bShowSkipButton;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnlineExternalUISubsystemShowAccountCreationUICallbackPin, int, ControllerIndex, FOnlineAccountCredential, AccountCredentials, FOnlineErrorInfo, Error);

UCLASS()
class MECHANICS_API UOnlineExternalUISubsystemShowAccountCreationUI : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnlineExternalUISubsystemShowAccountCreationUICallbackPin OnCallFailed;
    UPROPERTY(BlueprintAssignable)
    FOnlineExternalUISubsystemShowAccountCreationUICallbackPin OnAccountCreationUIClosed;

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|ExternalUI", meta = (BlueprintInternalUseOnly = "true"))
    static UOnlineExternalUISubsystemShowAccountCreationUI* ShowAccountCreationUI(
        UOnlineExternalUISubsystem* Subsystem
        ,int ControllerIndex
    );

    virtual void Activate() override;

private:
    UPROPERTY()
    UOnlineExternalUISubsystem* __Store__Subsystem;
    UPROPERTY()
    FString __Store__CallUniquenessId;
    void HandleCallback_OnAccountCreationUIClosed(const int ControllerIndex, const FOnlineAccountCredentials& AccountCredentials, const FOnlineError& Error);

    UPROPERTY()
    int __Store__ControllerIndex;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnlineExternalUISubsystemShowWebURLCallbackPin, FString, FinalUrl);

UCLASS()
class MECHANICS_API UOnlineExternalUISubsystemShowWebURL : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnlineExternalUISubsystemShowWebURLCallbackPin OnCallFailed;
    UPROPERTY(BlueprintAssignable)
    FOnlineExternalUISubsystemShowWebURLCallbackPin OnShowWebUrlClosed;

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|ExternalUI", meta = (BlueprintInternalUseOnly = "true"))
    static UOnlineExternalUISubsystemShowWebURL* ShowWebURL(
        UOnlineExternalUISubsystem* Subsystem
        ,FString Url, FShowWebUrlParameters ShowParams
    );

    virtual void Activate() override;

private:
    UPROPERTY()
    UOnlineExternalUISubsystem* __Store__Subsystem;
    UPROPERTY()
    FString __Store__CallUniquenessId;
    void HandleCallback_OnShowWebUrlClosed(const FString& FinalUrl);

    UPROPERTY()
    FString __Store__Url;

UPROPERTY()
    FShowWebUrlParameters __Store__ShowParams;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnlineExternalUISubsystemShowProfileUICallbackPin);

UCLASS()
class MECHANICS_API UOnlineExternalUISubsystemShowProfileUI : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnlineExternalUISubsystemShowProfileUICallbackPin OnCallFailed;
    UPROPERTY(BlueprintAssignable)
    FOnlineExternalUISubsystemShowProfileUICallbackPin OnProfileUIClosed;

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|ExternalUI", meta = (BlueprintInternalUseOnly = "true"))
    static UOnlineExternalUISubsystemShowProfileUI* ShowProfileUI(
        UOnlineExternalUISubsystem* Subsystem
        ,FUniqueNetIdRepl Requestor, FUniqueNetIdRepl Requestee
    );

    virtual void Activate() override;

private:
    UPROPERTY()
    UOnlineExternalUISubsystem* __Store__Subsystem;
    UPROPERTY()
    FString __Store__CallUniquenessId;
    void HandleCallback_OnProfileUIClosed();

    UPROPERTY()
    FUniqueNetIdRepl __Store__Requestor;

UPROPERTY()
    FUniqueNetIdRepl __Store__Requestee;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnlineExternalUISubsystemShowStoreUICallbackPin, bool, bPurchased);

UCLASS()
class MECHANICS_API UOnlineExternalUISubsystemShowStoreUI : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnlineExternalUISubsystemShowStoreUICallbackPin OnCallFailed;
    UPROPERTY(BlueprintAssignable)
    FOnlineExternalUISubsystemShowStoreUICallbackPin OnShowStoreUIClosed;

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|ExternalUI", meta = (BlueprintInternalUseOnly = "true"))
    static UOnlineExternalUISubsystemShowStoreUI* ShowStoreUI(
        UOnlineExternalUISubsystem* Subsystem
        ,int32 LocalUserNum, FShowStoreParameters ShowParams
    );

    virtual void Activate() override;

private:
    UPROPERTY()
    UOnlineExternalUISubsystem* __Store__Subsystem;
    UPROPERTY()
    FString __Store__CallUniquenessId;
    void HandleCallback_OnShowStoreUIClosed(bool bPurchased);

    UPROPERTY()
    int32 __Store__LocalUserNum;

UPROPERTY()
    FShowStoreParameters __Store__ShowParams;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnlineExternalUISubsystemShowSendMessageUICallbackPin, bool, bMessageSent);

UCLASS()
class MECHANICS_API UOnlineExternalUISubsystemShowSendMessageUI : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnlineExternalUISubsystemShowSendMessageUICallbackPin OnCallFailed;
    UPROPERTY(BlueprintAssignable)
    FOnlineExternalUISubsystemShowSendMessageUICallbackPin OnShowSendMessageUIClosed;

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|ExternalUI", meta = (BlueprintInternalUseOnly = "true"))
    static UOnlineExternalUISubsystemShowSendMessageUI* ShowSendMessageUI(
        UOnlineExternalUISubsystem* Subsystem
        ,int32 LocalUserNum, FShowSendMessageParameters ShowParams
    );

    virtual void Activate() override;

private:
    UPROPERTY()
    UOnlineExternalUISubsystem* __Store__Subsystem;
    UPROPERTY()
    FString __Store__CallUniquenessId;
    void HandleCallback_OnShowSendMessageUIClosed(bool bMessageSent);

    UPROPERTY()
    int32 __Store__LocalUserNum;

UPROPERTY()
    FShowSendMessageParameters __Store__ShowParams;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnlineExternalUISubsystemShowSendMessageToUserUICallbackPin, bool, bMessageSent);

UCLASS()
class MECHANICS_API UOnlineExternalUISubsystemShowSendMessageToUserUI : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnlineExternalUISubsystemShowSendMessageToUserUICallbackPin OnCallFailed;
    UPROPERTY(BlueprintAssignable)
    FOnlineExternalUISubsystemShowSendMessageToUserUICallbackPin OnShowSendMessageUIClosed;

    UFUNCTION(BlueprintCallable, Category = "MyOnlineSubsystem|ExternalUI", meta = (BlueprintInternalUseOnly = "true"))
    static UOnlineExternalUISubsystemShowSendMessageToUserUI* ShowSendMessageToUserUI(
        UOnlineExternalUISubsystem* Subsystem
        ,int32 LocalUserNum, FUniqueNetIdRepl Recipient, FShowSendMessageParameters ShowParams
    );

    virtual void Activate() override;

private:
    UPROPERTY()
    UOnlineExternalUISubsystem* __Store__Subsystem;
    UPROPERTY()
    FString __Store__CallUniquenessId;
    void HandleCallback_OnShowSendMessageUIClosed(bool bMessageSent);

    UPROPERTY()
    int32 __Store__LocalUserNum;

UPROPERTY()
    FUniqueNetIdRepl __Store__Recipient;

UPROPERTY()
    FShowSendMessageParameters __Store__ShowParams;
};
