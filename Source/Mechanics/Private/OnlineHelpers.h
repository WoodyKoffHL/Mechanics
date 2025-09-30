// Copyright June Rhodes. All Rights Reserved.

#pragma once

#include "MyOnlineSubsystems/MyOnlineSessionSubsystem/OnlineSessionSubsystem.h"
#include "CoreMinimal.h"
#include "GameFramework/OnlineReplStructs.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/OSBFileTypes.h"
#include "Types/OSBPartyTypes.h"
#include "GameFramework/SaveGame.h"
#include "MyOnlineSubsystems/MyOnlinePartySubsystem/OnlinePartySubsystem.h"

#include "OnlineHelpers.generated.h"

UCLASS()
class MECHANICS_API UOnlineHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(
        BlueprintPure,
        Category = "Online",
        Meta =
            (DisplayName = "ToString (FUniqueNetIdRepl)",
             CompactNodeTitle = "->",
             Keywords = "FUniqueNetIdRepl netid cast convert to string",
             BlueprintAutocast))
    static FString Conv_FUniqueNetIdReplToString(FUniqueNetIdRepl InNetId);

    UFUNCTION(
        BlueprintPure,
        Category = "Online",
        Meta =
            (DisplayName = "Get Subsystem Type",
             Keywords = "FUniqueNetIdRepl netid subsystem type name",
             BlueprintAutocast))
    static FName FUniqueNetIdGetType(FUniqueNetIdRepl InNetId);

    UFUNCTION(
        BlueprintPure,
        Category = "Online",
        Meta =
            (DisplayName = "Is Valid (FUniqueNetIdRepl)",
             Keywords = "FUniqueNetIdRepl netid subsystem is valid",
             BlueprintAutocast))
    static bool FUniqueNetIdIsValid(FUniqueNetIdRepl InNetId);

    UFUNCTION(
        BlueprintPure,
        meta = (DisplayName = "Equal Unique Net Id", CompactNodeTitle = "==", Keywords = "== equal"),
        Category = "Online")
    static bool EqualEqual_FUniqueNetIdReplFUniqueNetIdRepl(const FUniqueNetIdRepl &InA, const FUniqueNetIdRepl &InB);

    UFUNCTION(
        BlueprintPure,
        meta = (DisplayName = "Equal Party Id", CompactNodeTitle = "==", Keywords = "== equal"),
        Category = "Online")
    static bool EqualEqual_PartyIdPartyId(UPartyId *A, UPartyId *B);

    UFUNCTION(
        BlueprintPure,
        meta = (DisplayName = "Party Id Is Valid?", CompactNodeTitle = "Valid?", Keywords = "is valid"),
        Category = "Online")
    static bool IsValid_PartyId(UPartyId *A);

    UFUNCTION(
        BlueprintPure,
        meta = (DisplayName = "Equal Lobby Id", CompactNodeTitle = "==", Keywords = "== equal"),
        Category = "Online")
    static bool EqualEqual_LobbyIdLobbyId(ULobbyId *A, ULobbyId *B);

    UFUNCTION(
        BlueprintPure,
        meta = (DisplayName = "Lobby Id Is Valid?", CompactNodeTitle = "Valid?", Keywords = "is valid"),
        Category = "Online")
    static bool IsValid_LobbyId(ULobbyId *A);

    UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Primary Party Type"), Category = "Online")
    static int64 GetPrimaryPartyType()
    {
        return (int64)IOnlinePartySystem::GetPrimaryPartyTypeId().GetValue();
    }

    UFUNCTION(
        BlueprintPure,
        meta = (DisplayName = "Get Resolved Connect String (from Session Name)"),
        Category = "Online|Sessions")
    static void GetResolvedConnectStringByName(
        class UOnlineSessionSubsystem *Subsystem,
        FName SessionName,
        FName PortType,
        bool &bWasSuccessful,
        FString &OutConnectInfo);

    UFUNCTION(
        BlueprintPure,
        meta = (DisplayName = "Get Resolved Connect String (from Search Result)"),
        Category = "Online|Sessions")
    static void GetResolvedConnectStringBySearchResult(
        class UOnlineSessionSubsystem *Subsystem,
        FOnlineSessionSearchResultBP SearchResult,
        FName PortType,
        bool &bWasSuccessful,
        FString &OutConnectInfo);

    UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Controller Unique Net Id"), Category = "Online")
    static FUniqueNetIdRepl GetControllerUniqueNetId(APlayerController *PlayerController);

    UFUNCTION(
        BlueprintCallable,
        meta = (DisplayName = "Create Modifiable Party Data", Keywords = "modify party data set"),
        Category = "Online")
    static UMutablePartyData *CreateMutablePartyData(UReadablePartyData *ReadOnlyPartyData);

    UFUNCTION(
        BlueprintCallable,
        meta =
            (DisplayName = "Create File Data from String",
             Keywords = "file data filedata create user title cloud string"),
        Category = "Online|Files")
    static UFileData *CreateFileDataFromString(const FString &String);

    UFUNCTION(
        BlueprintCallable,
        meta = (DisplayName = "Read File Data as String", Keywords = "file data filedata read user title cloud string"),
        Category = "Online|Files")
    static FString ReadFileDataAsString(UFileData *FileData);

    UFUNCTION(
        BlueprintCallable,
        meta =
            (DisplayName = "Create File Data from Save Game",
             Keywords = "file data filedata create user title cloud save game"),
        Category = "Online|Files")
    static UFileData *CreateFileDataFromSaveGame(USaveGame* SaveGame);

    UFUNCTION(
        BlueprintCallable,
        meta = (DisplayName = "Read File Data as Save Game", Keywords = "file data filedata read user title cloud save game"),
        Category = "Online|Files")
    static USaveGame *ReadFileDataAsSaveGame(UFileData *FileData);

    UFUNCTION(
        BlueprintCallable,
        meta =
            (DisplayName = "Convert Lobby Id to Party Join Info",
             Keywords = "convert lobby id party join info",
             BlueprintAutocast),
        Category = "Online")
    static UOnlinePartyJoinInfo *Conv_ULobbyIdToUOnlinePartyJoinInfo(
        FUniqueNetIdRepl InLocalUserId,
        UOnlinePartySubsystem* InPartySubsystem,
        ULobbyId *InLobbyId);

    UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Activity Id"), Category = "Online")
        static const FString GetActivityId();

    static void SetActivityId(const FString& ActivityId);

    static FString ActivityID;
};