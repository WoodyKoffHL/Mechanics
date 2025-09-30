// Copyright June Rhodes. All Rights Reserved.

#include "MyOnlineSubsystems/MyOnlineEventsSubsystem/OnlineEventsSubsystem.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemTypes.h"

TSharedPtr<class IOnlineEvents, ESPMode::ThreadSafe> UOnlineEventsSubsystem::GetHandle()
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

    TSharedPtr<class IOnlineEvents, ESPMode::ThreadSafe> Pinned = this->HandleWk.Pin();
    if (Pinned == nullptr || !Pinned.IsValid())
    {
        auto Subsystem = Online::GetSubsystem(World);
        //check(Subsystem != nullptr);
        
        auto __Handle__ = Subsystem->GetEventsInterface();
        this->HandleWk = __Handle__;
        Pinned = __Handle__;
    }

    return Pinned;
}

bool UOnlineEventsSubsystem::IsHandleValid(const TSharedPtr<class IOnlineEvents, ESPMode::ThreadSafe>& InHandle)
{
    return InHandle.IsValid();
}

UOnlineEventsSubsystem::UOnlineEventsSubsystem()
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
    
    TSharedPtr<class IOnlineEvents, ESPMode::ThreadSafe> Handle = this->GetHandle();
    if (Handle.IsValid())
    {
        for (auto i = 0; i < MAX_LOCAL_PLAYERS; i++)
        {
        }
    }
}

void UOnlineEventsSubsystem::BeginDestroy()
{
    if (this->HasAnyFlags(RF_ClassDefaultObject))
    {
        Super::BeginDestroy();
        return;
    }

    TSharedPtr<class IOnlineEvents, ESPMode::ThreadSafe> Handle = this->GetHandle();
    if (Handle.IsValid())
    {
        for (auto i = 0; i < MAX_LOCAL_PLAYERS; i++)
        {
        }
        
    }

    Super::BeginDestroy();
}

bool UOnlineEventsSubsystem::IsSubsystemAvailable()
{
    //check(!this->HasAnyFlags(RF_ClassDefaultObject));
    return this->IsHandleValid(this->GetHandle());
}
