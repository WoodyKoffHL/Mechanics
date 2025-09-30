// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"

void UMyGameInstance::Init()
{
    Super::Init();

    TSharedPtr<class IOnlineGameActivity, ESPMode::ThreadSafe>Handle = GetHandle();
    if (Handle.IsValid())
    {
        OnGameActivityActivationRequested_DelegateHandle = Handle->AddOnGameActivityActivationRequestedDelegate_Handle(
            FOnGameActivityActivationRequestedDelegate::CreateUObject(this, &UMyGameInstance::OnGameActivityHandle)
        );
    }
}

void UMyGameInstance::FinishDestroy()
{
    TSharedPtr<class IOnlineGameActivity, ESPMode::ThreadSafe>Handle = GetHandle();
    if (Handle.IsValid())
    {
        Handle->ClearOnGameActivityActivationRequestedDelegate_Handle(OnGameActivityActivationRequested_DelegateHandle);
    }
    Super::FinishDestroy();
}

TSharedPtr<class IOnlineGameActivity, ESPMode::ThreadSafe> UMyGameInstance::GetHandle()
{
    TSharedPtr<class IOnlineGameActivity, ESPMode::ThreadSafe> Pinned = this->HandleWk.Pin();
    if (Pinned == nullptr || !Pinned.IsValid())
    {
        auto Subsystem = Online::GetSubsystem(GetWorld());

        //完成设置，存储对接口的引用
        auto __Handle__ = Subsystem->GetGameActivityInterface();
        this->HandleWk = __Handle__;
        Pinned = __Handle__;
    }

    return Pinned;
}

void UMyGameInstance::OnGameActivityHandle(const FUniqueNetId& LocalUserId, const FString& ActivityId, const FOnlineSessionSearchResult* SessionInfo)
{
    UOnlineHelpers::SetActivityId(ActivityId);
    OnGameActivityActivationRequested.Broadcast(ConvertDangerousUniqueNetIdToRepl(LocalUserId), ActivityId, FOnlineSessionSearchResultBP());
}