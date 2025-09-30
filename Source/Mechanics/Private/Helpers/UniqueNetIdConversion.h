// Copyright June Rhodes. All Rights Reserved.

#pragma once

#include "GameFramework/OnlineReplStructs.h"
#include "Types/OnlineErrorInfo.h"
#include "Interfaces/OnlineGameActivityInterface.h"

FUniqueNetIdRepl ConvertDangerousUniqueNetIdToRepl(const FUniqueNetId &UniqueNetIdByRef);

const TOptional<FOnlineActivityTasksToReset> BP_To_TasksToReset(FOnlineActivityTasksToResetBP TasksToReset);
FOnlineActivityTasksToResetBP TasksToReset_To_BP(const TOptional<FOnlineActivityTasksToReset>& TasksToReset);

EOnlineActivityOutcome BP_To_OutCome(EOnlineActivityOutcomeBP Outcome);
EOnlineActivityOutcomeBP OutCome_To_BP(EOnlineActivityOutcome Outcome);