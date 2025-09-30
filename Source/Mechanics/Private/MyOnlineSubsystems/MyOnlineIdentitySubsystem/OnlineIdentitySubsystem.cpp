// Copyright June Rhodes. All Rights Reserved.

#include "MyOnlineSubsystems/MyOnlineIdentitySubsystem/OnlineIdentitySubsystem.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemTypes.h"

TSharedPtr<class IOnlineIdentity, ESPMode::ThreadSafe> UOnlineIdentitySubsystem::GetHandle()
{
    if (this->HasAnyFlags(RF_BeginDestroyed))
    {
        auto BDCheck_GameInstance = this->GetGameInstance();
        //check(IsValid(BDCheck_GameInstance));
    
        auto BDCheck_World = BDCheck_GameInstance->GetWorld();
        auto BDCheck_Pinned = this->HandleWk.Pin();

        if ((BDCheck_Pinned == nullptr || !BDCheck_Pinned.IsValid()) && !IsValid(BDCheck_World))
        {
            return nullptr;
        }
    }

    auto GameInstance = this->GetGameInstance();
    //check(IsValid(GameInstance));

    auto World = GameInstance->GetWorld();
    if (World == nullptr)
    {
        return nullptr;
    }

    auto CurrentWorldContext = GEngine->GetWorldContextFromWorld(World);
    //check(CurrentWorldContext != nullptr);
    //check(CurrentWorldContext->ContextHandle.IsEqual(this->WorldContext->ContextHandle));

    TSharedPtr<class IOnlineIdentity, ESPMode::ThreadSafe> Pinned = this->HandleWk.Pin();
    if (Pinned == nullptr || !Pinned.IsValid())
    {
        auto Subsystem = Online::GetSubsystem(World);
        //check(Subsystem != nullptr);
        
        auto __Handle__ = Subsystem->GetIdentityInterface();
        this->HandleWk = __Handle__;
        Pinned = __Handle__;
    }

    return Pinned;
}

bool UOnlineIdentitySubsystem::IsHandleValid(const TSharedPtr<class IOnlineIdentity, ESPMode::ThreadSafe>& InHandle)
{
    return InHandle.IsValid();
}

UOnlineIdentitySubsystem::UOnlineIdentitySubsystem()
{
    if (this->HasAnyFlags(RF_ClassDefaultObject))
    {
        return;
    }

    auto GameInstance = this->GetGameInstance();
    //check(IsValid(GameInstance));

    auto World = GameInstance->GetWorld();
    //check(IsValid(World));
    
    this->WorldContext = GEngine->GetWorldContextFromWorld(World);
    //check(WorldContext != nullptr);
    this->HandleWk = nullptr;
    this->GetHandle();
    
    TSharedPtr<class IOnlineIdentity, ESPMode::ThreadSafe> Handle = this->GetHandle();
    if (Handle.IsValid())
    {
        for (auto i = 0; i < MAX_LOCAL_PLAYERS; i++)
        {
            this->DelegateHandle_OnLoginStatusChanged.Add(Handle->AddOnLoginStatusChangedDelegate_Handle(
                i,
                FOnLoginStatusChanged::FDelegate::CreateLambda([this, i](int32 _UnusedLocalUserNum, ELoginStatus::Type OldStatus, ELoginStatus::Type NewStatus, const FUniqueNetId& NewId) {
                    this->OnLoginStatusChanged.Broadcast(i, (EOnlineLoginStatus)(OldStatus), (EOnlineLoginStatus)(NewStatus), ConvertDangerousUniqueNetIdToRepl(NewId));
                })
            ));
            this->DelegateHandle_OnLoginComplete.Add(Handle->AddOnLoginCompleteDelegate_Handle(
                i,
                FOnLoginComplete::FDelegate::CreateLambda([this, i](int32 _UnusedLocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error) {
                    this->OnLoginComplete.Broadcast(i, bWasSuccessful, ConvertDangerousUniqueNetIdToRepl(UserId), Error);
                })
            ));
            this->DelegateHandle_OnLogoutComplete.Add(Handle->AddOnLogoutCompleteDelegate_Handle(
                i,
                FOnLogoutComplete::FDelegate::CreateLambda([this, i](int32 _UnusedLocalUserNum, bool bWasSuccessful) {
                    this->OnLogoutComplete.Broadcast(i, bWasSuccessful);
                })
            ));
        }
        this->DelegateHandle_OnLoginChanged = Handle->AddOnLoginChangedDelegate_Handle(
            FOnLoginChanged::FDelegate::CreateLambda([this](int32 LocalUserNum) {
                this->OnLoginChanged.Broadcast(LocalUserNum);
            })
        );
        this->DelegateHandle_OnLoginFlowLogout = Handle->AddOnLoginFlowLogoutDelegate_Handle(
            FOnLoginFlowLogout::FDelegate::CreateLambda([this](const TArray<FString>& LoginDomains) {
                this->OnLoginFlowLogout.Broadcast(ConvertArrayElements<FString, FString>(LoginDomains, [](const FString& Val) { return Val; }));
            })
        );
    }
}

void UOnlineIdentitySubsystem::BeginDestroy()
{
    if (this->HasAnyFlags(RF_ClassDefaultObject))
    {
        Super::BeginDestroy();
        return;
    }

    TSharedPtr<class IOnlineIdentity, ESPMode::ThreadSafe> Handle = this->GetHandle();
    if (Handle.IsValid())
    {
        for (auto i = 0; i < MAX_LOCAL_PLAYERS; i++)
        {
            Handle->ClearOnLoginStatusChangedDelegate_Handle(i, this->DelegateHandle_OnLoginStatusChanged[i]);
            Handle->ClearOnLoginCompleteDelegate_Handle(i, this->DelegateHandle_OnLoginComplete[i]);
            Handle->ClearOnLogoutCompleteDelegate_Handle(i, this->DelegateHandle_OnLogoutComplete[i]);
        }
        Handle->ClearOnLoginChangedDelegate_Handle(this->DelegateHandle_OnLoginChanged);
        Handle->ClearOnLoginFlowLogoutDelegate_Handle(this->DelegateHandle_OnLoginFlowLogout);
        
    }

    Super::BeginDestroy();
}

bool UOnlineIdentitySubsystem::IsSubsystemAvailable()
{
    //check(!this->HasAnyFlags(RF_ClassDefaultObject));
    return this->IsHandleValid(this->GetHandle());
}

UOnlineIdentitySubsystemLogin* UOnlineIdentitySubsystemLogin::Login(UOnlineIdentitySubsystem* Subsystem ,int32 LocalUserNum, FOnlineAccountCredential AccountCredentials)
{
    if (!IsValid(Subsystem))
    {
        return nullptr;
    }

    //check(!Subsystem->HasAnyFlags(RF_ClassDefaultObject));

    UOnlineIdentitySubsystemLogin* Node = NewObject<UOnlineIdentitySubsystemLogin>();
    Node->__Store__Subsystem = Subsystem;
    Node->__Store__CallUniquenessId = FString::Printf(TEXT("OnLoginComplete|%d"), LocalUserNum);
    Node->__Store__LocalUserNum = LocalUserNum;
    Node->__Store__AccountCredentials = AccountCredentials;
    return Node;
}

void UOnlineIdentitySubsystemLogin::Activate()
{
    auto Handle = this->__Store__Subsystem->GetHandle();
    if (!this->__Store__Subsystem->IsHandleValid(Handle))
    {
        this->OnCallFailed.Broadcast(false, FUniqueNetIdRepl(), TEXT(""));
        return;
    }

    if (this->__Store__CallUniquenessId != TEXT(""))
    {
        if (this->__Store__Subsystem->__InFlightRequests.Contains(this->__Store__CallUniquenessId))
        {
            this->OnCallFailed.Broadcast(false, FUniqueNetIdRepl(), TEXT(""));
            return;
        }

        this->__Store__Subsystem->__InFlightRequests.Add(this->__Store__CallUniquenessId);
    }

    this->DelegateHandle_OnLoginComplete = Handle->AddOnLoginCompleteDelegate_Handle(this->__Store__LocalUserNum, 
        FOnLoginCompleteDelegate::CreateUObject(this, &UOnlineIdentitySubsystemLogin::HandleCallback_OnLoginComplete));
    
    if (!Handle->Login(this->__Store__LocalUserNum, this->__Store__AccountCredentials.ToNative()))
    {
        Handle->ClearOnLoginCompleteDelegate_Handle(this->__Store__LocalUserNum, this->DelegateHandle_OnLoginComplete);
        this->OnCallFailed.Broadcast(false, FUniqueNetIdRepl(), TEXT(""));
        return;
    }
}

void UOnlineIdentitySubsystemLogin::HandleCallback_OnLoginComplete(int32 LocalUserNumCb, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
    if (!IsValid(this->__Store__Subsystem))
    {
        this->OnCallFailed.Broadcast(false, FUniqueNetIdRepl(), TEXT(""));
        return;
    }
    if (LocalUserNumCb != this->__Store__LocalUserNum)
    {
        return;
    }

    auto Handle = this->__Store__Subsystem->GetHandle();
    if (!this->__Store__Subsystem->IsHandleValid(Handle))
    {
        this->OnCallFailed.Broadcast(false, FUniqueNetIdRepl(), TEXT(""));
        return;
    }
    if (this->__Store__CallUniquenessId != TEXT(""))
    {
        this->__Store__Subsystem->__InFlightRequests.Remove(this->__Store__CallUniquenessId);
    }
        Handle->ClearOnLoginCompleteDelegate_Handle(this->__Store__LocalUserNum, this->DelegateHandle_OnLoginComplete);

    this->OnLoginComplete.Broadcast(bWasSuccessful, ConvertDangerousUniqueNetIdToRepl(UserId), Error);
}

UOnlineIdentitySubsystemLogout* UOnlineIdentitySubsystemLogout::Logout(UOnlineIdentitySubsystem* Subsystem ,int32 LocalUserNum)
{
    if (!IsValid(Subsystem))
    {
        return nullptr;
    }

    //check(!Subsystem->HasAnyFlags(RF_ClassDefaultObject));

    UOnlineIdentitySubsystemLogout* Node = NewObject<UOnlineIdentitySubsystemLogout>();
    Node->__Store__Subsystem = Subsystem;
    Node->__Store__CallUniquenessId = FString::Printf(TEXT("OnLogoutComplete|%d"), LocalUserNum);
    Node->__Store__LocalUserNum = LocalUserNum;
    return Node;
}

void UOnlineIdentitySubsystemLogout::Activate()
{
    auto Handle = this->__Store__Subsystem->GetHandle();
    if (!this->__Store__Subsystem->IsHandleValid(Handle))
    {
        this->OnCallFailed.Broadcast(false);
        return;
    }

    if (this->__Store__CallUniquenessId != TEXT(""))
    {
        if (this->__Store__Subsystem->__InFlightRequests.Contains(this->__Store__CallUniquenessId))
        {
            this->OnCallFailed.Broadcast(false);
            return;
        }

        this->__Store__Subsystem->__InFlightRequests.Add(this->__Store__CallUniquenessId);
    }

    this->DelegateHandle_OnLogoutComplete = Handle->AddOnLogoutCompleteDelegate_Handle(this->__Store__LocalUserNum, 
        FOnLogoutCompleteDelegate::CreateUObject(this, &UOnlineIdentitySubsystemLogout::HandleCallback_OnLogoutComplete));
    
    if (!Handle->Logout(this->__Store__LocalUserNum))
    {
        Handle->ClearOnLogoutCompleteDelegate_Handle(this->__Store__LocalUserNum, this->DelegateHandle_OnLogoutComplete);
        this->OnCallFailed.Broadcast(false);
        return;
    }
}

void UOnlineIdentitySubsystemLogout::HandleCallback_OnLogoutComplete(int32 LocalUserNumCb, bool bWasSuccessful)
{
    if (!IsValid(this->__Store__Subsystem))
    {
        this->OnCallFailed.Broadcast(false);
        return;
    }
    if (LocalUserNumCb != this->__Store__LocalUserNum)
    {
        return;
    }

    auto Handle = this->__Store__Subsystem->GetHandle();
    if (!this->__Store__Subsystem->IsHandleValid(Handle))
    {
        this->OnCallFailed.Broadcast(false);
        return;
    }
    if (this->__Store__CallUniquenessId != TEXT(""))
    {
        this->__Store__Subsystem->__InFlightRequests.Remove(this->__Store__CallUniquenessId);
    }
        Handle->ClearOnLogoutCompleteDelegate_Handle(this->__Store__LocalUserNum, this->DelegateHandle_OnLogoutComplete);

    this->OnLogoutComplete.Broadcast(bWasSuccessful);
}

UOnlineIdentitySubsystemAutoLogin* UOnlineIdentitySubsystemAutoLogin::AutoLogin(UOnlineIdentitySubsystem* Subsystem ,int32 LocalUserNum)
{
    if (!IsValid(Subsystem))
    {
        return nullptr;
    }

    //check(!Subsystem->HasAnyFlags(RF_ClassDefaultObject));

    UOnlineIdentitySubsystemAutoLogin* Node = NewObject<UOnlineIdentitySubsystemAutoLogin>();
    Node->__Store__Subsystem = Subsystem;
    Node->__Store__CallUniquenessId = FString::Printf(TEXT("OnLoginComplete|%d"), LocalUserNum);
    Node->__Store__LocalUserNum = LocalUserNum;
    return Node;
}

void UOnlineIdentitySubsystemAutoLogin::Activate()
{
    auto Handle = this->__Store__Subsystem->GetHandle();
    if (!this->__Store__Subsystem->IsHandleValid(Handle))
    {
        this->OnCallFailed.Broadcast(false, FUniqueNetIdRepl(), TEXT(""));
        return;
    }

    if (this->__Store__CallUniquenessId != TEXT(""))
    {
        if (this->__Store__Subsystem->__InFlightRequests.Contains(this->__Store__CallUniquenessId))
        {
            this->OnCallFailed.Broadcast(false, FUniqueNetIdRepl(), TEXT(""));
            return;
        }

        this->__Store__Subsystem->__InFlightRequests.Add(this->__Store__CallUniquenessId);
    }

    this->DelegateHandle_OnLoginComplete = Handle->AddOnLoginCompleteDelegate_Handle(this->__Store__LocalUserNum, 
        FOnLoginCompleteDelegate::CreateUObject(this, &UOnlineIdentitySubsystemAutoLogin::HandleCallback_OnLoginComplete));
    
    if (!Handle->AutoLogin(this->__Store__LocalUserNum))
    {
        Handle->ClearOnLoginCompleteDelegate_Handle(this->__Store__LocalUserNum, this->DelegateHandle_OnLoginComplete);
        this->OnCallFailed.Broadcast(false, FUniqueNetIdRepl(), TEXT(""));
        return;
    }
}

void UOnlineIdentitySubsystemAutoLogin::HandleCallback_OnLoginComplete(int32 LocalUserNumCb, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
    if (!IsValid(this->__Store__Subsystem))
    {
        this->OnCallFailed.Broadcast(false, FUniqueNetIdRepl(), TEXT(""));
        return;
    }
    if (LocalUserNumCb != this->__Store__LocalUserNum)
    {
        return;
    }

    auto Handle = this->__Store__Subsystem->GetHandle();
    if (!this->__Store__Subsystem->IsHandleValid(Handle))
    {
        this->OnCallFailed.Broadcast(false, FUniqueNetIdRepl(), TEXT(""));
        return;
    }
    if (this->__Store__CallUniquenessId != TEXT(""))
    {
        this->__Store__Subsystem->__InFlightRequests.Remove(this->__Store__CallUniquenessId);
    }
        Handle->ClearOnLoginCompleteDelegate_Handle(this->__Store__LocalUserNum, this->DelegateHandle_OnLoginComplete);

    this->OnLoginComplete.Broadcast(bWasSuccessful, ConvertDangerousUniqueNetIdToRepl(UserId), Error);
}

UUserOnlineAccountRef* UOnlineIdentitySubsystem::GetUserAccount(FUniqueNetIdRepl UserId)
{
    //check(!this->HasAnyFlags(RF_ClassDefaultObject));
    auto Handle = this->GetHandle();
    if (!this->IsHandleValid(Handle))
    {
        return nullptr;
    }
    if (!(UserId.IsValid()))
    {
        return nullptr;
    }


    auto __Result = UUserOnlineAccountRef::FromUserOnlineAccount(Handle->GetUserAccount(UserId.GetUniqueNetId().ToSharedRef().Get()));

    return __Result;
}

TArray<UUserOnlineAccountRef*> UOnlineIdentitySubsystem::GetAllUserAccounts()
{
    //check(!this->HasAnyFlags(RF_ClassDefaultObject));
    auto Handle = this->GetHandle();
    if (!this->IsHandleValid(Handle))
    {
        return TArray<UUserOnlineAccountRef*>();
    }


    auto __Result = ConvertArrayElements<TSharedPtr<FUserOnlineAccount>, UUserOnlineAccountRef*>(Handle->GetAllUserAccounts(), [](const TSharedPtr<FUserOnlineAccount>& Val) { return UUserOnlineAccountRef::FromUserOnlineAccount(Val); });

    return __Result;
}

FUniqueNetIdRepl UOnlineIdentitySubsystem::GetUniquePlayerId(int32 LocalUserNum)
{
    //check(!this->HasAnyFlags(RF_ClassDefaultObject));
    auto Handle = this->GetHandle();
    if (!this->IsHandleValid(Handle))
    {
        return FUniqueNetIdRepl();
    }


    auto __Result = FUniqueNetIdRepl(Handle->GetUniquePlayerId(LocalUserNum));

    return __Result;
}

FUniqueNetIdRepl UOnlineIdentitySubsystem::GetSponsorUniquePlayerId(int32 LocalUserNum)
{
    //check(!this->HasAnyFlags(RF_ClassDefaultObject));
    auto Handle = this->GetHandle();
    if (!this->IsHandleValid(Handle))
    {
        return FUniqueNetIdRepl();
    }


    auto __Result = FUniqueNetIdRepl(Handle->GetSponsorUniquePlayerId(LocalUserNum));

    return __Result;
}

FUniqueNetIdRepl UOnlineIdentitySubsystem::CreateUniquePlayerId(FString Str)
{
    //check(!this->HasAnyFlags(RF_ClassDefaultObject));
    auto Handle = this->GetHandle();
    if (!this->IsHandleValid(Handle))
    {
        return FUniqueNetIdRepl();
    }


    auto __Result = FUniqueNetIdRepl(Handle->CreateUniquePlayerId(Str));

    return __Result;
}

EOnlineLoginStatus UOnlineIdentitySubsystem::GetLoginStatus(int32 LocalUserNum)
{
    //check(!this->HasAnyFlags(RF_ClassDefaultObject));
    auto Handle = this->GetHandle();
    if (!this->IsHandleValid(Handle))
    {
        return EOnlineLoginStatus();
    }


    auto __Result = (EOnlineLoginStatus)(Handle->GetLoginStatus(LocalUserNum));

    return __Result;
}

FString UOnlineIdentitySubsystem::GetPlayerNickname(FUniqueNetIdRepl UserId)
{
    //check(!this->HasAnyFlags(RF_ClassDefaultObject));
    auto Handle = this->GetHandle();
    if (!this->IsHandleValid(Handle))
    {
        return TEXT("");
    }
    if (!(UserId.IsValid()))
    {
        return TEXT("");
    }


    auto __Result = Handle->GetPlayerNickname(UserId.GetUniqueNetId().ToSharedRef().Get());

    return __Result;
}

FString UOnlineIdentitySubsystem::GetAuthToken(int32 LocalUserNum)
{
    //check(!this->HasAnyFlags(RF_ClassDefaultObject));
    auto Handle = this->GetHandle();
    if (!this->IsHandleValid(Handle))
    {
        return TEXT("");
    }


    auto __Result = Handle->GetAuthToken(LocalUserNum);

    return __Result;
}

UOnlineIdentitySubsystemRevokeAuthToken* UOnlineIdentitySubsystemRevokeAuthToken::RevokeAuthToken(UOnlineIdentitySubsystem* Subsystem ,FUniqueNetIdRepl LocalUserId)
{
    if (!IsValid(Subsystem))
    {
        return nullptr;
    }

    //check(!Subsystem->HasAnyFlags(RF_ClassDefaultObject));

    UOnlineIdentitySubsystemRevokeAuthToken* Node = NewObject<UOnlineIdentitySubsystemRevokeAuthToken>();
    Node->__Store__Subsystem = Subsystem;
    Node->__Store__CallUniquenessId = TEXT("");
    Node->__Store__LocalUserId = LocalUserId;
    return Node;
}

void UOnlineIdentitySubsystemRevokeAuthToken::Activate()
{
    auto Handle = this->__Store__Subsystem->GetHandle();
    if (!this->__Store__Subsystem->IsHandleValid(Handle))
    {
        this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), FOnlineErrorInfo());
        return;
    }
    if (!(this->__Store__LocalUserId.IsValid()))
    {
        this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), FOnlineErrorInfo());
        return;
    }

    if (this->__Store__CallUniquenessId != TEXT(""))
    {
        if (this->__Store__Subsystem->__InFlightRequests.Contains(this->__Store__CallUniquenessId))
        {
            this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), FOnlineErrorInfo());
            return;
        }

        this->__Store__Subsystem->__InFlightRequests.Add(this->__Store__CallUniquenessId);
    }

    auto __DelegateHandle_OnRevokeAuthTokenComplete = FOnRevokeAuthTokenCompleteDelegate::CreateUObject(this, &UOnlineIdentitySubsystemRevokeAuthToken::HandleCallback_OnRevokeAuthTokenComplete);

    Handle->RevokeAuthToken(this->__Store__LocalUserId.GetUniqueNetId().ToSharedRef().Get(), __DelegateHandle_OnRevokeAuthTokenComplete);
}

void UOnlineIdentitySubsystemRevokeAuthToken::HandleCallback_OnRevokeAuthTokenComplete(const FUniqueNetId& UserId, const FOnlineError& OnlineError)
{
    if (!IsValid(this->__Store__Subsystem))
    {
        this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), FOnlineErrorInfo());
        return;
    }
    auto Handle = this->__Store__Subsystem->GetHandle();
    if (!this->__Store__Subsystem->IsHandleValid(Handle))
    {
        this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), FOnlineErrorInfo());
        return;
    }
    if (this->__Store__CallUniquenessId != TEXT(""))
    {
        this->__Store__Subsystem->__InFlightRequests.Remove(this->__Store__CallUniquenessId);
    }

    this->OnRevokeAuthTokenComplete.Broadcast(ConvertDangerousUniqueNetIdToRepl(UserId), FOnlineErrorInfo::FromNative(OnlineError));
}

UOnlineIdentitySubsystemGetUserPrivilege* UOnlineIdentitySubsystemGetUserPrivilege::GetUserPrivilege(UOnlineIdentitySubsystem* Subsystem ,FUniqueNetIdRepl LocalUserId, EOnlineUserPrivilege Privilege)
{
    if (!IsValid(Subsystem))
    {
        return nullptr;
    }

    //check(!Subsystem->HasAnyFlags(RF_ClassDefaultObject));

    UOnlineIdentitySubsystemGetUserPrivilege* Node = NewObject<UOnlineIdentitySubsystemGetUserPrivilege>();
    Node->__Store__Subsystem = Subsystem;
    Node->__Store__CallUniquenessId = TEXT("");
    Node->__Store__LocalUserId = LocalUserId;
    Node->__Store__Privilege = Privilege;
    return Node;
}

void UOnlineIdentitySubsystemGetUserPrivilege::Activate()
{
    auto Handle = this->__Store__Subsystem->GetHandle();
    if (!this->__Store__Subsystem->IsHandleValid(Handle))
    {
        this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), EOnlineUserPrivilege(), int64());
        return;
    }
    if (!(this->__Store__LocalUserId.IsValid()))
    {
        this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), EOnlineUserPrivilege(), int64());
        return;
    }

    if (this->__Store__CallUniquenessId != TEXT(""))
    {
        if (this->__Store__Subsystem->__InFlightRequests.Contains(this->__Store__CallUniquenessId))
        {
            this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), EOnlineUserPrivilege(), int64());
            return;
        }

        this->__Store__Subsystem->__InFlightRequests.Add(this->__Store__CallUniquenessId);
    }

    auto __DelegateHandle_OnGetUserPrivilegeComplete = IOnlineIdentity::FOnGetUserPrivilegeCompleteDelegate::CreateUObject(this, &UOnlineIdentitySubsystemGetUserPrivilege::HandleCallback_OnGetUserPrivilegeComplete);

    Handle->GetUserPrivilege(this->__Store__LocalUserId.GetUniqueNetId().ToSharedRef().Get(), (EUserPrivileges::Type)(this->__Store__Privilege), __DelegateHandle_OnGetUserPrivilegeComplete);
}

void UOnlineIdentitySubsystemGetUserPrivilege::HandleCallback_OnGetUserPrivilegeComplete(const FUniqueNetId& LocalUserId, EUserPrivileges::Type Privilege, uint32 PrivilegeResult)
{
    if (!IsValid(this->__Store__Subsystem))
    {
        this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), EOnlineUserPrivilege(), int64());
        return;
    }
    auto Handle = this->__Store__Subsystem->GetHandle();
    if (!this->__Store__Subsystem->IsHandleValid(Handle))
    {
       this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), EOnlineUserPrivilege(), int64());
        return;
    }
    if (this->__Store__CallUniquenessId != TEXT(""))
    {
        this->__Store__Subsystem->__InFlightRequests.Remove(this->__Store__CallUniquenessId);
    }

    this->OnGetUserPrivilegeComplete.Broadcast(ConvertDangerousUniqueNetIdToRepl(LocalUserId), (EOnlineUserPrivilege)(Privilege), (int64)(PrivilegeResult));
}

int32 UOnlineIdentitySubsystem::GetPlatformUserIdFromUniqueNetId(FUniqueNetIdRepl UniqueNetId)
{
    //check(!this->HasAnyFlags(RF_ClassDefaultObject));
    auto Handle = this->GetHandle();
    if (!this->IsHandleValid(Handle))
    {
        return int32();
    }
    if (!(UniqueNetId.IsValid()))
    {
        return int32();
    }


    auto __Result = Handle->GetPlatformUserIdFromUniqueNetId(UniqueNetId.GetUniqueNetId().ToSharedRef().Get());

    return __Result;
}

FString UOnlineIdentitySubsystem::GetAuthType()
{
    //check(!this->HasAnyFlags(RF_ClassDefaultObject));
    auto Handle = this->GetHandle();
    if (!this->IsHandleValid(Handle))
    {
        return TEXT("");
    }


    auto __Result = Handle->GetAuthType();

    return __Result;
}

