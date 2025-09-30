// Copyright June Rhodes. All Rights Reserved.

#include "MyOnlineSubsystems/MyOnlineGameActivity/OnlineGameActivitySubsystem.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemTypes.h"
#include "OnlineHelpers.h"

TSharedPtr<class IOnlineGameActivity, ESPMode::ThreadSafe> UOnlineGameActivitySubsystem::GetHandle()
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

    TSharedPtr<class IOnlineGameActivity, ESPMode::ThreadSafe> Pinned = this->HandleWk.Pin();
    if (Pinned == nullptr || !Pinned.IsValid())
    {
        auto Subsystem = Online::GetSubsystem(World);
        //check(Subsystem != nullptr);

        auto __Handle__ = Subsystem->GetGameActivityInterface();
        this->HandleWk = __Handle__;
        Pinned = __Handle__;
    }

    return Pinned;
}

bool UOnlineGameActivitySubsystem::IsHandleValid(const TSharedPtr<class IOnlineGameActivity, ESPMode::ThreadSafe>& InHandle)
{
    return InHandle.IsValid();
}

UOnlineGameActivitySubsystem::UOnlineGameActivitySubsystem()
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
    this->HandleWk = nullptr;
    this->GetHandle();

    TSharedPtr<class IOnlineGameActivity, ESPMode::ThreadSafe> Handle = this->GetHandle();
    if (Handle.IsValid())
    {
        //DelegateHandle_OnGameActivityActivationRequested = Handle->AddOnGameActivityActivationRequestedDelegate_Handle(
        //    FOnGameActivityActivationRequestedDelegate::CreateUObject(this, &UOnlineGameActivitySubsystem::OnGameActivityDelegate)
        //);
    }
}

void UOnlineGameActivitySubsystem::BeginDestroy()
{
    if (this->HasAnyFlags(RF_ClassDefaultObject))
    {
        Super::BeginDestroy();
        return;
    }

    TSharedPtr<class IOnlineGameActivity, ESPMode::ThreadSafe> Handle = this->GetHandle();
    if (Handle.IsValid())
    {
        //Handle->ClearOnGameActivityActivationRequestedDelegate_Handle(this->DelegateHandle_OnGameActivityActivationRequested);
    }

    Super::BeginDestroy();
}

bool UOnlineGameActivitySubsystem::IsSubsystemAvailable()
{
    //check(!this->HasAnyFlags(RF_ClassDefaultObject));
    return this->IsHandleValid(this->GetHandle());
}

//void UOnlineGameActivitySubsystem::OnGameActivityDelegate(const FUniqueNetId& LocalUserId, const FString& ActivityId, const FOnlineSessionSearchResult* SessionInfo)
//{
//    UOnlineHelpers::SetActivityId(ActivityId);
//    OnGameActivityActivationRequested.Broadcast(ConvertDangerousUniqueNetIdToRepl(LocalUserId), ActivityId, FOnlineSessionSearchResultBP());
//}

UOnlineGameActivitySubsystemResetAllActiveActivities* UOnlineGameActivitySubsystemResetAllActiveActivities::ResetAllActiveActivities(UOnlineGameActivitySubsystem* Subsystem, FUniqueNetIdRepl LocalUserId)
{
    if (!IsValid(Subsystem))
    {
        return nullptr;
    }

    //check(!Subsystem->HasAnyFlags(RF_ClassDefaultObject));

    UOnlineGameActivitySubsystemResetAllActiveActivities* Node = NewObject<UOnlineGameActivitySubsystemResetAllActiveActivities>();
    Node->__Store__Subsystem = Subsystem;
    Node->__Store__CallUniquenessId = TEXT("");
    Node->__Store__LocalUserId = LocalUserId;
    return Node;
}

void UOnlineGameActivitySubsystemResetAllActiveActivities::Activate()
{
    auto Handle = this->__Store__Subsystem->GetHandle();
    if (!this->__Store__Subsystem->IsHandleValid(Handle))
    {
        //UE_LOG(LogOnlineBlueprint, Error, TEXT("Subsystem value not valid when Activate() was called"));
        this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), FOnlineErrorInfo());
        return;
    }
    if (!(this->__Store__LocalUserId.IsValid()))
    {
        //UE_LOG(LogOnlineBlueprint, Error, TEXT("Parameter 'LocalUserId' was not valid"));
        this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), FOnlineErrorInfo());
        return;
    }

    if (this->__Store__CallUniquenessId != TEXT(""))
    {
        if (this->__Store__Subsystem->__InFlightRequests.Contains(this->__Store__CallUniquenessId))
        {
            //UE_LOG(LogOnlineBlueprint, Error, TEXT("There is already a request running with ID '%s'. Wait until it is complete before starting another request with the same parameters."), *this->__Store__CallUniquenessId);
            this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), FOnlineErrorInfo());
            return;
        }

        this->__Store__Subsystem->__InFlightRequests.Add(this->__Store__CallUniquenessId);
    }

    auto __DelegateHandle_OnResetAllActiveActivitiesComplete = FOnResetAllActiveActivitiesComplete::CreateUObject(this, &UOnlineGameActivitySubsystemResetAllActiveActivities::HandleCallback_OnResetAllActiveActivitiesComplete);

    Handle->ResetAllActiveActivities(this->__Store__LocalUserId.GetUniqueNetId().ToSharedRef().Get(), __DelegateHandle_OnResetAllActiveActivitiesComplete);
}

void UOnlineGameActivitySubsystemResetAllActiveActivities::HandleCallback_OnResetAllActiveActivitiesComplete(const FUniqueNetId& LocalUserId, const FOnlineError& Status)
{
    if (!IsValid(this->__Store__Subsystem))
    {
        //UE_LOG(LogOnlineBlueprint, Error, TEXT("Subsystem was not valid during callback handling"));
        this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), FOnlineErrorInfo());
        return;
    }
    auto Handle = this->__Store__Subsystem->GetHandle();
    if (!this->__Store__Subsystem->IsHandleValid(Handle))
    {
        //UE_LOG(LogOnlineBlueprint, Error, TEXT("Subsystem handle was not valid during callback handling"));
        this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), FOnlineErrorInfo());
        return;
    }
    if (this->__Store__CallUniquenessId != TEXT(""))
    {
        this->__Store__Subsystem->__InFlightRequests.Remove(this->__Store__CallUniquenessId);
    }

    this->OnResetAllActiveActivitiesComplete.Broadcast(ConvertDangerousUniqueNetIdToRepl(LocalUserId), FOnlineErrorInfo::FromNative(Status));
}

UOnlineGameActivitySubsystemSetActivityAvailability* UOnlineGameActivitySubsystemSetActivityAvailability::SetActivityAvailability(UOnlineGameActivitySubsystem* Subsystem, FUniqueNetIdRepl LocalUserId, FString ActivityId, bool bEnabled)
{
    if (!IsValid(Subsystem))
    {
        //UE_LOG(LogOnlineBlueprint, Error, TEXT("OnlineGameActivitySubsystem is not available. It's possible the online subsystem you are using doesn't support it. Alternatively, make sure that you are not attempting to use Online Subsystem Blueprints during the Init event in a GameInstance. Add a 'Delay 0.1' node between the Init event and the rest of your logic if you need to use Online Subsystem Blueprints this early in game startup."));
        return nullptr;
    }

    //check(!Subsystem->HasAnyFlags(RF_ClassDefaultObject));

    UOnlineGameActivitySubsystemSetActivityAvailability* Node = NewObject<UOnlineGameActivitySubsystemSetActivityAvailability>();
    Node->__Store__Subsystem = Subsystem;
    Node->__Store__CallUniquenessId = TEXT("");
    Node->__Store__LocalUserId = LocalUserId;
    Node->__Store__ActivityId = ActivityId;
    Node->__Store__bEnabled = bEnabled;
    return Node;
}

void UOnlineGameActivitySubsystemSetActivityAvailability::Activate()
{
    auto Handle = this->__Store__Subsystem->GetHandle();
    if (!this->__Store__Subsystem->IsHandleValid(Handle))
    {
        //UE_LOG(LogOnlineBlueprint, Error, TEXT("Subsystem value not valid when Activate() was called"));
        this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), FOnlineErrorInfo());
        return;
    }
    if (!(this->__Store__LocalUserId.IsValid()))
    {
        //UE_LOG(LogOnlineBlueprint, Error, TEXT("Parameter 'LocalUserId' was not valid"));
        this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), FOnlineErrorInfo());
        return;
    }

    if (this->__Store__CallUniquenessId != TEXT(""))
    {
        if (this->__Store__Subsystem->__InFlightRequests.Contains(this->__Store__CallUniquenessId))
        {
            //UE_LOG(LogOnlineBlueprint, Error, TEXT("There is already a request running with ID '%s'. Wait until it is complete before starting another request with the same parameters."), *this->__Store__CallUniquenessId);
            this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), FOnlineErrorInfo());
            return;
        }

        this->__Store__Subsystem->__InFlightRequests.Add(this->__Store__CallUniquenessId);
    }

    auto __DelegateHandle_OnSetActivityAvailabilityComplete = FOnSetActivityAvailabilityComplete::CreateUObject(this, &UOnlineGameActivitySubsystemSetActivityAvailability::HandleCallback_OnSetActivityAvailabilityComplete);

    Handle->SetActivityAvailability(this->__Store__LocalUserId.GetUniqueNetId().ToSharedRef().Get(), this->__Store__ActivityId, this->__Store__bEnabled, __DelegateHandle_OnSetActivityAvailabilityComplete);
}

void UOnlineGameActivitySubsystemSetActivityAvailability::HandleCallback_OnSetActivityAvailabilityComplete(const FUniqueNetId& LocalUserId, const FOnlineError& Status)
{
    if (!IsValid(this->__Store__Subsystem))
    {
        //UE_LOG(LogOnlineBlueprint, Error, TEXT("Subsystem was not valid during callback handling"));
        this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), FOnlineErrorInfo());
        return;
    }
    auto Handle = this->__Store__Subsystem->GetHandle();
    if (!this->__Store__Subsystem->IsHandleValid(Handle))
    {
        //UE_LOG(LogOnlineBlueprint, Error, TEXT("Subsystem handle was not valid during callback handling"));
        this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), FOnlineErrorInfo());
        return;
    }
    if (this->__Store__CallUniquenessId != TEXT(""))
    {
        this->__Store__Subsystem->__InFlightRequests.Remove(this->__Store__CallUniquenessId);
    }

    this->OnSetActivityAvailabilityComplete.Broadcast(ConvertDangerousUniqueNetIdToRepl(LocalUserId), FOnlineErrorInfo::FromNative(Status));
}

UOnlineGameActivitySubsystemSetActivityPriority* UOnlineGameActivitySubsystemSetActivityPriority::SetActivityPriority(UOnlineGameActivitySubsystem* Subsystem, FUniqueNetIdRepl LocalUserId, TMap<FString, int32> PrioritizedActivities)
{
    if (!IsValid(Subsystem))
    {
        //UE_LOG(LogOnlineBlueprint, Error, TEXT("OnlineGameActivitySubsystem is not available. It's possible the online subsystem you are using doesn't support it. Alternatively, make sure that you are not attempting to use Online Subsystem Blueprints during the Init event in a GameInstance. Add a 'Delay 0.1' node between the Init event and the rest of your logic if you need to use Online Subsystem Blueprints this early in game startup."));
        return nullptr;
    }

    //check(!Subsystem->HasAnyFlags(RF_ClassDefaultObject));

    UOnlineGameActivitySubsystemSetActivityPriority* Node = NewObject<UOnlineGameActivitySubsystemSetActivityPriority>();
    Node->__Store__Subsystem = Subsystem;
    Node->__Store__CallUniquenessId = TEXT("");
    Node->__Store__LocalUserId = LocalUserId;
    Node->__Store__PrioritizedActivities = PrioritizedActivities;
    return Node;
}

void UOnlineGameActivitySubsystemSetActivityPriority::Activate()
{
    auto Handle = this->__Store__Subsystem->GetHandle();
    if (!this->__Store__Subsystem->IsHandleValid(Handle))
    {
        //UE_LOG(LogOnlineBlueprint, Error, TEXT("Subsystem value not valid when Activate() was called"));
        this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), FOnlineErrorInfo());
        return;
    }
    if (!(this->__Store__LocalUserId.IsValid()))
    {
        //UE_LOG(LogOnlineBlueprint, Error, TEXT("Parameter 'LocalUserId' was not valid"));
        this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), FOnlineErrorInfo());
        return;
    }
    if (!(ValidateMapElements<FString, int32>(this->__Store__PrioritizedActivities, [](const FString& Val) { return true; }, [](const int32& Val) { return true; })))
    {
        //UE_LOG(LogOnlineBlueprint, Error, TEXT("Parameter 'PrioritizedActivities' was not valid"));
        this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), FOnlineErrorInfo());
        return;
    }

    if (this->__Store__CallUniquenessId != TEXT(""))
    {
        if (this->__Store__Subsystem->__InFlightRequests.Contains(this->__Store__CallUniquenessId))
        {
            //UE_LOG(LogOnlineBlueprint, Error, TEXT("There is already a request running with ID '%s'. Wait until it is complete before starting another request with the same parameters."), *this->__Store__CallUniquenessId);
            this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), FOnlineErrorInfo());
            return;
        }

        this->__Store__Subsystem->__InFlightRequests.Add(this->__Store__CallUniquenessId);
    }

    auto __DelegateHandle_OnSetActivityPriorityComplete = FOnSetActivityPriorityComplete::CreateUObject(this, &UOnlineGameActivitySubsystemSetActivityPriority::HandleCallback_OnSetActivityPriorityComplete);

    Handle->SetActivityPriority(this->__Store__LocalUserId.GetUniqueNetId().ToSharedRef().Get(), ConvertMapElements<FString, FString, int32, int32>(this->__Store__PrioritizedActivities, [](const FString& Val) { return Val; }, [](const int32& Val) { return Val; }), __DelegateHandle_OnSetActivityPriorityComplete);
}

void UOnlineGameActivitySubsystemSetActivityPriority::HandleCallback_OnSetActivityPriorityComplete(const FUniqueNetId& LocalUserId, const FOnlineError& Status)
{
    if (!IsValid(this->__Store__Subsystem))
    {
        //UE_LOG(LogOnlineBlueprint, Error, TEXT("Subsystem was not valid during callback handling"));
        this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), FOnlineErrorInfo());
        return;
    }
    auto Handle = this->__Store__Subsystem->GetHandle();
    if (!this->__Store__Subsystem->IsHandleValid(Handle))
    {
        //UE_LOG(LogOnlineBlueprint, Error, TEXT("Subsystem handle was not valid during callback handling"));
        this->OnCallFailed.Broadcast(FUniqueNetIdRepl(), FOnlineErrorInfo());
        return;
    }
    if (this->__Store__CallUniquenessId != TEXT(""))
    {
        this->__Store__Subsystem->__InFlightRequests.Remove(this->__Store__CallUniquenessId);
    }

    this->OnSetActivityPriorityComplete.Broadcast(ConvertDangerousUniqueNetIdToRepl(LocalUserId), FOnlineErrorInfo::FromNative(Status));
}


UOnlineGameActivitySubsystemStartActivity* UOnlineGameActivitySubsystemStartActivity::StartActivity(UOnlineGameActivitySubsystem* Subsystem, FUniqueNetIdRepl LocalUserId, FString ActivityId/*, FBOnlineEventParms Parms*/)
{
    if (!IsValid(Subsystem))
    {
        return nullptr;
    }
    ////check(!Subsystem->HasAnyFlags(RF_ClassDefaultObject));

    UOnlineGameActivitySubsystemStartActivity* Node = NewObject<UOnlineGameActivitySubsystemStartActivity>();
    Node->__Store__Subsystem = Subsystem;
    Node->__Store__CallUniquenessId = TEXT("");
    Node->__Store__LocalUserId = LocalUserId;
    Node->__Store__ActivityId = ActivityId;
    //Node->__Store__Parms = Parms;
    return Node;
}

void UOnlineGameActivitySubsystemStartActivity::Activate()
{
    auto Handle = this->__Store__Subsystem->GetHandle();
    if (!this->__Store__Subsystem->IsHandleValid(Handle))
    {
        this->Failed.Broadcast(FUniqueNetIdRepl(), FString(), FOnlineErrorInfo());
        return;
    }
    if (!(this->__Store__LocalUserId.IsValid()))
    {
        this->Failed.Broadcast(FUniqueNetIdRepl(), FString(), FOnlineErrorInfo());
        return;
    }

    if (this->__Store__CallUniquenessId != TEXT(""))
    {
        if (this->__Store__Subsystem->__InFlightRequests.Contains(this->__Store__CallUniquenessId))
        {
            this->Failed.Broadcast(FUniqueNetIdRepl(), FString(), FOnlineErrorInfo());
            return;
        }

        this->__Store__Subsystem->__InFlightRequests.Add(this->__Store__CallUniquenessId);
    }

    auto __DelegateHandle_OnStartActivityComplete = FOnStartActivityComplete::CreateUObject(this, &UOnlineGameActivitySubsystemStartActivity::HandleCallback_OnStartActivityComplete);

    Handle->StartActivity(this->__Store__LocalUserId.GetUniqueNetId().ToSharedRef().Get(), this->__Store__ActivityId, FOnlineEventParms(), __DelegateHandle_OnStartActivityComplete);

}

void UOnlineGameActivitySubsystemStartActivity::HandleCallback_OnStartActivityComplete(const FUniqueNetId& LocalUserId, const FString& ActivityId, const FOnlineError& Status)
{
    if (!IsValid(this->__Store__Subsystem))
    {
        this->Failed.Broadcast(FUniqueNetIdRepl(), FString(), FOnlineErrorInfo());
        return;
    }
    auto Handle = this->__Store__Subsystem->GetHandle();
    if (!this->__Store__Subsystem->IsHandleValid(Handle))
    {
        this->Failed.Broadcast(FUniqueNetIdRepl(), FString(), FOnlineErrorInfo());
        return;
    }
    if (this->__Store__CallUniquenessId != TEXT(""))
    {
        this->__Store__Subsystem->__InFlightRequests.Remove(this->__Store__CallUniquenessId);
    }

    this->Complete.Broadcast(ConvertDangerousUniqueNetIdToRepl(LocalUserId), ActivityId, FOnlineErrorInfo::FromNative(Status));
}

UOnlineGameActivitySubsystemEndActivity* UOnlineGameActivitySubsystemEndActivity::EndActivity(UOnlineGameActivitySubsystem* Subsystem, FUniqueNetIdRepl LocalUserId, FString ActivityId, EOnlineActivityOutcomeBP ActivityOutcome/*, FBOnlineEventParms Parms*/)
{
    if (!IsValid(Subsystem))
    {
        return nullptr;
    }

    ////check(!Subsystem->HasAnyFlags(RF_ClassDefaultObject));

    UOnlineGameActivitySubsystemEndActivity* Node = NewObject<UOnlineGameActivitySubsystemEndActivity>();
    Node->__Store__Subsystem = Subsystem;
    Node->__Store__CallUniquenessId = TEXT("");
    Node->__Store__LocalUserId = LocalUserId;
    Node->__Store__ActivityId = ActivityId;
    Node->__Store__Outcome = ActivityOutcome;
    //Node->__Store__Parms = Parms;

    return Node;
}

void UOnlineGameActivitySubsystemEndActivity::Activate()
{
    auto Handle = this->__Store__Subsystem->GetHandle();
    if (!this->__Store__Subsystem->IsHandleValid(Handle))
    {
        this->Failed.Broadcast(FUniqueNetIdRepl(), FString(), EOnlineActivityOutcomeBP(), FOnlineErrorInfo());
        return;
    }
    if (!(this->__Store__LocalUserId.IsValid()))
    {
        this->Failed.Broadcast(FUniqueNetIdRepl(), FString(), EOnlineActivityOutcomeBP(), FOnlineErrorInfo());
        return;
    }

    if (this->__Store__CallUniquenessId != TEXT(""))
    {
        if (this->__Store__Subsystem->__InFlightRequests.Contains(this->__Store__CallUniquenessId))
        {
            this->Failed.Broadcast(FUniqueNetIdRepl(), FString(), EOnlineActivityOutcomeBP(), FOnlineErrorInfo());
            return;
        }

        this->__Store__Subsystem->__InFlightRequests.Add(this->__Store__CallUniquenessId);
    }

    auto __DelegateHandle_OnEndActivityComplete = FOnEndActivityComplete::CreateUObject(this, &UOnlineGameActivitySubsystemEndActivity::HandleCallback_OnEndActivityComplete);

    Handle->EndActivity(this->__Store__LocalUserId.GetUniqueNetId().ToSharedRef().Get(), this->__Store__ActivityId, BP_To_OutCome(this->__Store__Outcome), FOnlineEventParms(), __DelegateHandle_OnEndActivityComplete);
}

void UOnlineGameActivitySubsystemEndActivity::HandleCallback_OnEndActivityComplete(const FUniqueNetId& LocalUserId, const FString& ActivityId, const EOnlineActivityOutcome& ActivityOutcome, const FOnlineError& Status)
{
    if (!IsValid(this->__Store__Subsystem))
    {
        this->Failed.Broadcast(FUniqueNetIdRepl(), FString(), EOnlineActivityOutcomeBP(), FOnlineErrorInfo());
        return;
    }
    auto Handle = this->__Store__Subsystem->GetHandle();
    if (!this->__Store__Subsystem->IsHandleValid(Handle))
    {
        this->Failed.Broadcast(FUniqueNetIdRepl(), FString(), EOnlineActivityOutcomeBP(), FOnlineErrorInfo());
        return;
    }
    if (this->__Store__CallUniquenessId != TEXT(""))
    {
        this->__Store__Subsystem->__InFlightRequests.Remove(this->__Store__CallUniquenessId);
    }

    this->Complete.Broadcast(ConvertDangerousUniqueNetIdToRepl(LocalUserId), ActivityId, OutCome_To_BP(ActivityOutcome), FOnlineErrorInfo::FromNative(Status));
}

UOnlineGameActivitySubsystemResumeActivity* UOnlineGameActivitySubsystemResumeActivity::ResumeActivity(UOnlineGameActivitySubsystem* Subsystem, FUniqueNetIdRepl LocalUserId, FString ActivityId, FOnlineActivityTasksToResetBP TasksToReset)
{
    if (!IsValid(Subsystem))
    {
        return nullptr;
    }

    ////check(!Subsystem->HasAnyFlags(RF_ClassDefaultObject));

    UOnlineGameActivitySubsystemResumeActivity* Node = NewObject<UOnlineGameActivitySubsystemResumeActivity>();
    Node->__Store__Subsystem = Subsystem;
    Node->__Store__CallUniquenessId = TEXT("");
    Node->__Store__LocalUserId = LocalUserId;
    Node->__Store__ActivityId = ActivityId;
    Node->__Store__TasksToReset = TasksToReset;
    return Node;
}

void UOnlineGameActivitySubsystemResumeActivity::Activate()
{
    auto Handle = this->__Store__Subsystem->GetHandle();
    if (!this->__Store__Subsystem->IsHandleValid(Handle))
    {
        this->Failed.Broadcast(FUniqueNetIdRepl(), FString(), FOnlineErrorInfo());
        return;
    }
    if (!(this->__Store__LocalUserId.IsValid()))
    {
        this->Failed.Broadcast(FUniqueNetIdRepl(), FString(), FOnlineErrorInfo());
        return;
    }

    if (this->__Store__CallUniquenessId != TEXT(""))
    {
        if (this->__Store__Subsystem->__InFlightRequests.Contains(this->__Store__CallUniquenessId))
        {
            this->Failed.Broadcast(FUniqueNetIdRepl(), FString(), FOnlineErrorInfo());
            return;
        }

        this->__Store__Subsystem->__InFlightRequests.Add(this->__Store__CallUniquenessId);
    }

    auto __DelegateHandle_OnResumeActivityComplete = FOnResumeActivityComplete::CreateUObject(this, &UOnlineGameActivitySubsystemResumeActivity::HandleCallback_OnResumeActivityComplete);

    Handle->ResumeActivity(this->__Store__LocalUserId.GetUniqueNetId().ToSharedRef().Get(), this->__Store__ActivityId, BP_To_TasksToReset(this->__Store__TasksToReset), __DelegateHandle_OnResumeActivityComplete);
}

void UOnlineGameActivitySubsystemResumeActivity::HandleCallback_OnResumeActivityComplete(const FUniqueNetId& LocalUserId, const FString& ActivityId, const FOnlineError& Status)
{
    if (!IsValid(this->__Store__Subsystem))
    {
        this->Failed.Broadcast(FUniqueNetIdRepl(), FString(), FOnlineErrorInfo());
        return;
    }
    auto Handle = this->__Store__Subsystem->GetHandle();
    if (!this->__Store__Subsystem->IsHandleValid(Handle))
    {
        this->Failed.Broadcast(FUniqueNetIdRepl(), FString(), FOnlineErrorInfo());
        return;
    }
    if (this->__Store__CallUniquenessId != TEXT(""))
    {
        this->__Store__Subsystem->__InFlightRequests.Remove(this->__Store__CallUniquenessId);
    }

    this->Complete.Broadcast(ConvertDangerousUniqueNetIdToRepl(LocalUserId), ActivityId, FOnlineErrorInfo::FromNative(Status));
}
