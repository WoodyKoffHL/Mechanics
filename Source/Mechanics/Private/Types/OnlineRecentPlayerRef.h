// Copyright June Rhodes. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Types/OnlineUserRef.h"
#include "Online/CoreOnline.h"

#include "OnlineRecentPlayerRef.generated.h"

UCLASS(BlueprintType, Transient, Meta = (DontUseGenericSpawnObject, IsBlueprintBase = "false"))
class MECHANICS_API UOnlineRecentPlayerRef : public UOnlineUserRef
{
    GENERATED_BODY()

private:
    TSharedPtr<FOnlineRecentPlayer> RecentPlayer;

public:
    static UOnlineRecentPlayerRef *FromRecentPlayer(const TSharedPtr<FOnlineRecentPlayer> &RecentPlayer);
    TSharedPtr<FOnlineRecentPlayer> GetRecentPlayer() const
    {
        return this->RecentPlayer;
    }

    UFUNCTION(BlueprintPure, Category = "MyOnlineSubsystem")
    FDateTime GetLastSeen();
};