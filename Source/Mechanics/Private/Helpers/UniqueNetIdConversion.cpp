// Copyright June Rhodes. All Rights Reserved.

#include "UniqueNetIdConversion.h"

#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystem.h"

FUniqueNetIdRepl ConvertDangerousUniqueNetIdToRepl(const FUniqueNetId &UniqueNetIdByRef)
{
    if (!UniqueNetIdByRef.DoesSharedInstanceExist())
    {
        IOnlineSubsystem *LocalSubsystem = IOnlineSubsystem::Get(UniqueNetIdByRef.GetType());
        if (LocalSubsystem == nullptr)
        {
            LocalSubsystem = IOnlineSubsystem::Get();
        }

        IOnlineIdentityPtr Identity = LocalSubsystem->GetIdentityInterface();
        if (Identity == nullptr)
        {
            return FUniqueNetIdRepl();
        }

        TSharedPtr<const FUniqueNetId> Ptr = Identity->CreateUniquePlayerId(UniqueNetIdByRef.ToString());
        if (Ptr == nullptr)
        {
            return FUniqueNetIdRepl();
        }
        if (!Ptr->GetType().IsEqual(UniqueNetIdByRef.GetType()))
        {
            return FUniqueNetIdRepl();
        }

        return FUniqueNetIdRepl(Ptr);
    }

    return FUniqueNetIdRepl(UniqueNetIdByRef);
}

const TOptional<FOnlineActivityTasksToReset> BP_To_TasksToReset(FOnlineActivityTasksToResetBP TasksToReset)
{
    TOptional<FOnlineActivityTasksToReset> Result;
    Result.GetValue().InProgressTasks = TasksToReset.InProgressTasks;
    Result.GetValue().CompletedTasks = TasksToReset.CompletedTasks;
    return Result;
}

FOnlineActivityTasksToResetBP TasksToReset_To_BP(const TOptional<FOnlineActivityTasksToReset>& TasksToReset)
{
    FOnlineActivityTasksToResetBP Result;
    Result.InProgressTasks = TasksToReset.GetValue().InProgressTasks;
    Result.CompletedTasks = TasksToReset.GetValue().CompletedTasks;
    return Result;
}

EOnlineActivityOutcome BP_To_OutCome(EOnlineActivityOutcomeBP Outcome)
{
    EOnlineActivityOutcome Out = EOnlineActivityOutcome::Failed;
    if (Outcome == EOnlineActivityOutcomeBP::Cancelled)
    {
        Out = EOnlineActivityOutcome::Cancelled;
    }
    else if (Outcome == EOnlineActivityOutcomeBP::Completed)
    {
        Out = EOnlineActivityOutcome::Completed;
    }
    else if (Outcome == EOnlineActivityOutcomeBP::Failed)
    {
        Out = EOnlineActivityOutcome::Failed;
    }
    return Out;
}

EOnlineActivityOutcomeBP OutCome_To_BP(EOnlineActivityOutcome Outcome)
{
    EOnlineActivityOutcomeBP Out = EOnlineActivityOutcomeBP::Failed;
    if (Outcome == EOnlineActivityOutcome::Cancelled)
    {
        Out = EOnlineActivityOutcomeBP::Cancelled;
    }
    else if (Outcome == EOnlineActivityOutcome::Completed)
    {
        Out = EOnlineActivityOutcomeBP::Completed;
    }
    else if (Outcome == EOnlineActivityOutcome::Failed)
    {
        Out = EOnlineActivityOutcomeBP::Failed;
    }
    return Out;
}